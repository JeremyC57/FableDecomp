// NPhysicsHelpers::GetClosestPointOnFiniteLineToPoint(const C3DVector& lineStart, const C3DVector& lineEnd, const C3DVector& point) -- project the point onto the segment's line; when the projection falls outside the segment, the nearer end point
// Grade: DIFFER(465v465) -- x87 scheduling only, 4 instructions: retail keeps the projection parameter on the stack (`fst [esp+30h] ; fmul st(1) ; fstp st(2) ; fstp st(0)`), ours pops and reloads it (`fstp [esp+30h] ; fmul [esp+30h] ; fstp st(1)`); every load, store, slot and branch else matches. ~30 diagnostic variants (named vs ctor-return operators, in-place vs separate direction, t named/unnamed, friend t*v, /= forms): only a separate `direction` variable gives the retail fst form, and it moves the direction to new slots (43-51 lines).
// Leaf: ?GetClosestPointOnFiniteLineToPoint@NPhysicsHelpers@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F5970, 480 bytes incl. int3 padding, /Gr `__fastcall` with a hidden
//   C3DVector result (result in ecx, lineStart in edx, lineEnd and point on the stack, `ret 8`).
// Identity: the catalog's "Geometry_ClosestPointOnLineSegment" is descriptive. Ego_r/FableWin PDB name set:
//   `?GetClosestPointOnFiniteLineToPoint@NPhysicsHelpers@@YI?AVC3DVector@@ABV2@00@Z`. Callers: the landed
//   NPhysicsHelpers::GetLineOutFromClosestPointOnFiniteLineToPoint 0x008F5B50 and GetPerpendicularDistanceFromFiniteLine
//   0x008F5BA0 (address-adjacent group), and 0x0080CA54.
// Constants: 0.0001f 0x0129BA3C, 1.0f 0x0122DED8. No callees (sqrt is the intrinsic `fsqrt`).
// Shape: the result starts as a copy of lineStart; the line is normalised in place (`line *= 1.0f / length`: the length
//   and the projection parameter spill to the dead argument slots, X stays on the x87 stack); C3DVector's operator-,
//   operator+ and operator*(float) return constructor expressions (unnamed RVO: the line is built in place, the
//   product temporary sits above it and the sum reuses the line's slots, copied dword-wise into the result as each
//   component lands; named-result operators move the temporaries, 48v..); the two squared end distances are compared
//   with the squared length as `!(start < len2 && end < len2)` (`test ah,5 ; jp`), falling back to the nearer end.
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& operator*=(float f)
    {
        X *= f;
        Y *= f;
        Z *= f;
        return *this;
    }
    float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};

namespace NPhysicsHelpers {

C3DVector __fastcall GetClosestPointOnFiniteLineToPoint(const C3DVector& lineStart, const C3DVector& lineEnd,
                                                        const C3DVector& point)
{
    C3DVector closest = lineStart;
    C3DVector line = lineEnd - lineStart;
    float length = line.GetLength();
    if (length > 0.0001f) {
        line *= 1.0f / length;
        closest = lineStart + line * (point - lineStart).Dot(line);
        float startDistanceSquared = (closest - lineStart).GetLengthSquared();
        float endDistanceSquared = (closest - lineEnd).GetLengthSquared();
        float lengthSquared = length * length;
        if (!(startDistanceSquared < lengthSquared && endDistanceSquared < lengthSquared)) {
            if (startDistanceSquared < endDistanceSquared)
                closest = lineStart;
            else
                closest = lineEnd;
        }
    }
    return closest;
}

}
