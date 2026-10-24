// NPhysicsHelpers::GetPerpendicularDistanceFromFiniteLine(const C3DVector&, const C3DVector&, const C3DVector&) -- distance from the point to the closest point on the segment
// Leaf: ?GetPerpendicularDistanceFromFiniteLine@
// Retail 0x008F5BA0, 0x4A bytes, __fastcall float (lineStart in ecx, lineEnd in edx, point on the stack, `ret 4`).
//   Manifest label "Geometry_DistancePointToLineSegment" (_global) is descriptive; the PDB name set has
//   `?GetPerpendicularDistanceFromFiniteLine@NPhysicsHelpers@@YIMABVC3DVector@@00@Z` (the address-adjacent
//   NPhysicsHelpers group: 0x8F58C0 GetLineThingSearchArea, 0x8F5970 GetClosestPointOnFiniteLineToPoint, 0x8F5B50
//   GetLineOutFromClosestPointOnFiniteLineToPoint, this one).
// Callee: GetClosestPointOnFiniteLineToPoint __fastcall @ 0x008F5970 (hidden result = the 12-byte frame local).
// Shape: the closest point is a NAMED local (an unnamed temporary is read back through eax, 70v74); the difference stays
//   on the x87 stack and the squares are summed Z, X, Y (`fld st(0) ; fmul st(1) ; fld st(3) ; fmul st(4)`), which is the
//   statement-ordered sum `s = Z*Z; s += X*X; s += Y*Y;` (a single `X*X + Y*Y + Z*Z` expression squares Z, Y, X here).
//   The same named-result operator- as 0x8F5B50 also matches.
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector operator-(const C3DVector& v) const
    {
        C3DVector result;
        result.X = X - v.X;
        result.Y = Y - v.Y;
        result.Z = Z - v.Z;
        return result;
    }
    float GetLength() const
    {
        float lengthSquared = Z * Z;
        lengthSquared += X * X;
        lengthSquared += Y * Y;
        return (float)sqrt(lengthSquared);
    }
};

namespace NPhysicsHelpers {
C3DVector __fastcall GetClosestPointOnFiniteLineToPoint(const C3DVector& lineStart, const C3DVector& lineEnd,
                                                        const C3DVector& point);   // 0x008F5970

float __fastcall GetPerpendicularDistanceFromFiniteLine(const C3DVector& lineStart, const C3DVector& lineEnd,
                                                        const C3DVector& point)
{
    C3DVector closest = GetClosestPointOnFiniteLineToPoint(lineStart, lineEnd, point);
    return (point - closest).GetLength();
}
}