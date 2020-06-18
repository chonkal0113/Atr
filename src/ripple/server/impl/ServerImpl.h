#ifndef RIPPLE_SERVER_SERVERIMPL_H_INCLUDED
#define RIPPLE_SERVER_SERVERIMPL_H_INCLUDED
#include <ripple/basics/chrono.h>
#include <ripple/server/Server.h>
#include <ripple/server/impl/Door.h>
#include <ripple/server/impl/io_list.h>
#include <ripple/beast/core/List.h>
#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include <array>
#include <chrono>
#include <mutex>
namespace ripple {
using Endpoints = std::vector<boost::asio::ip::tcp::endpoint>;
class Server
{
public:
    virtual
    ~Server() = default;
    virtual
    beast::Journal
    journal() = 0;
    virtual
    Endpoints
    ports (std::vector<Port> const& v) = 0;
    virtual
    void
    close() = 0;
};
template<class Handler>
class ServerImpl : public Server
{
private:
    using clock_type = std::chrono::system_clock;
    enum
    {
        historySize = 100
    };
    Handler& handler_;
    beast::Journal j_;
    boost::asio::io_service& io_service_;
    boost::asio::io_service::strand strand_;
    boost::optional <boost::asio::io_service::work> work_;
    std::mutex m_;
    std::vector<Port> ports_;
    std::vector<std::weak_ptr<Door<Handler>>> list_;
    int high_ = 0;
    std::array <std::size_t, 64> hist_;
    io_list ios_;
public:
    ServerImpl(Handler& handler,
        boost::asio::io_service& io_service, beast::Journal journal);
    ~ServerImpl();
    beast::Journal
    journal() override
    {
        return j_;
    }
    Endpoints
    ports (std::vector<Port> const& ports) override;
    void
    close() override;
    io_list&
    ios()
    {
        return ios_;
    }
    boost::asio::io_service&
    get_io_service()
    {
        return io_service_;
    }
    bool
    closed();
private:
    static
    int
    ceil_log2 (unsigned long long x);
};
template<class Handler>
ServerImpl<Handler>::
ServerImpl(Handler& handler,
        boost::asio::io_service& io_service, beast::Journal journal)
    : handler_(handler)
    , j_(journal)
    , io_service_(io_service)
    , strand_(io_service_)
    , work_(io_service_)
{
}
template<class Handler>
ServerImpl<Handler>::
~ServerImpl()
{
    work_ = boost::none;
    ios_.close();
    ios_.join();
}
template<class Handler>
Endpoints
ServerImpl<Handler>::
ports (std::vector<Port> const& ports)
{
    if (closed())
        Throw<std::logic_error> ("ports() on closed Server");
    ports_.reserve(ports.size());
    Endpoints eps;
    eps.reserve(ports.size());
    for(auto const& port : ports)
    {
        ports_.push_back(port);
        if(auto sp = ios_.emplace<Door<Handler>>(handler_,
            io_service_, ports_.back(), j_))
        {
            list_.push_back(sp);
            eps.push_back(sp->get_endpoint());
            sp->run();
        }
    }
    return eps;
}
template<class Handler>
void
ServerImpl<Handler>::
close()
{
    ios_.close(
    [&]
    {
        work_ = boost::none;
        handler_.onStopped(*this);
    });
}
template<class Handler>
bool
ServerImpl<Handler>::
closed()
{
    return ios_.closed();
}
} 
#endif
