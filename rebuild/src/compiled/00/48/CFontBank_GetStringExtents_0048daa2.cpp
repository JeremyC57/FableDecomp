#pragma optimize("s", on)
// CFontBank::GetStringExtents(const CWideString& text, bool) const -- slot 1 GetStringExtents on the string's GetAsCharArray (0x0099AF10), the
// hidden result handed through
// Leaf: ?GetStringExtents@CFontBank@@QBE?AVC2DExtentsF@@ABVCWideString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048DAA2.
// NF39 (2026-11-16): moved off its CGuiControlListBox::SelectEntryFromName stand-in (a propagated label) onto the shared engine/CFontBank.h
//   (opt-in FABLE_RETAIL_CFONTBANK_STRING_HELPERS, which names 0x0048DAA2): FableWin.pdb ?GetStringExtents@CFontBank@@QBE?AVC2DExtentsF@@ABVCWideString@@_N@Z
//   (the hand784 NAME_FIXES entry); CWideString::GetAsCharArray 0x0099AF10. Defines from the landed 0x00492381 (the same opt-in).
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

C2DExtentsF CFontBank::GetStringExtents(const CWideString& text, bool b) const
{
    return GetStringExtents(text.GetAsCharArray(), b);
}
