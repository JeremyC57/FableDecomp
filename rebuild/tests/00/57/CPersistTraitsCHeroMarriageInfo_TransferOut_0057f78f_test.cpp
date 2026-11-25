// Behaviour test for 0x0057F78F CPersistTraits<CHeroMarriageInfo>::TransferOut: writes "(wife,home,state,TRUE|FALSE)" -- the
// separators "(" "," ")" (retail 0x0122E028 / 0x0122E024 / 0x0122E020) and "TRUE" / "FALSE" (0x0122D6D4 / 0x0122D6CC).
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); the callees are defined
// under the row's own spellings and append to one text buffer, so the test asserts the exact text the row writes; a mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
// ---- row declarations (copied from the row source; its function definition left out) ----
// CPersistTraits<CHeroMarriageInfo>::TransferOut(CPersistContext& context, CCharString& out, const CHeroMarriageInfo* value) const -- text persist write of a marriage record as "(wifeUID,homeUID,state,TRUE|FALSE)"
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057F78F, 178 bytes, thiscall `ret 0Ch` (the traits `this` is unused). Called by
//   the CHeroMarriageInfo vector's text transfer (0x0057F770 passes element i = begin + 18h*i and a 0x18-byte traits
//   local); the binary twin is the landed TransferBinaryOut 0x0057A3CD. Label correction: the catalog
//   "Inventory_FormatItemTransferDetails" is a guess (0x009E2560 is TransferOutLong, not GetItemDescription).
// Callees: CCharString::operator+=(const char*) @ 0x0099F100 (literals "(" 0x0122E028, "," 0x0122E024, ")" 0x0122E020,
//   "TRUE" 0x0122D6D4, "FALSE" 0x0122D6CC), GFUHUGEToCharString(unsigned __int64) @ 0x0099F930 + operator+=(const CCharString&)
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
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CHeroMarriageInfo.h"

class CPersistContext;
class CCharString {
public:
    struct SBlock* m_pBlock;
    ~CCharString();                                                  // 0x0099EAE0
    const CCharString& operator+=(const char* text);                       // 0x0099F100
    const CCharString& operator+=(const CCharString& other);               // 0x0099F0A0
};
// Naming (NF183, 2026-11-20, q4628): 0x0099F930 is the free __fastcall GFUHUGEToCharString (Ego_r.pdb + FableWin.pdb
//   ?GFUHUGEToCharString@@YI?AVCCharString@@_K@Z, the spelling the landed 0x004FB990 / 0x004B5EE0 / 0x008304A0 bind), was a stand-in
//   CCharString(unsigned __int64) constructor (same registers: hidden result in ecx, the value on the stack).
CCharString __fastcall GFUHUGEToCharString(unsigned __int64 value);   // 0x0099F930
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);   // 0x009E2560

template <class T> struct CPersistTraits;

template <> struct CPersistTraits<unsigned __int64> {
    FABLE_FORCEINLINE void TransferOut(CPersistContext& context, CCharString& out, const unsigned __int64* value) const
    {
        unsigned __int64 copy = *value;
        out += GFUHUGEToCharString(copy);
    }
};
template <> struct CPersistTraits<long> {
    FABLE_FORCEINLINE void TransferOut(CPersistContext& context, CCharString& out, const long* value) const
    {
        long copy = *value;
        TransferOutLong(&context, &out, &copy);
    }
};
template <> struct CPersistTraits<bool> {
    FABLE_FORCEINLINE void TransferOut(CPersistContext& context, CCharString& out, const bool* value) const
    {
        if (*value)
            out += "TRUE";
        else
            out += "FALSE";
    }
};

template <> struct CPersistTraits<CHeroMarriageInfo> {
    CHeroMarriageInfo DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, const CHeroMarriageInfo* value) const;
};

// ---- end of row declarations ----
static char g_out[256]; static char g_tmp[64]; static int g_ctxOk = 1; static CPersistContext* g_ctx = 0;
CCharString::~CCharString() { }
const CCharString& CCharString::operator+=(const char* text) { strcat((char*)m_pBlock, text); return *this; }
const CCharString& CCharString::operator+=(const CCharString& other) { strcat((char*)m_pBlock, (const char*)other.m_pBlock); return *this; }
CCharString __fastcall GFUHUGEToCharString(unsigned __int64 value) { CCharString r; sprintf(g_tmp, "%I64u", value); r.m_pBlock = (SBlock*)g_tmp; return r; }
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value)
{ char b[16]; if (context != g_ctx) g_ctxOk = 0; sprintf(b, "%ld", *value); strcat((char*)out->m_pBlock, b); }
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    static double ctxRaw[4]; CPersistContext& ctx = *(CPersistContext*)ctxRaw; g_ctx = &ctx;
    CPersistTraits<CHeroMarriageInfo> traits; memset(&traits, 0, sizeof(traits));
    CHeroMarriageInfo info; memset(&info, 0, sizeof(info));
    info.WifeUID = 12345678901ull; info.MaritalHomeUID = 7; *(long*)info.State = -3; info.GayMarriage = true;
    CCharString out; out.m_pBlock = (SBlock*)g_out;
    traits.TransferOut(ctx, out, &info);
    bool ok = strcmp(g_out, "(12345678901,7,-3,TRUE)") == 0;
    info.GayMarriage = false; g_out[0] = 0;
    traits.TransferOut(ctx, out, &info);
    ok = ok && strcmp(g_out, "(12345678901,7,-3,FALSE)") == 0 && g_ctxOk;
    if (ok) printf("0057f78f_TEST PASS\n");
    else printf("FAIL out=%s\n", g_out);
    return ok ? 0 : 1;
}
