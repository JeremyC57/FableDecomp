// FtoL(float) -- libpk (the GameCODA audio middleware linked into Fable.exe, 0x00C08AE0..0x00C7xxxx): round a float to a
// long with the x87 rounding mode (round to nearest, ties to even, by default), a bare `fld` / `fistp dword` pair in inline
// assembly, expanded in place by libpk's callers and also emitted out of line (0x00C6B240, one copy per libpk TU).
// Identity: FableWin.pdb ?FtoL@@YAJM@Z (cdecl long(float); the editor build links the same middleware); the libpk rows
//   that expand it name it FtoL (landed 0x00C6B240) or carry the block inline (0x00C40F50 AmplitudeToMillibels,
//   0x00C42F50 CAudioObject::SetGain, 0x00C3FF70 RoundHalfDownToLong). Distinct from the engine's GFFloatToLongNear
//   (engine/GFFloatToLongNear.h): the same two instructions in a different library.
// Retail-asm-helper: FtoL, LIBPK_FTOL
// Retail-asm: 0x00C6B241 `fld dword [esp+8]` + 0x00C6B245 `fistp dword ptr [esp]` (the out-of-line copy), 0x00C40F50
//   AmplitudeToMillibels and 0x00C3FF70 RoundHalfDownToLong: a float reloaded with `fld` and stored with a bare dword
//   `fistp`. VC7.1 `(long)x` calls _ftol2, `/QIfist` emits `fistp qword`: the original is inline asm (rasm1 evidence).
// Semantics: fistp under the control word's RC (round to nearest, ties to even; libpk never changes RC), NaN / out of
//   range store 0x80000000.
// Macro form (2026-11-05 asm migration; call it on a line of its own, an `__asm` statement runs to the end of its line): LIBPK_FTOL(result, value) names the caller's float and long directly (no
//   parameter copy), for the out-of-line copies that define the C symbol _FtoL themselves (0x00C6B240): such a row defines
//   FABLE_LIBPK_FTOL_MACRO_ONLY before the #include, so the C++ FtoL above is not declared next to its extern "C" FtoL.
// Portable twin (FABLE_PORTABLE): Strategy B, plain C++ (fable_x87_twins.h X87FistpNear); tests in
//   rebuild/tests/portable/test_FtoL.cpp.
#ifndef FABLE_LIBPK_FTOL_H
#define FABLE_LIBPK_FTOL_H
#include "fable_compiler.h"
#include "fable_x87_twins.h"

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline long FtoL(float f)
    {
        return X87FistpNear((double)f);
    }
}
#endif

#ifdef FABLE_PORTABLE
#define LIBPK_FTOL(result, value) ((result) = fable_portable::X87FistpNear((double)(value)))
#ifndef FABLE_LIBPK_FTOL_MACRO_ONLY
using fable_portable::FtoL;
#endif
#else
#define LIBPK_FTOL(result, value) \
    __asm fld value \
    __asm fistp result
#ifndef FABLE_LIBPK_FTOL_MACRO_ONLY
FABLE_FORCEINLINE long __cdecl FtoL(float f)
{
    long result;
    __asm fld f
    __asm fistp result
    return result;
}
#endif
#endif

#endif  // FABLE_LIBPK_FTOL_H
