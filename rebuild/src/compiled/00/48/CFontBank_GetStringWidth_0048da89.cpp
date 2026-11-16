#pragma optimize("s",on)
// CFontBank::GetStringWidth(const unsigned short* text) const -- the X of slot 1 GetStringExtents(text, false) (the hidden result in a frame temporary)
// Leaf: ?GetStringWidth@CFontBank@@QBEMPBG@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048DA89.
// NF39 (2026-11-16): moved off its local CFontBank stand-in (non-const, `const char*` text) onto the shared engine/CFontBank.h (opt-in
//   FABLE_RETAIL_CFONTBANK_STRINGWIDTH_TEXT, which names 0x0048DA89): FableWin.pdb ?GetStringWidth@CFontBank@@QBEMPB_W@Z (const wide text, const
//   method; PBG on the lane, no /Zc:wchar_t); body slot 1 GetStringExtents(text, false).X. Defines from the landed 0x00492381 (the same opt-ins).
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

float CFontBank::GetStringWidth(const unsigned short* text) const
{
    return GetStringExtents(text, false).X;
}
