// CXMVPlayer::GetCharWidthW(unsigned short c) -- word-wrap callback: the width of one character in the movie player's font, in whole virtual units
// Leaf: ?GetCharWidthW@CXMVPlayer@@SIIG@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BCE70, 120 bytes, public static __fastcall (the character in cx), plain `ret`; no caller
//   in retail (the Xbox movie path that hands it to the word-wrapper is not built).
// Identity: Ego_d.pdb lib_xmv_player.obj CXMVPlayer::GetCharWidthW (locals c, char_text, unsigned int res), the first of the two
//   GetCharWidthW of that object (CMovieSubtitles' follows at 0x009BCEF0); Ego_r.pdb ?GetCharWidthW@CXMVPlayer@@SII_W@Z at 0x00B22240
//   before CMovieSubtitles' at 0x00B222E0 (retail VC7.1: the wide char is unsigned short, mangled G). The catalog label "_global::GetCharWidthW" is unscoped.
// Callees: CWideString() 0x0099AED0, CWideString::Sprintf 0x0099BA70 (L"%c" 0x01238154), CWideString::GetAsCharArray
//   0x0099AF10, the font's slot 1 GetStringExtents(text, false), GFAXToVX 0x009E1DA0, GFAYToVY 0x009E1DD0, ~CWideString
//   0x0099B510, __ftol2 0x00BFEA70. Global: CXMVPlayer::PlayerFont 0x013CA7F0 (Ego_d.pdb private static CCountedPointer<CFontBank>;
//   read only here and by its atexit destructor 0x0122A6C0 (initializer 0x01224070)).
// Shape: the shared inline CFontBank::GetVStringExtents (X converted in place, the Y conversion's result dead); Ego_d's `res` takes
//   the implicitly converted width and VC7.1 still truncates after ~CWideString (retail 0x009BCED4 dtor, 0x009BCED9 fld,
//   0x009BCEDD __ftol2), byte-identical to CProgressDisplay::GetCharWidthW 0x00498410 and CMovieSubtitles::GetCharWidthW 0x009BCEF0.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CXMVPLAYER_GETCHARWIDTHW
#include "fable_string.h"
#include "engine/C2DExtentsF.h"
#include "engine/CFontBank.h"
#include "engine/CXMVPlayer.h"

unsigned int __fastcall CXMVPlayer::GetCharWidthW(unsigned short c)
{
    CWideString char_text;
    char_text.Sprintf(L"%c", c);
    unsigned int res = PlayerFont->GetVStringExtents(char_text).X;
    return res;
}
