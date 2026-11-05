// NParticleEngine::GFInterpolate(const C3DVector& a, const C3DVector& b, float t) -- linear interpolation of two vectors: a * (1 - t) + b * t, returned by value
// Leaf: ?GFInterpolate@NParticleEngine@@YI?AVC3DVector@@ABV2@0M@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF3CF0, 99 bytes, __fastcall: hidden result pointer in ecx (returned in eax), a in edx,
//   b and t on the stack (`ret 8`).
// Identity: FableWin.pdb name set `?GFInterpolate@NParticleEngine@@YI?AVC3DVector@@ABV2@0M@Z` (the C3DVector overload of the
//   particle engine's GFInterpolate family, beside the GFInterpolateArray templates); no direct call sites (reached through
//   the particle parameter interpolation templates' COMDAT-folded or address-taken uses).
// Callees: none. Float 1.0 at 0x0122DED8.
// Label: catalog "InterpolateRadialBlurPositionParams" is descriptive.
// Shape: the expression `a * (1.0f - t) + b * t` over C3DVector's inline operator*(float) / operator+ by-value temporaries:
//   the right operand (b * t) is evaluated first.
#pragma pack(push, 1)
#include "engine/C3DVector.h"
#pragma pack(pop)

namespace NParticleEngine {
C3DVector __fastcall GFInterpolate(const C3DVector& a, const C3DVector& b, float t)
{
    return a * (1.0f - t) + b * t;
}
}