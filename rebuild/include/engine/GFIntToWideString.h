#if defined(FABLE_RETAIL_PASS22_CREATURE_GENERATOR_DEBUG)
#ifndef FABLE_ENGINE_GFINTTOWIDESTRING_H
#define FABLE_ENGINE_GFINTTOWIDESTRING_H
#include "fable_string.h"
// Original Ego_r/FableWin GFIntToWideString(long), expanded in native0074BCA3..DE and0074BCEE..BD25.
// VC71 has no named-return optimisation: default, format, copy result, destroy local.
inline CWideString FABLE_FASTCALL GFIntToWideString(long value) {
    CWideString text;
    text.Sprintf(L"%d",value);
    return text;
}
#endif
#endif
