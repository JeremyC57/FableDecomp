#ifndef FABLE_ENGINE_GFINTTOWIDESTRING_H
#define FABLE_ENGINE_GFINTTOWIDESTRING_H
// Ego_r.pdb / FableWin.pdb ?GFIntToWideString@@YI?AVCWideString@@J@Z (global __fastcall), an inline function: its out-of-line COMDAT
// is 0x00405F00 (landed: rebuild/src/compiled/00/40/global_GFIntToWideString_00405f00.cpp; the body below is copied from it).
// Expanded in CTCCreatureGenerator::DrawDebugInfo (0x0074BCA3..DE and 0x0074BCEE..BD25; Codex pass 22, formerly behind
// FABLE_RETAIL_PASS22_CREATURE_GENERATOR_DEBUG) and twice in CDisplayEngine::DoRender (wfA2, row 0x00435530: 0x00435ACF..,
// 0x00435E4C..; that row also calls the COMDAT once, 0x00435D9D): default constructor, Sprintf(L"%d") 0x0099BA70, copy into the
// result, destructor (VC7.1 has no named-return optimisation).
#include "fable_string.h"
#ifdef FABLE_RETAIL_GFINTTOWIDESTRING_DECL_ONLY_PASS29
// The console style-card UI imports the speed-built 00405f00 instance.
CWideString FABLE_FASTCALL GFIntToWideString(long value);
#else
inline CWideString FABLE_FASTCALL GFIntToWideString(long value)
{
    CWideString text;
    text.Sprintf(L"%d",value);
    return text;
}
#endif
#endif // FABLE_ENGINE_GFINTTOWIDESTRING_H
