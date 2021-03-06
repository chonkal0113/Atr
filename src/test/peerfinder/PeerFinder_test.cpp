
#include <ripple/basics/chrono.h>
#include <ripple/basics/Slice.h>
#include <ripple/protocol/PublicKey.h>
#include <ripple/protocol/SecretKey.h>
#include <ripple/peerfinder/impl/Logic.h>
#include <ripple/beast/unit_test.h>
#include <test/unit_test/SuiteJournal.h>
namespace ripple {
namespace PeerFinder {
class PeerFinder_test : public beast::unit_test::suite
{
    test::SuiteJournal journal_;
public:
    PeerFinder_test()
    : journal_ ("PeerFinder_test", *this)
    { }
    struct TestStore : Store
    {
        std::size_t
        load (load_callback const& cb) override
        {
            return 0;
        }
        void
        save (std::vector <Entry> const&) override
        {
        }
    };
    struct TestChecker
    {
        void
        stop()
        {
        }
        void
        wait()
        {
        }
        template <class Handler>
        void
        async_connect (beast::IP::Endpoint const& ep,
            Handler&& handler)
        {
            boost::system::error_code ec;
            handler (ep, ep, ec);
        }
    };
    void
    test_backoff1()
    {
        auto const seconds = 10000;
        testcase("backoff 1");
        TestStore store;
        TestChecker checker;
        TestStopwatch clock;
        Logic<TestChecker> logic (clock, store, checker, journal_);
        logic.addFixedPeer ("test",
            beast::IP::Endpoint::from_string("65.0.0.1:5"));
        {
            Config c;
            c.autoConnect = false;
            c.listeningPort = 1024;
            logic.config(c);
        }
        std::size_t n = 0;
        for (std::size_t i = 0; i < seconds; ++i)
        {
            auto const list = logic.autoconnect();
            if (! list.empty())
            {
                BEAST_EXPECT(list.size() == 1);
                auto const slot = logic.new_outbound_slot(list.front());
                BEAST_EXPECT(logic.onConnected(slot,
                    beast::IP::Endpoint::from_string("65.0.0.2:5")));
                logic.on_closed(slot);
                ++n;
            }
            clock.advance(std::chrono::seconds(1));
            logic.once_per_second();
        }
        BEAST_EXPECT(n < 20);
    }
    void
    test_backoff2()
    {
        auto const seconds = 10000;
        testcase("backoff 2");
        TestStore store;
        TestChecker checker;
        TestStopwatch clock;
        Logic<TestChecker> logic (clock, store, checker, journal_);
        logic.addFixedPeer ("test",
            beast::IP::Endpoint::from_string("65.0.0.1:5"));
        {
            Config c;
            c.autoConnect = false;
            c.listeningPort = 1024;
            logic.config(c);
        }
        PublicKey const pk (randomKeyPair(KeyType::secp256k1).first);
        std::size_t n = 0;
        for (std::size_t i = 0; i < seconds; ++i)
        {
            auto const list = logic.autoconnect();
            if (! list.empty())
            {
                BEAST_EXPECT(list.size() == 1);
                auto const slot = logic.new_outbound_slot(list.front());
                if (! BEAST_EXPECT(logic.onConnected(slot,
                        beast::IP::Endpoint::from_string("65.0.0.2:5"))))
                    return;
                std::string s = ".";
                if (! BEAST_EXPECT(logic.activate(slot, pk, false) ==
                        PeerFinder::Result::success))
                    return;
                logic.on_closed(slot);
                ++n;
            }
            clock.advance(std::chrono::seconds(1));
            logic.once_per_second();
        }
        BEAST_EXPECT(n <= (seconds+59)/60);
    }
    void run () override
    {
        test_backoff1();
        test_backoff2();
    }
};
BEAST_DEFINE_TESTSUITE(PeerFinder,PeerFinder,ripple);
}
}
