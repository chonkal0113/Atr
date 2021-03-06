
#include <ripple/protocol/ErrorCodes.h>
#include <cassert>
namespace ripple {
namespace RPC {
namespace detail {
constexpr static ErrorInfo unorderedErrorInfos[]
{
    {rpcACT_BITCOIN,           "actBitcoin",          "Account is bitcoin address."},
    {rpcACT_MALFORMED,         "actMalformed",        "Account malformed."},
    {rpcACT_NOT_FOUND,         "actNotFound",         "Account not found."},
    {rpcALREADY_MULTISIG,      "alreadyMultisig",     "Already multisigned."},
    {rpcALREADY_SINGLE_SIG,    "alreadySingleSig",    "Already single-signed."},
    {rpcAMENDMENT_BLOCKED,     "amendmentBlocked",    "Amendment blocked, need upgrade."},
    {rpcATX_DEPRECATED,        "deprecated",          "Use the new API or specify a ledger range."},
    {rpcBAD_FEATURE,           "badFeature",          "Feature unknown or invalid."},
    {rpcBAD_ISSUER,            "badIssuer",           "Issuer account malformed."},
    {rpcBAD_MARKET,            "badMarket",           "No such market."},
    {rpcBAD_SECRET,            "badSecret",           "Secret does not match account."},
    {rpcBAD_SEED,              "badSeed",             "Disallowed seed."},
    {rpcBAD_SYNTAX,            "badSyntax",           "Syntax error."},
    {rpcCHANNEL_MALFORMED,     "channelMalformed",    "Payment channel is malformed."},
    {rpcCHANNEL_AMT_MALFORMED, "channelAmtMalformed", "Payment channel amount is malformed."},
    {rpcCOMMAND_MISSING,       "commandMissing",      "Missing command entry."},
    {rpcDST_ACT_MALFORMED,     "dstActMalformed",     "Destination account is malformed."},
    {rpcDST_ACT_MISSING,       "dstActMissing",       "Destination account not provided."},
    {rpcDST_ACT_NOT_FOUND,     "dstActNotFound",      "Destination account not found."},
    {rpcDST_AMT_MALFORMED,     "dstAmtMalformed",     "Destination amount/currency/issuer is malformed."},
    {rpcDST_AMT_MISSING,       "dstAmtMissing",       "Destination amount/currency/issuer is missing."},
    {rpcDST_ISR_MALFORMED,     "dstIsrMalformed",     "Destination issuer is malformed."},
    {rpcFORBIDDEN,             "forbidden",           "Bad credentials."},
    {rpcHIGH_FEE,              "highFee",             "Current transaction fee exceeds your limit."},
    {rpcINTERNAL,              "internal",            "Internal error."},
    {rpcINVALID_PARAMS,        "invalidParams",       "Invalid parameters."},
    {rpcJSON_RPC,              "json_rpc",            "JSON-RPC transport error."},
    {rpcLGR_IDXS_INVALID,      "lgrIdxsInvalid",      "Ledger indexes invalid."},
    {rpcLGR_IDX_MALFORMED,     "lgrIdxMalformed",     "Ledger index malformed."},
    {rpcLGR_NOT_FOUND,         "lgrNotFound",         "Ledger not found."},
    {rpcLGR_NOT_VALIDATED,     "lgrNotValidated",     "Ledger not validated."},
    {rpcMASTER_DISABLED,       "masterDisabled",      "Master key is disabled."},
    {rpcNOT_ENABLED,           "notEnabled",          "Not enabled in configuration."},
    {rpcNOT_IMPL,              "notImpl",             "Not implemented."},
    {rpcNOT_READY,             "notReady",            "Not ready to handle this request."},
    {rpcNOT_SUPPORTED,         "notSupported",        "Operation not supported."},
    {rpcNO_CLOSED,             "noClosed",            "Closed ledger is unavailable."},
    {rpcNO_CURRENT,            "noCurrent",           "Current ledger is unavailable."},
    {rpcNO_EVENTS,             "noEvents",            "Current transport does not support events."},
    {rpcNO_NETWORK,            "noNetwork",           "Not synced to Ripple network."},
    {rpcNO_PERMISSION,         "noPermission",        "You don't have permission for this command."},
    {rpcNO_PF_REQUEST,         "noPathRequest",       "No pathfinding request in progress."},
    {rpcPUBLIC_MALFORMED,      "publicMalformed",     "Public key is malformed."},
    {rpcSIGNING_MALFORMED,     "signingMalformed",    "Signing of transaction is malformed."},
    {rpcSLOW_DOWN,             "slowDown",            "You are placing too much load on the server."},
    {rpcSRC_ACT_MALFORMED,     "srcActMalformed",     "Source account is malformed."},
    {rpcSRC_ACT_MISSING,       "srcActMissing",       "Source account not provided."},
    {rpcSRC_ACT_NOT_FOUND,     "srcActNotFound",      "Source account not found."},
    {rpcSRC_CUR_MALFORMED,     "srcCurMalformed",     "Source currency is malformed."},
    {rpcSRC_ISR_MALFORMED,     "srcIsrMalformed",     "Source issuer is malformed."},
    {rpcSTREAM_MALFORMED,      "malformedStream",     "Stream malformed."},
    {rpcTOO_BUSY,              "tooBusy",             "The server is too busy to help you now."},
    {rpcTXN_NOT_FOUND,         "txnNotFound",         "Transaction not found."},
    {rpcUNKNOWN_COMMAND,       "unknownCmd",          "Unknown method."},
    {rpcSENDMAX_MALFORMED,     "sendMaxMalformed",    "SendMax amount malformed."},
};
template <int N>
struct ErrorInfoArray
{
    constexpr ErrorInfoArray()
    : infos {}
    { }
    ErrorInfo infos[N];
};
template<int M, int N>
constexpr auto
sortErrorInfos (ErrorInfo const (&unordered)[N]) -> ErrorInfoArray<M>
{
    ErrorInfoArray<M> ret;
    for (ErrorInfo const& info : unordered)
    {
        if (info.code <= rpcSUCCESS || info.code > rpcLAST)
            throw (std::out_of_range ("Invalid error_code_i"));
        static_assert (rpcSUCCESS == 0, "Unexpected error_code_i layout.");
        int const index {info.code - 1};
        if (ret.infos[index].code != rpcUNKNOWN)
            throw (std::invalid_argument ("Duplicate error_code_i in list"));
        ret.infos[index].code    = info.code;
        ret.infos[index].token   = info.token;
        ret.infos[index].message = info.message;
    }
    int codeCount {0};
    int expect {rpcBAD_SYNTAX - 1};
    for (ErrorInfo const& info : ret.infos)
    {
        ++expect;
        if (info.code == rpcUNKNOWN)
            continue;
        if (info.code != expect)
            throw (std::invalid_argument ("Empty error_code_i in list"));
        ++codeCount;
    }
    if (expect != rpcLAST)
        throw (std::invalid_argument ("Insufficient list entries"));
    if (codeCount != N)
        throw (std::invalid_argument ("Bad handling of unorderedErrorInfos"));
    return ret;
}
constexpr auto sortedErrorInfos {sortErrorInfos<rpcLAST>(unorderedErrorInfos)};
constexpr ErrorInfo unknownError;
} 
ErrorInfo const& get_error_info (error_code_i code)
{
    if (code <= rpcSUCCESS || code > rpcLAST)
        return detail::unknownError;
    return detail::sortedErrorInfos.infos[code - 1];
}
Json::Value make_error (error_code_i code)
{
    Json::Value json;
    inject_error (code, json);
    return json;
}
Json::Value make_error (error_code_i code, std::string const& message)
{
    Json::Value json;
    inject_error (code, message, json);
    return json;
}
bool contains_error (Json::Value const& json)
{
    if (json.isObject() && json.isMember (jss::error))
        return true;
    return false;
}
} 
std::string rpcErrorString(Json::Value const& jv)
{
    assert(RPC::contains_error(jv));
    return jv[jss::error].asString() +
        jv[jss::error_message].asString();
}
} 
