// CBasicString<char>::Compare(const char* a, const char* b) -- signed byte-wise comparison: 0 when both strings end together, -1 / 1 at the first difference
// Leaf: ?Compare@?$CBasicString@D@@KIJPBD0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00411570, 41 bytes (__fastcall: a in ecx, b in edx).
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb (+ _fablewin_api) ?Compare@?$CBasicString@D@@KIJPBD0@Z, the protected static
//   __fastcall the shared fable_string.h declares in CBasicString<T> (retail branch, FABLE_RETAIL_CCHARSTRING); defined here as the <char>
//   specialisation of the member. Was the free stand-in `long __fastcall strcmp_signed(const char*, const char*)`.
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"

template<> long __fastcall CBasicString<char>::Compare(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
