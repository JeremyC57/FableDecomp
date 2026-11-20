#pragma optimize("s",on)
// CFontBank::GetStringWidth(const CWideString& text) const -- the X of slot 1 GetStringExtents(text.GetAsCharArray(), false) (the hidden result in a frame temporary)
// Leaf: ?GetStringWidth@CFontBank@@QBEMABVCWideString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048DA62, length 39 bytes: `push 0` (the bool), GetAsCharArray 0x0099AF10 on the string,
//   slot 1 (+4) with the hidden 8-byte result, `fld [eax]`, `leave`, `ret 4`.
// Naming (NF198, 2026-11-20, q11974): moved off its local CFontBank / CCharString stand-ins (non-const, `const CCharString&`, a one-argument
//   slot 1 with the bool modelled as GetCharacters(0)) onto the shared engine/CFontBank.h (opt-in FABLE_RETAIL_CFONTBANK_STRING_HELPERS, which
//   names 0x0048DA62): Ego_r.pdb 0x00496B2A + FableWin.pdb 0x01A6BC18 ?GetStringWidth@CFontBank@@QBEMABVCWideString@@@Z. All six landed REFS
//   rows bind this spelling (HEAD compile, 0x00645CE9 respelled in this hand-back). Same TU and defines as the landed twins 0x0048DA89
//   (GetStringWidth(const wchar_t*)) and 0x0048DAA2 (GetStringExtents(const CWideString&, bool)).
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_STRING_HELPERS
#define FABLE_RETAIL_CFONTBANK_STRINGWIDTH_TEXT
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "fable_string.h"
#include "engine/CFontBank.h"
float CFontBank::GetStringWidth(const CWideString& text) const
{
    return GetStringExtents(text.GetAsCharArray(), false).X;
}
