// CPersistTraits<CHeroMarriageInfo>::TransferOut(CPersistContext& context, CCharString& out, const CHeroMarriageInfo* value) const -- text persist write of a marriage record as "(wifeUID,homeUID,state,TRUE|FALSE)"
// Leaf: ?TransferOut@?$CPersistTraits@UCHeroMarriageInfo@@@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057F78F, 178 bytes, thiscall `ret 0Ch` (the traits `this` is unused). Called by
//   the CHeroMarriageInfo vector's text transfer (0x0057F770 passes element i = begin + 18h*i and a 0x18-byte traits
//   local); the binary twin is the landed TransferBinaryOut 0x0057A3CD. Label correction: the catalog
//   "Inventory_FormatItemTransferDetails" is a guess (0x009E2560 is TransferOutLong, not GetItemDescription).
// Callees: CCharString::operator+=(const char*) @ 0x0099F100 (literals "(" 0x0122E028, "," 0x0122E024, ")" 0x0122E020,
//   "TRUE" 0x0122D6D4, "FALSE" 0x0122D6CC), CCharString(unsigned __int64) @ 0x0099F930 + operator+=(const CCharString&)
//   @ 0x0099F0A0 + ~CCharString @ 0x0099EAE0 (each UID), TransferOutLong @ 0x009E2560 (`__fastcall` context, out,
//   &value: the marriage state).
// Shape: the landed pair<long,long> twin (0x00755A30) generalised: each member goes through its own element traits'
//   inlined TransferOut (`__forceinline`: the size TU leaves plain inline members out of line) -- the UID copied to
//   a 64-bit local first (`mov eax,[esi] ; push eax` for the low word; `CCharString(*value)` pushes both words from
//   memory), the temporary built in the dead `out` argument slot, the state copied into that slot for
//   TransferOutLong, the bool as `if (*value) out += "TRUE"; else out += "FALSE";` (one tail-merged call; pairing
//   checked against the object relocations: TRUE on the fall-through, FALSE on the `je`) -- with the "," literal
//   CSE'd into edi. Frame-pointer size TU.
// Layout: engine/CHeroMarriageInfo.h (WifeUID +0, MaritalHomeUID +8, State +0x10, GayMarriage +0x14).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CHeroMarriageInfo.h"

struct CPersistContext;
class CCharString {
public:
    struct SBlock* m_pBlock;
    explicit CCharString(unsigned __int64 value);                    // 0x0099F930
    ~CCharString();                                                  // 0x0099EAE0
    CCharString& operator+=(const char* text);                       // 0x0099F100
    CCharString& operator+=(const CCharString& other);               // 0x0099F0A0
};
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);   // 0x009E2560
extern const char g_textOpenBracket_0122e028[];                       // 0x0122E028 "("
extern const char g_textComma_0122e024[];                             // 0x0122E024 ","
extern const char g_textCloseBracket_0122e020[];                      // 0x0122E020 ")"
extern const char g_textTrue_0122d6d4[];                              // 0x0122D6D4 "TRUE"
extern const char g_textFalse_0122d6cc[];                             // 0x0122D6CC "FALSE"

template <class T> struct CPersistTraits;

template <> struct CPersistTraits<unsigned __int64> {
    __forceinline void TransferOut(CPersistContext& context, CCharString& out, const unsigned __int64* value) const
    {
        unsigned __int64 copy = *value;
        out += CCharString(copy);
    }
};
template <> struct CPersistTraits<long> {
    __forceinline void TransferOut(CPersistContext& context, CCharString& out, const long* value) const
    {
        long copy = *value;
        TransferOutLong(&context, &out, &copy);
    }
};
template <> struct CPersistTraits<bool> {
    __forceinline void TransferOut(CPersistContext& context, CCharString& out, const bool* value) const
    {
        if (*value)
            out += g_textTrue_0122d6d4;
        else
            out += g_textFalse_0122d6cc;
    }
};

template <> struct CPersistTraits<CHeroMarriageInfo> {
    CHeroMarriageInfo DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, const CHeroMarriageInfo* value) const;
};

void CPersistTraits<CHeroMarriageInfo>::TransferOut(CPersistContext& context, CCharString& out,
                                                    const CHeroMarriageInfo* value) const
{
    CPersistTraits<unsigned __int64> uidTraits;
    CPersistTraits<long> stateTraits;
    CPersistTraits<bool> flagTraits;
    out += g_textOpenBracket_0122e028;
    uidTraits.TransferOut(context, out, &value->WifeUID);
    out += g_textComma_0122e024;
    uidTraits.TransferOut(context, out, &value->MaritalHomeUID);
    out += g_textComma_0122e024;
    stateTraits.TransferOut(context, out, (const long*)value->State);
    out += g_textComma_0122e024;
    flagTraits.TransferOut(context, out, &value->GayMarriage);
    out += g_textCloseBracket_0122e020;
}