// CHeroMarriageInfo::CHeroMarriageInfo(unsigned __int64, unsigned __int64, EMarriageState, bool) -- a marriage record: wife, marital home, state, gay-marriage flag
// Leaf: ??0CHeroMarriageInfo@@QAE@_K0W4EMarriageState@@_N@Z
// Retail: Fable.exe @ 0x0057A376, 44 bytes, `ret 18h`, returns this; FableWin.pdb ??0CHeroMarriageInfo@@QAE@_K0W4EMarriageState@@_N@Z.
// Catalog label CDateAndTime::CDateAndTime is wrong (no CDateAndTime ctor takes a trailing bool, and that class is 28 bytes):
// the only caller, CTCHeroStats::AddNewMarriage (0x0057BB8D), builds a 0x18-byte temporary from the wife's UID (+0x18/+0x1c
// of the first thing), the home's UID (or 0), the state argument and a bool, then hands it to the Marriages list (+0x1d8).
// Layout: Ego_r.pdb CHeroMarriageInfo (24 bytes): WifeUID +0, MaritalHomeUID +8, State +0x10, GayMarriage +0x14.
// Opt: size TU with frame pointers (`optimize("y", off)`: push ebp / mov ebp,esp), as the CTCHeroStats code around it.
#pragma optimize("s", on)
#pragma optimize("y", off)
enum EMarriageState { MARRIAGE_STATE_NONE };
class CHeroMarriageInfo {
public:
    unsigned __int64 WifeUID;                            // +0x00
    unsigned __int64 MaritalHomeUID;                     // +0x08
    EMarriageState   State;                              // +0x10
    bool             GayMarriage;                        // +0x14
    CHeroMarriageInfo(unsigned __int64 wife_uid, unsigned __int64 marital_home_uid, EMarriageState state, bool gay_marriage);
};
CHeroMarriageInfo::CHeroMarriageInfo(unsigned __int64 wife_uid, unsigned __int64 marital_home_uid, EMarriageState state, bool gay_marriage)
    : WifeUID(wife_uid)
    , MaritalHomeUID(marital_home_uid)
    , State(state)
    , GayMarriage(gay_marriage)
{
}