// CVectorCompare_BestForTargeting::GetCompareValueForPoint(const C3DVector& point) -- the targeting score of a point: offset from AnchorPos (flattened to XY when CompareInXYPlane), squared length, normalised direction dotted with PreferredLine, both handed to ComputeValue
// Grade: RELOCATION_MATCH (130 bytes) -- GetLengthSquared written as the chain `s = Y*Y; s += X*X; s += Z*Z;` gives retail's x87 square order (applied by the landing session)
// Leaf: ?GetCompareValueForPoint@CVectorCompare_BestForTargeting@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F9660, length 130 bytes, `ret 4`.
// Callees: CVectorCompare_BestForTargeting::ComputeValue(float sqrDist, float angle) @ 0x007F9480 (`ret 8`).
// Float pool: 0.0f @ 0x0122DEDC, 1e-4f @ 0x0129BA3C, 1.0f @ 0x0122DED8.
// Layout (PDB, 44 bytes, matches retail): AnchorPos +0x00, PreferredLine +0x0c, AngleCompare +0x18, SqrDistCompare +0x1c,
// OverrideShortDistanceFactor +0x20, OverrideShortDistanceFalloff +0x24, CompareInXYPlane +0x28.
// Shape: the squared length is kept in the dead `point` argument slot (`fst [esp+4]`) and pushed from there as a dword.
#include <math.h>
#pragma intrinsic(sqrt)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { float s = Y * Y; s += X * X; s += Z * Z; return s; }
    void operator*=(float f) { X *= f; Y *= f; Z *= f; }
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
};
struct CVectorCompare_BestForTargeting {
    C3DVector AnchorPos;                           // +0x00
    C3DVector PreferredLine;                       // +0x0c
    float     AngleCompare;                        // +0x18
    float     SqrDistCompare;                      // +0x1c
    float     OverrideShortDistanceFactor;         // +0x20
    float     OverrideShortDistanceFalloff;        // +0x24
    bool      CompareInXYPlane;                    // +0x28
    float ComputeValue(float sqrDist, float angle) const;   // 0x007F9480
    float GetCompareValueForPoint(const C3DVector& point) const;
};
float CVectorCompare_BestForTargeting::GetCompareValueForPoint(const C3DVector& point) const
{
    C3DVector d = point - AnchorPos;
    if (CompareInXYPlane)
        d.Z = 0.0f;
    float sqrDist = d.GetLengthSquared();
    if (sqrDist > 1e-4f)
        d *= 1.0f / (float)sqrt(sqrDist);
    return ComputeValue(sqrDist, d.Dot(PreferredLine));
}