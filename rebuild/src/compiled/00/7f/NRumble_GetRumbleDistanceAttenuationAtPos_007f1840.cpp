// NRumble::GetRumbleDistanceAttenuationAtPos(const C3DVector& pos, const C3DVector& source) -- the rumble strength factor at a position: exp(-0.01 * squared distance to the source)
// Leaf: ?GetRumbleDistanceAttenuationAtPos@NRumble@@YIMABVC3DVector@@0@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F1840, 67 bytes (+ int3 padding), `__fastcall` (pos in ecx, source in edx),
//   float in st(0). FableWin.pdb `?GetRumbleDistanceAttenuationAtPos@NRumble@@YIMABVC3DVector@@0@Z`; callers 0x0069863C
//   and the CTCDRumble frame update 0x007F1A1D. Catalog label `Math_Distance3DExp` was descriptive.
// Constants: -0.01f @ 0x01231564. The intrinsic exp (`fldl2e ; ... f2xm1 ; fscale`).
// Shape: `(pos - source).GetLengthSquared()` through a C3DVector temporary (the x87 sum order x, z, y; three scalar
//   locals give z, y, x) into a NAMED float local, then `exp(distSq * -0.01f)` (inline in the exp argument VC distributes
//   the negative constant over the three squares: three fmuls and fsubps).
#include <math.h>
#pragma intrinsic(exp)

#include "engine/C3DVector.h"

namespace NRumble {
float __fastcall GetRumbleDistanceAttenuationAtPos(const C3DVector& pos, const C3DVector& source);
}

float __fastcall NRumble::GetRumbleDistanceAttenuationAtPos(const C3DVector& pos, const C3DVector& source)
{
    float distSq = (pos - source).GetSquaredMagnitude();
    return (float)exp(distSq * -0.01f);
}