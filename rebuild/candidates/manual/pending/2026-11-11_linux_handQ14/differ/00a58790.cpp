// NBBBMaths::CSphere::GetFirstIntersectionWithFiniteLine(const C3DVector& start, const C3DVector& end, C3DVector& point, C3DVector& normal) const -- first point where the segment start..end enters the sphere (or start itself when the segment starts inside and leaves past end), with the outward unit normal there
// Grade: DIFFER(ours v retail) -- 699v707 on new engine/CSphere.h (NBBBMaths::CSphere, PDB reference signature) + engine/C3DVector.h: (1) retail homes the squared length in a compiler temp in the dead end-argument slot (fst [esp+2Ch]; fxch; fcompp) and copies it to a with integer moves, ours stores it straight into a's home (fstp [esp+28h]; fcomp [mem]); (2) the dir*t products load t first (fld t; fmul dir.X) where retail loads dir first -- c2prio shows ours gives dir's component candidates ids 7..9 below t1's 16 (higher id goes first), the old local-model draft (free operators) gave them 23..27 and matched that part. Tried: IsNonZero ZYX + GetSquaredMagnitude (one-line sums load X first), new SQUAREDMAGNITUDE_ZYX opt-in (same).
// Leaf: ?GetFirstIntersectionWithFiniteLine@CSphere@NBBBMaths@@QBE_NABVC3DVector@@0AAV3@1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A58790, 707 bytes, __thiscall, `ret 10h`. Callee: C3DVector::AccurateNormalise 0x00A14440.
#define FABLE_RETAIL_C3DVECTOR
#include <math.h>
#include "engine/CSphere.h"

namespace NBBBMaths {

bool CSphere::GetFirstIntersectionWithFiniteLine(const C3DVector& start, const C3DVector& end, C3DVector& point, C3DVector& normal) const
{
    C3DVector dir = end - start;
    float length_squared = dir.Z * dir.Z;
    length_squared += dir.Y * dir.Y;
    length_squared += dir.X * dir.X;
    if (!(length_squared > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO))
        return false;
    float a = length_squared;
    C3DVector offset = start - Base;
    float b = dir.Z * offset.Z;
    b += dir.Y * offset.Y;
    b += dir.X * offset.X;
    b *= 2.0f;
    float c = offset.Z * offset.Z;
    c += offset.Y * offset.Y;
    c += offset.X * offset.X;
    float disc = b * b - (c - Radius * Radius) * a * 4.0f;
    if (disc < 0.0f)
        return false;
    float root = sqrtf(disc);
    float t1 = (root - b) / (2.0f * a);
    float t2 = (-b - root) / (2.0f * a);
    float best = 2.0f;
    C3DVector hit;
    if (t1 > 0.0f && t1 < 1.0f && t1 < best) {
        best = t1;
        hit = start + dir * t1;
    }
    if (t2 > 0.0f && t2 < 1.0f && t2 < best) {
        best = t2;
        hit = start + dir * t2;
    }
    if ((t1 < 0.0f && t2 > 1.0f) || (t2 < 0.0f && t1 > 1.0f))
        hit = start;
    else if (!(best > 0.0f && best < 1.0f))
        return false;
    point = hit;
    normal = point - Base;
    normal.AccurateNormalise();
    return true;
}

}
