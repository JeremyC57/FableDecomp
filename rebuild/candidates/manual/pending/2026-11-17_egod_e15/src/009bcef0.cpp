// CMovieSubtitles::GetCharWidthW(unsigned short c) -- word-wrap callback: the width of one character in the subtitle font, in whole virtual units
// Leaf: ?GetCharWidthW@CMovieSubtitles@@SIIG@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BCEF0, 120 bytes, public static __fastcall (the character in cx), plain `ret`; its address
//   is taken at 0x009BD47B (ParseSubtitles 0x009BD460, the word-wrap setup).
// Identity: Ego_d.pdb lib_xmv_player.obj CMovieSubtitles::GetCharWidthW (locals c, char_text, unsigned int res), the second of the
//   two GetCharWidthW of that object (CXMVPlayer's precedes at 0x009BCE70); Ego_r.pdb ?GetCharWidthW@CMovieSubtitles@@SII_W@Z at
//   0x00B222E0 right after CXMVPlayer's 0x00B22240 (retail VC7.1: the wide char is unsigned short, mangled G); the font 0x013CA7F8 is
//   CMovieSubtitles::PFont (Ego_r.pdb `dynamic initializer for 'CMovieSubtitles::PFont''). The catalog label is unscoped.
// Callees: CWideString() 0x0099AED0, CWideString::Sprintf 0x0099BA70 (L"%c" 0x01238154), CWideString::GetAsCharArray
//   0x0099AF10, the font's slot 1 GetStringExtents(text, false), GFAXToVX 0x009E1DA0, GFAYToVY 0x009E1DD0, ~CWideString
//   0x0099B510, __ftol2 0x00BFEA70. Global: CMovieSubtitles::PFont 0x013CA7F8 (Ego_d.pdb protected static CCountedPointer<CFontBank>;
//   also read by Play 0x009BD910, landed under the descriptive extern g_pSubtitleFont).
// Shape: the shared inline CFontBank::GetVStringExtents (X converted in place, the Y conversion's result dead); Ego_d's `res` takes
//   the implicitly converted width and VC7.1 still truncates after ~CWideString (retail 0x009BCF54 dtor, 0x009BCF59 fld,
//   0x009BCF5D __ftol2), byte-identical to CProgressDisplay::GetCharWidthW 0x00498410 and CXMVPlayer::GetCharWidthW 0x009BCE70.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CMOVIESUBTITLES_GETCHARWIDTHW
#include "fable_string.h"
#include "engine/C2DExtentsF.h"
#include "engine/CFontBank.h"
#include "engine/CMovieSubtitles.h"

unsigned int __fastcall CMovieSubtitles::GetCharWidthW(unsigned short c)
{
    CWideString char_text;
    char_text.Sprintf(L"%c", c);
    unsigned int res = PFont->GetVStringExtents(char_text).X;
    return res;
}
