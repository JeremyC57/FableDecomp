// GFFPUSetRoundingNearest() -- set the x87 rounding control (RC, control-word bits 10-11) to round to nearest, leaving every
// other control-word bit (precision control, exception masks) as it was: the engine's inline-assembly control-word switch,
// expanded in place by its callers.
// Identity: FableWin.pdb ?GFFPUSetRoundingNearest@@YIXXZ (__fastcall void(void), out of line only in the debug build,
//   0x018A63C0, 31 bytes), between its siblings ?GFFPUSetDoublePrecision@@YIXXZ (0x018A6390) and ?GFFPUClear@@YIXXZ.
// Retail-asm-helper: GFFPUSetRoundingNearest
// Retail-asm: 0x0042DFD8 `fnstcw word ptr [ebp-4]` + 0x0042DFDB `and byte ptr [ebp-3],0F3h` + 0x0042DFDF `fldcw word ptr [ebp-4]`
//   (CNewFrontendGameComponent::Draw), and the same three instructions at 0x004177AE and 0x004182B2 (each right after a
//   GFFPUSetDoublePrecision block `fnstcw ; mov eax,[cw] ; and eax,0FFFFFEFFh ; or eax,200h ; mov [cw],eax ; fldcw`). VC7.1
//   (RTM 3077 and QFE 4035) emits neither fnstcw nor fldcw from C++: _controlfp / _control87 / _fpreset are CRT calls,
//   <fenv.h> does not exist, and no compiler intrinsic touches the x87 control word (the SSE _mm_setcsr / _MM_SET_ROUNDING_MODE
//   give stmxcsr / ldmxcsr); 44 probes in chunk 774 (NOTES.md, ATTEMPTS fpu table). So the original is inline asm around a
//   C++ mask of the stored word: the `& ~0xC00` on the 32-bit local is narrowed by VC7.1 to the one-byte `and [cw+1],0F3h`.
// Semantics: RC := 00 (round to nearest, ties to even); PC, the exception masks and the rest of the word are unchanged.
// Portable twin (FABLE_PORTABLE): plain C++ through the C runtime -- _controlfp(_RC_NEAR, _MCW_RC) under MSVC (the same RC field;
//   on x86 it also sets the SSE rounding field), fesetround(FE_TONEAREST) elsewhere (the C99 <fenv.h> rounding mode, which on
//   x86 / x86-64 glibc sets both the x87 RC and the SSE MXCSR.RC). Tests in rebuild/tests/portable/test_GFFPUSetRoundingNearest.cpp.
#ifndef FABLE_ENGINE_GFFPUSETROUNDINGNEAREST_H
#define FABLE_ENGINE_GFFPUSETROUNDINGNEAREST_H
#include "fable_compiler.h"

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
#if defined(_MSC_VER)
#include <float.h>
#else
#include <fenv.h>
#endif
namespace fable_portable
{
    inline void GFFPUSetRoundingNearest()
    {
#if defined(_MSC_VER)
        _controlfp(_RC_NEAR, _MCW_RC);
#else
        fesetround(FE_TONEAREST);
#endif
    }
}
#endif

#ifdef FABLE_PORTABLE
using fable_portable::GFFPUSetRoundingNearest;
#else
FABLE_FORCEINLINE void __fastcall GFFPUSetRoundingNearest()
{
    unsigned long control_word;
    __asm fnstcw word ptr control_word
    control_word &= ~0x0C00UL;                                           // RC = 00: round to nearest
    __asm fldcw word ptr control_word
}
#endif

#endif  // FABLE_ENGINE_GFFPUSETROUNDINGNEAREST_H
