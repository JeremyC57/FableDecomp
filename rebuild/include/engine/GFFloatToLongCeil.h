// GFFloatToLongCeil(float) -- ceil(value) as a long: the engine's inline-assembly ceiling, expanded in place by its
// callers (the out-of-line twin of the block is CTCBulletTime::GetBulletTimeTicksFromAbilityLevel 0x004FA9A0).
// Identity: FableWin.pdb ?GFFloatToLongCeil@@YIJM@Z (__fastcall; debug body 58 bytes, the same size as the retail
//   floor copy 0x0048D940). Sibling of GFFloatToLongFloor (engine/GFFloatToLongFloor.h) and GFFloatToLongNear.
// Retail-asm-helper: GFFloatToLongCeil
// Retail-asm: 0x004FA9B4..0x004FA9CE `fld [ebp+8] ; fadd [ebp-0Ch] ; fistp dword ptr [ebp-4] ; fild [ebp-4] ; fsub [ebp-8] ;
//   fstp [ebp-10h] ; mov eax,[ebp+8] ; cmp eax,[ebp-10h] ; jne ; dec dword ptr [ebp-4]` (0x004FA9A0), inlined at 0x0050EF30
//   (CThingFilter_OverlappingArea::operator()), 0x00516200 (CThingSearchTools::GetAllThings), 0x0079C5F0 / 0x0079C680 /
//   0x0079C6D0 (CTCBulletTime), 0x007726B0 (CTCShop::GetStockQuantity) and 20 more: 0.5f / 1.0f stored as frame
//   constants, a bare dword fistp into a named long, and the fix-up on memory. VC7.1 emits no bare dword fistp from C++
//   (`(long)x` = __ftol2, /QIfist = `fistp qword`, /arch:SSE2 = cvttss2si) and would hoist the `dec` of the memory long
//   into a register, so the ten-instruction block is the original's inline asm (rasm2, GOTCHAS 2026-10-27).
// Semantics: result = fistp(value + 0.5) under RC = round to nearest, ties to even (the engine never changes RC; this is
//   NOT an RC = up switch); then, when the float (result - 1) has the same 32-bit pattern as value (value was an integer
//   and the tie rounded up to even), result - 1. The compare is integer (`mov eax ; cmp eax`): -0.0f is not 0.0f, and
//   a NaN never matches. The sum with 0.5 is rounded at the x87 precision control (53 bits under the CRT default: a
//   positive value below 2^-54 rounds the sum to 0.5 and the result to 0; see fable_x87_twins.h for the Direct3D
//   single-precision note); NaN / out of range give 0x80000000.
// Portable twin (FABLE_PORTABLE): Strategy B, plain C++ with the same steps; tests in
//   rebuild/tests/portable/test_GFFloatToLongCeil.cpp.
#ifndef FABLE_ENGINE_GFFLOATTOLONGCEIL_H
#define FABLE_ENGINE_GFFLOATTOLONGCEIL_H
#include "fable_compiler.h"
#include "fable_x87_twins.h"

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline long GFFloatToLongCeil(float value)
    {
        long result = X87FistpNear((double)value + 0.5);             // fld value ; fadd half ; fistp result
        float back = (float)((double)result - 1.0);                 // fild result ; fsub one ; fstp back
        if (FloatBits(value) == FloatBits(back))                    // mov eax, value ; cmp eax, back ; jne done
            result = (long)(int)((unsigned int)result - 1u);       // dec result (a 32-bit wrap at -2^31, as `dec dword`)
        return result;
    }
}
#endif

#ifdef FABLE_PORTABLE
using fable_portable::GFFloatToLongCeil;
#else
FABLE_FORCEINLINE long __fastcall GFFloatToLongCeil(float value)
{
    long  result;
    float one;
    float half;
    float back;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fadd  half
        fistp result
        fild  result
        fsub  one
        fstp  back
        mov   eax, value
        cmp   eax, back
        jne   done
        dec   result
    done:
    }
    return result;
}
#endif

#endif  // FABLE_ENGINE_GFFLOATTOLONGCEIL_H
