// NCameraModes::CTools::Get2DRotationAngle(const C3DVector& from, const C3DVector& to) const -- the signed angle (in turns) that rotates from's XY direction onto to's: acos of the clamped normalised dot product, negative when the 2D cross product is negative
// Grade: RELOCATION_MATCH (agent902, 2nd grade) -- `angle *= sign; return angle;`: VC7.1 then keeps the sign as an x87 local joined at one `fmulp` (retail); `return angle * sign;` / `sign * angle` tail-duplicate the product into the negative arm. The mirrored dot/length operands (agent901) fix the load order.
// Leaf: ?Get2DRotationAngle@CTools@NCameraModes@@QBEMABVC3DVector@@0@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00875A30, length 224 bytes (`__fastcall`, `this` unused, `ret 8`).
// Identity: Ego_r.pdb `?Get2DRotationAngle@CTools@NCameraModes@@QBEMABVC3DVector@@0@Z` (const, float, two const
//   C3DVector&) in the NCameraModes::CTools TU (next to GetLandscapePointIntersectionFromTo 0x00875550); no direct callers
//   or data references in TLC. The catalog label "Math_ComputeVectorAngle" is an auto label.
// Callee: _CIacos @ 0x00BFEBA2. Constants: -1.0f 0x0122DEE0, 1.0f 0x0122DED8, 0.0f 0x0122DEDC, 1/(2pi) 0x01237F08.
// Shape: both vectors are flattened into Z-unwritten C3DVector frame copies (the dead 0.0f Z stores are eliminated,
//   which is the 24-byte frame; arguments copied through ecx/edx, the `to` copy declared and stored first);
//   dot / (|a| * |b|) with `fsqrt` intrinsics, clamped to [-1, 1] in the dead `to` argument slot (immediate stores for
//   the clamps), acos through the CRT helper, scaled by 1/(2pi), then multiplied by a sign that starts at 1.0f on the
//   x87 stack and is swapped for -1.0f when the cross product is negative.
#include <math.h>
#pragma intrinsic(sqrt, acos)

#include "engine/C3DVector.h"

static const float INV_TWO_PI = 0.15915494f;           // 0x01237F08

namespace NCameraModes {
class CTools {
public:
    float Get2DRotationAngle(const C3DVector& from, const C3DVector& to) const;
};

float CTools::Get2DRotationAngle(const C3DVector& from, const C3DVector& to) const
{
    C3DVector b(to.X, to.Y, 0.0f);
    C3DVector a(from.X, from.Y, 0.0f);
    float cos_angle = (b.X * a.X + b.Y * a.Y) / ((float)sqrt(b.X * b.X + b.Y * b.Y) * (float)sqrt(a.X * a.X + a.Y * a.Y));
    if (cos_angle < -1.0f)
        cos_angle = -1.0f;
    else if (cos_angle > 1.0f)
        cos_angle = 1.0f;
    float angle = (float)acos(cos_angle) * INV_TWO_PI;
    float sign = 1.0f;
    if (a.Y * b.X - b.Y * a.X < 0.0f)
        sign = -1.0f;
    angle *= sign;
    return angle;
}
}