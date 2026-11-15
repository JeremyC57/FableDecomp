#ifndef FABLE_ENGINE_GFINTTOWIDESTRING_H
#define FABLE_ENGINE_GFINTTOWIDESTRING_H
// New shared header (wfA2, row 0x00435530 CDisplayEngine::DoRender): Ego_r.pdb / FableWin.pdb ?GFIntToWideString@@YI?AVCWideString@@J@Z
// (global __fastcall), an inline function: its out-of-line COMDAT is 0x00405F00 (landed: rebuild/src/compiled/00/40/
// global_GFIntToWideString_00405f00.cpp; the body below is copied from it). DoRender expands it twice (0x00435ACF.., 0x00435E4C..:
// default constructor, Sprintf(L"%d") 0x0099BA70, copy into the result, destructor) and calls the COMDAT once (0x00435D9D).
#include "fable_string.h"
inline CWideString FABLE_FASTCALL GFIntToWideString(long value)
{
    CWideString text;
    text.Sprintf(L"%d", value);
    return text;
}
#endif // FABLE_ENGINE_GFINTTOWIDESTRING_H
