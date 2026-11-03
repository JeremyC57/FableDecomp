// GFFastFloatToLong(float) -- round a float to a long with the x87 rounding mode (round to nearest, ties to even, by
// default), storing through a pointer to the result: the engine's pointer-out inline-assembly float-to-long, expanded in
// place by every caller.
// Identity: FableWin.pdb ?GFFastFloatToLong@@YIJM@Z (__fastcall, out of line only in the debug build: 29 bytes / 12
//   instructions = `push ebp ; mov ebp,esp ; sub esp,8 ; lea eax,[ebp-4] ; mov [ebp-8],eax ; fld [ebp+8] ;
//   mov edx,[ebp-8] ; fistp dword ptr [edx] ; mov eax,[ebp-4] ; mov esp,ebp ; pop ebp ; ret 4`, the debug frame around
//   this block with its result pointer local; the bare form without the pointer is GFFloatToLongNear's 19 / 9).
// Retail-asm-helper: GFFastFloatToLong
// Retail-asm: 0x0081E55A `fld dword ptr [esp+0Ch]` + 0x0081E55E `mov edx,[esp+4]` + 0x0081E562 `fistp dword ptr [edx]`
//   (FtoL), 0x00B1B5E0 (CSprite::GetHeight), 0x00B0A420 / 0x00B0A4A0 (NParticleEngine::CParticle::SetAngleChange /
//   SetCreationTime), 0x00BA7B30 (CEngineInternalPrimitiveBase::GetAlphaIncludingDistanceFade): `fld` of the float, the
//   result pointer reloaded into edx, a bare `fistp dword ptr [edx]`. VC7.1 C++ `(long)x` calls __ftol2 and `/QIfist`
//   gives `fistp qword`: no C++ form stores a dword fistp, so the original is inline asm.
// Semantics: as GFFloatToLongNear -- fistp rounds by the control word's RC field (round to nearest, ties to even; the
//   engine never changes RC), NaN / out of range store 0x80000000.
// Portable twin (FABLE_PORTABLE): Strategy B, plain C++ (fable_x87_twins.h X87FistpNear); tests in
//   rebuild/tests/portable/test_GFFastFloatToLong.cpp.
#ifndef FABLE_ENGINE_GFFASTFLOATTOLONG_H
#define FABLE_ENGINE_GFFASTFLOATTOLONG_H
#include "fable_x87_twins.h"

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline long GFFastFloatToLong(float value)
    {
        return X87FistpNear((double)value);
    }
}
#endif

#ifdef FABLE_PORTABLE
using fable_portable::GFFastFloatToLong;
#else
__forceinline long __fastcall GFFastFloatToLong(float value)
{
    long result;
    long* resultPtr = &result;
    __asm fld value
    __asm mov edx, resultPtr
    __asm fistp dword ptr [edx]
    return result;
}
#endif

#endif  // FABLE_ENGINE_GFFASTFLOATTOLONG_H
