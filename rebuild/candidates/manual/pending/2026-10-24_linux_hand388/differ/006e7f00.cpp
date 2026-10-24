// GetAttackerProngSelectionCompareValue(const C3DVector& attackerPos, const C3DVector& defenderPos, const C3DVector& prongVector, const C3DVector& placementVector) -- how well a prong lines up with a placement direction in the ground plane: 1 + the cosine between the two flattened, normalised vectors (0..2), or the epsilon when either is degenerate
// Grade: DIFFER(205v207) -- only the first (placement) normalise: retail multiplies X with `fld st(0) ; fmul st(2)` and keeps the old X on the x87 stack until after Z (`fstp st(1)`), ours consumes it with `fxch st(1) ; fmul st(1)`; the prong normalise, both degenerate exits and the dot match
// Leaf: ?GetAttackerProngSelectionCompareValue@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E7F00, 207 bytes (208 with padding), file-static `__fastcall` (the two
//   positions in ecx/edx are unused), `ret 8`.
// Identity: Ego_r.pdb name set `?GetAttackerProngSelectionCompareValue@?A0xd8b789b9@@YIMABVC3DVector@@000@Z`
//   (anonymous namespace of the combat-manager TU); callers 0x006F2D97 / 0x006F2F1B in the attacker-position
//   evaluator pass two GetPosition results in ecx/edx and two vector addresses on the stack, then add the
//   current-weapon compare value 0x006E9F60. The catalog's "Math_DotProductNormalized" is descriptive.
// Callees: none (x87 inline: fsqrt).
// Globals: the 1e-4 tolerance @ 0x0129BA3C (read as a variable, also the degenerate result), 1.0f @ 0x0122DED8,
//   0.0f @ 0x0122DEDC (the flattened Z, kept as `Z *= inv`).
// Shape: both vectors are copied flat (X, Y, 0) up front (placement X stays on the x87 stack, the rest in the frame)
//   and normalised in place through the engine's inline length squared (`1.0f / sqrt`), the placement vector first;
//   the result is `prong . placement + 1`, the dot written Z-term first (`Z*v.Z + (X*v.X + Y*v.Y)` reproduces retail's
//   Y, X, fxch, Z order with the dead placement Z dropped by the final `fstp st(1)`).
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    float Dot(const C3DVector& v) const { return Z * v.Z + (X * v.X + Y * v.Y); }
    C3DVector& operator*=(float f) { X = f * X; Y = f * Y; Z = f * Z; return *this; }
};

extern const float g_Epsilon;                               // 0x0129BA3C (1e-4f)

float __fastcall GetAttackerProngSelectionCompareValue(const C3DVector& attackerPos, const C3DVector& defenderPos,
                                                       const C3DVector& prongVector, const C3DVector& placementVector)
{
    C3DVector flatPlacement(placementVector.X, placementVector.Y, 0.0f);
    C3DVector flatProng(prongVector.X, prongVector.Y, 0.0f);
    float lengthSquared = flatPlacement.GetLengthSquared();
    if (lengthSquared < g_Epsilon)
        return g_Epsilon;
    flatPlacement *= 1.0f / (float)sqrt(lengthSquared);

    lengthSquared = flatProng.GetLengthSquared();
    if (lengthSquared < g_Epsilon)
        return g_Epsilon;
    flatProng *= 1.0f / (float)sqrt(lengthSquared);

    return flatProng.Dot(flatPlacement) + 1.0f;
}
