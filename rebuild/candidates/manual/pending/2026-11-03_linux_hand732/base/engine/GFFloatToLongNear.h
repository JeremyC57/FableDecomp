// GFFloatToLongNear(float) -- round a float to a long with the x87 rounding mode (round to nearest, ties to even, by
// default): the engine's two-instruction inline-assembly float-to-long, expanded in place by every caller.
// Identity: FableWin.pdb ?GFFloatToLongNear@@YIJM@Z (__fastcall, out of line only in the debug build: 19 bytes / 9
//   instructions = `push ebp ; mov ebp,esp ; push ecx ; fld [ebp+8] ; fistp [ebp-4] ; mov eax,[ebp-4] ; mov esp,ebp ;
//   pop ebp ; ret 4`, the exact debug frame around this block; the same arithmetic gives GFROR13's 18 / 9). Its siblings
//   in the PDB: GFFloatToLongFloor / Ceil (58-byte asm blocks, engine/GFFloatToLongFloor.h / Ceil.h), GFFastFloatToLong
//   (29 / 12: the pointer-out form, engine/GFFastFloatToLong.h), GFFloatToLongTrunc (15 / 6: `(long)x` through __ftol2).
// Retail-asm-helper: GFFloatToLongNear
// Retail-asm: 0x005BC41A `fld dword ptr [ebp+0Ch]` + 0x005BC41D `fistp dword ptr [ebp+8]` (GFGetGraphicFrame), 0x0088F8DF
//   `fld dword ptr [esp+8]` + 0x0088F8E3 `fistp dword ptr [esp]` (CGameScriptInterface::StartCountdownTimer), 0x00A147E0
//   GFFastSin, 0x006C3BD0 CTCFadeOutAndIn::FadeIn: a float stored to a frame slot, reloaded with `fld` and stored with a bare
//   dword `fistp`. VC7.1 (RTM 3077 and QFE 4035) compiles `(long)x` to __ftol2, `/QIfist` to `fistp qword`, `/arch:SSE2`
//   to cvttss2si: no C++ form gives the bare dword fistp, so the original is inline asm (rasm1 evidence, GOTCHAS).
// Semantics: fistp rounds by the FPU control word's RC field -- NOT floor and NOT truncation. The engine never changes
//   RC (its floor / ceiling helpers adjust the rounded value instead), so the result is round-to-nearest, ties to even;
//   an out-of-range or NaN input stores the integer indefinite 0x80000000 (invalid-operation exception masked).
// Portable twin (FABLE_PORTABLE): Strategy B, plain C++ (fable_x87_twins.h X87FistpNear) with identical results under
//   RC = nearest; tests in rebuild/tests/portable/test_GFFloatToLongNear.cpp.
#ifndef FABLE_ENGINE_GFFLOATTOLONGNEAR_H
#define FABLE_ENGINE_GFFLOATTOLONGNEAR_H
#include "fable_x87_twins.h"

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline long GFFloatToLongNear(float value)
    {
        return X87FistpNear((double)value);
    }
}
#endif

#ifdef FABLE_PORTABLE
using fable_portable::GFFloatToLongNear;
#else
__forceinline long __fastcall GFFloatToLongNear(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
#endif

#endif  // FABLE_ENGINE_GFFLOATTOLONGNEAR_H
