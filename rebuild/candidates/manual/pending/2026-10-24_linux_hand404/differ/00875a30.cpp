// NCameraModes::CTools::Get2DRotationAngle(const C3DVector& from, const C3DVector& to) const -- the signed angle (in turns) that rotates from's XY direction onto to's: acos of the clamped normalised dot product, negative when the 2D cross product is negative
// Grade: DIFFER(228v222) -- frame, argument copies, clamp, acos and cross product match; ours multiplies the dot product and the two lengths b-operand first (`fld b.Y ; fmul a.Y`, |b| before |a|) where retail starts with a, and VC tail-duplicates `angle * sign` into the negative arm (`fstp st(0) ; fmul [-1.0f]` + its own epilogue) where retail reloads -1.0f and joins at one `fmulp`. Tried: scalar float copies, implicit C3DVector copies, Z-unwritten copies with `to` first (this file), `from` first + __forceinline sign helper. Next: statement-order dot/length (`d = a.X*b.X; d += a.Y*b.Y;`), else lane gaps: single-term x87 commutation + tail duplication.
// Leaf: ?Get2DRotationAngle@CTools@NCameraModes@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00875A30, length 224 bytes (`__thiscall`, `this` unused, `ret 8`).
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

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

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
    float cos_angle = (a.X * b.X + a.Y * b.Y) / ((float)sqrt(a.X * a.X + a.Y * a.Y) * (float)sqrt(b.X * b.X + b.Y * b.Y));
    if (cos_angle < -1.0f)
        cos_angle = -1.0f;
    else if (cos_angle > 1.0f)
        cos_angle = 1.0f;
    float angle = (float)acos(cos_angle) * INV_TWO_PI;
    float sign = 1.0f;
    if (a.Y * b.X - b.Y * a.X < 0.0f)
        sign = -1.0f;
    return angle * sign;
}
}
