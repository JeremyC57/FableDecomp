// FtoL(float) -- libpk (the GameCODA audio middleware linked into Fable.exe, 0x00C08AE0..0x00C7xxxx): round a float to a
// long with the x87 rounding mode (round to nearest, ties to even, by default), a bare `fld` / `fistp dword` pair in inline
// assembly, expanded in place by libpk's callers and also emitted out of line (0x00C6B240, one copy per libpk TU).
// Identity: FableWin.pdb ?FtoL@@YAJM@Z (cdecl long(float); the editor build links the same middleware); the libpk rows
//   that expand it name it FtoL (landed 0x00C6B240) or carry the block inline (0x00C40F50 AmplitudeToMillibels,
//   0x00C42F50 CAudioObject::SetGain, 0x00C3FF70 RoundHalfDownToLong). Distinct from the engine's GFFloatToLongNear
//   (engine/GFFloatToLongNear.h): the same two instructions in a different library.
// Retail-asm-helper: FtoL
// Retail-asm: 0x00C6B241 `fld dword [esp+8]` + 0x00C6B245 `fistp dword ptr [esp]` (the out-of-line copy), 0x00C40F50
//   AmplitudeToMillibels and 0x00C3FF70 RoundHalfDownToLong: a float reloaded with `fld` and stored with a bare dword
//   `fistp`. VC7.1 `(long)x` calls _ftol2, `/QIfist` emits `fistp qword`: the original is inline asm (rasm1 evidence).
// Semantics: fistp under the control word's RC (round to nearest, ties to even; libpk never changes RC), NaN / out of
//   range store 0x80000000.
// Portable twin (FABLE_PORTABLE): Strategy B, plain C++ (fable_x87_twins.h X87FistpNear); tests in
//   rebuild/tests/portable/test_FtoL.cpp.
#ifndef FABLE_LIBPK_FTOL_H
#define FABLE_LIBPK_FTOL_H
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
using fable_portable::FtoL;
#else
__forceinline long __cdecl FtoL(float f)
{
    long result;
    __asm fld f
    __asm fistp result
    return result;
}
#endif

#endif  // FABLE_LIBPK_FTOL_H
