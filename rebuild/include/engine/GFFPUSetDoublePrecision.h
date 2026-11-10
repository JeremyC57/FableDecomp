// GFFPUSetDoublePrecision() -- set the x87 precision control (PC, control-word bits 8-9) to 53-bit double precision, leaving
// every other control-word bit (rounding control, exception masks) as it was: the engine's inline-assembly control-word
// switch, expanded in place by its callers (CMainGameComponent::Update / UpdateForcedTickSpeed run it once per frame, right
// before GFFPUSetRoundingNearest, because Direct3D drops the FPU to single precision).
// Identity: FableWin.pdb ?GFFPUSetDoublePrecision@@YIXXZ (__fastcall void(void), out of line only in the debug build,
//   0x018A6390, 48 bytes), between its siblings ?GFFPUClear@@YIXXZ (0x018A6380) and ?GFFPUSetRoundingNearest@@YIXXZ
//   (0x018A63C0, engine/GFFPUSetRoundingNearest.h).
// Retail-asm-helper: GFFPUSetDoublePrecision
// Retail-asm: 0x0041829C `fnstcw word ptr [ebp-34h]` + 0x0041829F `mov eax,[ebp-34h]` + 0x004182A2 `and eax,0FFFFFEFFh` +
//   0x004182A7 `or eax,200h` + 0x004182AC `mov [ebp-34h],eax` + 0x004182AF `fldcw word ptr [ebp-34h]` (CMainGameComponent::Update),
//   and the same six instructions at 0x00417798 (CMainGameComponent::UpdateForcedTickSpeed, word [ebp-4]). VC7.1 emits neither
//   fnstcw nor fldcw from C++ (engine/GFFPUSetRoundingNearest.h: 44 probes in chunk 774), so the original is inline asm around a C++
//   update of the stored word: `& ~0x100` then `| 0x200` (PC = 10b), done in eax on the 32-bit local.
// Semantics: PC := 10b (53-bit mantissa); RC, the exception masks and the rest of the word are unchanged.
// Portable twin (FABLE_PORTABLE): _controlfp(_PC_53, _MCW_PC) under MSVC (the same PC field); with glibc on 32-bit x86 the
//   <fpu_control.h> word (_FPU_GETCW / _FPU_SETCW, PC = _FPU_DOUBLE); elsewhere (SSE arithmetic, x86-64, other CPUs) floating
//   point already rounds float/double operations at their own precision, so the twin does nothing.
#ifndef FABLE_ENGINE_GFFPUSETDOUBLEPRECISION_H
#define FABLE_ENGINE_GFFPUSETDOUBLEPRECISION_H
#include "fable_compiler.h"

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
#if defined(_MSC_VER)
#include <float.h>
#elif defined(__GLIBC__) && defined(__i386__)
#include <fpu_control.h>
#endif
namespace fable_portable
{
    inline void GFFPUSetDoublePrecision()
    {
#if defined(_MSC_VER)
        _controlfp(_PC_53, _MCW_PC);
#elif defined(__GLIBC__) && defined(__i386__)
        fpu_control_t control_word;
        _FPU_GETCW(control_word);
        control_word = (fpu_control_t)((control_word & ~0x0300) | _FPU_DOUBLE);
        _FPU_SETCW(control_word);
#endif
    }
}
#endif

#ifdef FABLE_PORTABLE
using fable_portable::GFFPUSetDoublePrecision;
#else
FABLE_FORCEINLINE void __fastcall GFFPUSetDoublePrecision()
{
    unsigned long control_word;
    __asm fnstcw word ptr control_word
    control_word = (control_word & ~0x0100UL) | 0x0200UL;              // PC = 10b: 53-bit double precision
    __asm fldcw word ptr control_word
}
#endif

#endif  // FABLE_ENGINE_GFFPUSETDOUBLEPRECISION_H
