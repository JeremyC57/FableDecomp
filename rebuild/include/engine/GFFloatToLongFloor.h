// GFFloatToLongFloor(float) -- floor(value) as a long: the engine's inline-assembly floor, expanded in place by its
// callers and out of line at 0x0048D940.
// Identity: FableWin.pdb ?GFFloatToLongFloor@@YIJM@Z (__fastcall; debug body 58 bytes = the retail out-of-line copy
//   0x0048D940, 58 bytes). Sibling of GFFloatToLongCeil (engine/GFFloatToLongCeil.h) and GFFloatToLongNear.
// Retail-asm-helper: GFFloatToLongFloor
// Retail-asm: 0x0048D954..0x0048D971 `fld [ebp+8] ; fsub [ebp-0Ch] ; fistp dword ptr [ebp-4] ; fild [ebp-4] ; fadd [ebp-8] ;
//   fstp [ebp-10h] ; mov eax,[ebp+8] ; cmp eax,[ebp-10h] ; jne ; inc dword ptr [ebp-4]` (GFFloatToLongFloor), inlined at
//   0x00A52650 / 0x00A526D0 (ConvertTimeToFrames), 0x00A4DEA0, 0x00B46A30, 0x00B52990, 0x00646010, 0x006A68F0: 0.5f / 1.0f
//   stored as frame constants, a bare dword fistp into a named long, and the fix-up on memory. VC7.1 emits no bare dword
//   fistp from C++ (`(long)x` = __ftol2, /QIfist = `fistp qword`, /arch:SSE2 = cvttss2si) and would hoist the `inc` of the
//   memory long into a register, so the ten-instruction block is the original's inline asm (rasm2, GOTCHAS 2026-10-27).
// Semantics: result = fistp(value - 0.5) under RC = round to nearest, ties to even (the engine never changes RC; this is
//   NOT an RC = down switch); then, when the float (result + 1) has the same 32-bit pattern as value (value was an integer
//   and the tie rounded down to even), result + 1. The compare is integer (`mov eax ; cmp eax`): -0.0f is not 0.0f, and
//   a NaN never matches. The sum / difference with 0.5 is rounded at the x87 precision control (53 bits under the CRT
//   default; see fable_x87_twins.h for the Direct3D single-precision note); NaN / out of range give 0x80000000 (+1 never
//   applies there).
// Portable twin (FABLE_PORTABLE): Strategy B, plain C++ with the same steps; tests in
//   rebuild/tests/portable/test_GFFloatToLongFloor.cpp.
#ifndef FABLE_ENGINE_GFFLOATTOLONGFLOOR_H
#define FABLE_ENGINE_GFFLOATTOLONGFLOOR_H
#include "fable_compiler.h"
#include "fable_x87_twins.h"

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline long GFFloatToLongFloor(float value)
    {
        long result = X87FistpNear((double)value - 0.5);             // fld value ; fsub half ; fistp result
        float next = (float)((double)result + 1.0);                 // fild result ; fadd one ; fstp next
        if (FloatBits(value) == FloatBits(next))                    // mov eax, value ; cmp eax, next ; jne done
            result = (long)(int)((unsigned int)result + 1u);       // inc result (32-bit, as `inc dword`)
        return result;
    }
}
#endif

#ifdef FABLE_PORTABLE
using fable_portable::GFFloatToLongFloor;
#else
FABLE_FORCEINLINE long __fastcall GFFloatToLongFloor(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}
#endif

#endif  // FABLE_ENGINE_GFFLOATTOLONGFLOOR_H
