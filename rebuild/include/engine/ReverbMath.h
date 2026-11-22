#ifndef FABLE_ENGINE_REVERBMATH_H
#define FABLE_ENGINE_REVERBMATH_H
// GameCODA 2.10 sal/win32/envfx/ReverbMath.h (CODAApiDbg.lib CodeView source file e:\projects\gamecoda2\codaapi\sal\win32\envfx\
// ReverbMath.h): ds_ftoi, ?ds_ftoi@@YAHM@Z, int(float), a header inline (an out-of-line COMDAT in the debug member
// FloatReverbParameterTranslator.obj, expanded by the release build). Debug body: two range ASSERTs (-2^31 <= f < 2^31, debug only),
// `f -= 0.5f`, the bare `fld f ; fistp i` (libpk's inline-asm float-to-long, here through the shared libpk/FtoL.h helper, which carries
// the asm and its FABLE_PORTABLE twin), then `+1` when negative: truncation toward zero under round-to-nearest.
#include "libpk/FtoL.h"
#ifdef FABLE_RETAIL_REVERBMATH_ORIGINAL_DS_FTOI_PASS26
// Pass26 actual SDK FloatEcho/FloatPitch debug CodeView: value and result.
// Shared LIBPK_FTOL macro preserves the original inline instruction block and portable twin.
inline int ds_ftoi(float value)
{
    value-=0.5f;
    int result;
    LIBPK_FTOL(result,value);
    return result<0?result+1:result;
}
#else
inline int ds_ftoi(float f)
{
    f -= 0.5f;
    int i = FtoL(f);
    return i < 0 ? i + 1 : i;
}
#endif
#endif
