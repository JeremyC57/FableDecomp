// SalMath.h -- GameCODA 2.10 SAL math helpers (the SDK's .\SAL\Win32\SalMath.h, named by the CODAApiDbg.lib CodeView and assert strings).
// sal_ftoi(float value): round toward zero through the x87 rounding mode: value - 0.5 rounded to nearest by a bare `fld` / `fistp dword`
//   (the shared libpk/FtoL.h asm helper and its FABLE_PORTABLE twin), then a negative result is raised by one. Dbg body (CODAApiDbg.lib
//   .\Debug\Coda3DSource.obj ?sal_ftoi@@YAHM@Z, local `result`): two range asserts (compiled out of the release), `value -= 0.5f`,
//   `fld value ; fistp result` (a /Od build emitting a bare dword fistp: the original is inline asm), `result < 0 ? result + 1 : result`.
//   The release object's out-of-line COMDAT ?sal_ftoi@@YAHM@Z is byte-identical to retail 0x00C3FF70 (landed as RoundHalfDownToLong,
//   whose text this body repeats); retail expands it inline in CCoda3DSource::Commit 0x00C41FB0 (four sites), so it is forced inline:
//   VC7.1 never auto-inlines a function whose body holds inline asm (ASM_RULES rule 3.3).
// sal_round(float value): the Dbg body `fld value ; fistp result ; return result` = the shared FtoL (forced inline: it holds asm;
//   retail 0x00C40F7C keeps it expanded inside sal_gainToLevel).
// sal_gainToLevel(const float fGain): a static function kept out of line; retail's copy 0x00C40F50 is
//   byte-identical to the release COMDAT ?sal_gainToLevel@@YAHM@Z (landed as AmplitudeToMillibels). Dbg body: -10000 mB (C_LEVEL_MIN) for a
//   zero gain, else level = sal_round(log10f(fGain) * 2000.0f), clamped at -10000; the trailing SAL_ASSERT(level <= C_LEVEL_MAX) is
//   compiled out. The body must be compiled in its caller's TU: retail CCoda3DSource::Commit keeps -10000 in edx across the call
//   (0x00C424FD .. 0x00C42519), which VC7.1 does only for a callee it has compiled.
// Linkage: all three are file statics (the release object's symbols have storage class 3, IMAGE_SYM_CLASS_STATIC; the Dbg object puts
//   them in the main .text section, not in COMDATs as it does the header inlines, so the SDK did not write `inline`). Release expands
//   sal_ftoi and sal_round (asm bodies: forced); sal_gainToLevel stays a call, compiled ahead of its caller in source order, which is what
//   lets Commit keep -10000 in edx across it (an `inline` spelling is compiled after the caller and loses that: measured, row 0x00C41FB0).
#ifndef FABLE_SALMATH_H
#define FABLE_SALMATH_H
#include "fable_compiler.h"
#include "libpk/FtoL.h"
#include <math.h>   // sal_gainToLevel: the C++ inline log10f (SDK release COMDAT _log10f)

static FABLE_FORCEINLINE int sal_ftoi(float value)
{
    int result = FtoL(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}

#ifdef FABLE_RETAIL_SAL_FRAME_FLOAT_TO_INT_PASS29
// SalFrame.obj, both SDK builds: file-static int sal_round(float), called at retail 00C6B385.
#ifdef _MSC_VER
#pragma optimize("y", on)
#endif
static int sal_round(float value)
{
    int result;
    LIBPK_FTOL(result, value)
    ;
    return result;
}
#ifdef _MSC_VER
#pragma optimize("", on)
#endif
#else
static FABLE_FORCEINLINE int sal_round(float value)
{
    return FtoL(value);
}
#endif

static int sal_gainToLevel(const float fGain)
{
    int level;
    if (fGain == 0.0f)
        return -10000;
    level = sal_round(log10f(fGain) * 2000.0f);
    if (level < -10000)
        level = -10000;
    return level;
}

#endif  // FABLE_SALMATH_H
