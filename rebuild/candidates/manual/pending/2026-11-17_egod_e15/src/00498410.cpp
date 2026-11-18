// CProgressDisplay::GetCharWidthW(unsigned short c) -- word-wrap callback: the width of one character in the progress-display font, in whole virtual units
// Leaf: ?GetCharWidthW@CProgressDisplay@@CIIG@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00498410, 120 bytes, private static __fastcall (the character in cx), plain `ret`.
// Identity: Ego_d.pdb progress_debug_display.obj CProgressDisplay::GetCharWidthW (locals c, char_text only); Ego_r.pdb /
//   FableWin.pdb ?GetCharWidthW@CProgressDisplay@@CII_W@Z (retail VC7.1: the wide char is unsigned short, mangled G); its address is taken by
//   DrawRetailDisplay 0x00498490 (0x00499719). The catalog label "_global::GetCharWidthW" is unscoped.
// Callees: CWideString() 0x0099AED0, CWideString::Sprintf 0x0099BA70 (L"%c" 0x01238154), CWideString::GetAsCharArray
//   0x0099AF10, the font's slot 1 GetStringExtents(text, false), GFAXToVX 0x009E1DA0, GFAYToVY 0x009E1DD0, ~CWideString
//   0x0099B510, __ftol2 0x00BFEA70. Global: CProgressDisplay::PFont 0x013B8998 (CCountedPointer<CFontBank>, Data read inline).
// Shape: the shared inline CFontBank::GetVStringExtents (X converted in place, the Y conversion's result dead) and an implicit
//   float -> unsigned int return conversion, which VC7.1 performs after ~CWideString (retail 0x00498474 dtor, 0x00498479 fld,
//   0x0049847D __ftol2). Ego_d lists no width / extents local, so the source wrote one expression.
// EVIDENCED: one expression, no width local (Ego_d lists c, char_text only). The implicit float -> unsigned int return conversion is a
//   free plain spelling: the explicit `(unsigned int)` cast and a `res` local grade the same (RELOCATION_MATCH, audit 2026-11-18).
// Header: FABLE_RETAIL_CPROGRESSDISPLAY_GETCHARWIDTHW_U16 declares the wide char as unsigned short (the default header line says wchar_t,
//   which only matches this definition where wchar_t is unsigned short, i.e. VC7.1).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CPROGRESSDISPLAY_GETCHARWIDTHW_U16
#include "fable_string.h"
#include "engine/C2DExtentsF.h"
#include "engine/CFontBank.h"
#include "engine/CProgressDisplay.h"

unsigned int __fastcall CProgressDisplay::GetCharWidthW(unsigned short c)
{
    CWideString char_text;
    char_text.Sprintf(L"%c", c);
    return PFont->GetVStringExtents(char_text).X;
}
