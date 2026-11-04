// GFGet3DBoxDistance(const C3DVector&, const C3DVector&) -- "box" (Chebyshev) distance between two 3D points: the largest of |dx|, |dy| and |dz|
// Leaf: ?GFGet3DBoxDistance@
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1B30, length 68 bytes.
// PDB (FableWin): ?GFGet3DBoxDistance@@YIMABVC3DVector@@0@Z -- __fastcall, the points in ecx / edx, float result.
// The catalog label "Vector3_MaxComponentAbsDifference" is descriptive. C3DVector = {X, Y, Z} (PDB layout).
// Shape: as GFGet2DBoxDistance (0x009E1AD0) with a nested by-value max: dz is computed first (its statement comes
// first in the source; declared after dy it is computed last) and spilled to
// [esp+4], dx stays on the FPU, dy is spilled to [esp]; each `fcom` + `test ah,41h ; je` keeps the FPU value when greater.
#include <math.h>

#pragma pack(push, 1)
#include "engine/C3DVector.h"
#pragma pack(pop)

template <class T>
inline T GFMax(T a, T b)
{
    return a > b ? a : b;
}

float __fastcall GFGet3DBoxDistance(const C3DVector& a, const C3DVector& b)
{
    float dz = (float)fabs(a.Z - b.Z);
    float dx = (float)fabs(a.X - b.X);
    float dy = (float)fabs(a.Y - b.Y);
    return GFMax(GFMax(dx, dy), dz);
}