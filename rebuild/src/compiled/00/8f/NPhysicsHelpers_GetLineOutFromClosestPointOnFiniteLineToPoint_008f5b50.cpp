// NPhysicsHelpers::GetLineOutFromClosestPointOnFiniteLineToPoint(const C3DVector&, const C3DVector&, const C3DVector&) -- vector from the closest point on the segment to the point
// Leaf: ?GetLineOutFromClosestPointOnFiniteLineToPoint@
// Retail 0x008F5B50, 0x4A bytes, __fastcall (hidden C3DVector result in ecx, lineStart in edx, lineEnd and point on the
//   stack, `ret 8`). Manifest label matches the PDB name set
//   (`?GetLineOutFromClosestPointOnFiniteLineToPoint@NPhysicsHelpers@@YI?AVC3DVector@@ABV2@00@Z`).
// Callee: NPhysicsHelpers::GetClosestPointOnFiniteLineToPoint __fastcall @ 0x008F5970 (same argument registers, so edx
//   passes straight through; its hidden result is the 12-byte frame local).
// Shape: C3DVector needs a user default ctor (non-POD, so the __fastcall result travels in ecx) and the memberwise user
//   copy ctor. `operator-` builds a NAMED result field by field and returns it: X and Y are stored straight from the x87
//   stack and Z goes through the dead frame word and an integer copy (`fstp [esp+0Ch] ; mov ecx,[esp+0Ch]`). The
//   `return C3DVector(X - v.X, ...)` form evaluates Z first (64v74).
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
};

namespace NPhysicsHelpers {
C3DVector __fastcall GetClosestPointOnFiniteLineToPoint(const C3DVector& lineStart, const C3DVector& lineEnd,
                                                        const C3DVector& point);   // 0x008F5970

C3DVector __fastcall GetLineOutFromClosestPointOnFiniteLineToPoint(const C3DVector& lineStart, const C3DVector& lineEnd,
                                                                   const C3DVector& point)
{
    C3DVector closest = GetClosestPointOnFiniteLineToPoint(lineStart, lineEnd, point);
    return point - closest;
}
}