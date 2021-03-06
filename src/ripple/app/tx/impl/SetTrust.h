#ifndef RIPPLE_TX_SETTRUST_H_INCLUDED
#define RIPPLE_TX_SETTRUST_H_INCLUDED
#include <ripple/protocol/Quality.h>
#include <ripple/app/tx/impl/Transactor.h>
#include <ripple/basics/Log.h>
#include <ripple/protocol/Indexes.h>
#include <ripple/protocol/TxFlags.h>
namespace ripple {
class SetTrust
    : public Transactor
{
public:
    explicit SetTrust (ApplyContext& ctx)
        : Transactor(ctx)
    {
    }
    static
    NotTEC
    preflight (PreflightContext const& ctx);
    static
    TER
    preclaim(PreclaimContext const& ctx);
    TER doApply () override;
};
} 
#endif
