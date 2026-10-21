// Grade: DIFFER(506v495) -- retail is compiled with callee knowledge: the quadric stays in edx across GetInverseMatrixPart (0xAC70F0) and ecx is reused across both EvaluateError (0xAC6D50) calls with no reload, which the lane only reproduces with the REAL callee bodies above the caller under #pragma auto_inline(off) (GOTCHAS chunk 154/187); the lane holds the quadric in esi (push esi) and its frame is 0x74 v 0x68 (the vector-part temporary). Determinant, midpoint/edge and the two acceptance tests are in retail order.
// QuadricOptimiserInternals::COptimisedPrimitive::ComputeOptimalVertexPlacement(const CQuadric& quadric, const C3DVector&
// a, const C3DVector& b) -- the contracted vertex goes to the quadric's minimum (inverse matrix part applied to the
// negated vector part) when the matrix part is invertible, the minimum's error beats the edge midpoint's, and it lies
// within twice the edge length of the midpoint; otherwise to the midpoint
// Leaf: ComputeOptimalVertexPlacement
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC7750, 0x1ED bytes (padded to 496), `ret 10h`, `and esp,-8` frame.
// Callees: CQuadric::GetInverseMatrixPart() 0x00AC70F0 (labelled GetMatrixPart; CMatrix3x4 by value),
//   CQuadric::EvaluateError(const C3DVector&) 0x00AC6D50 (double). Data: 0.5f 0x0122F59C, epsilon 0x0129BA3C (0.0001f).
// Layout: CQuadric holds ten doubles (A2 +0, AB +8, AC +0x10, AD +0x18, B2 +0x20, BC +0x28, BD +0x30, C2 +0x38,
//   CD +0x40, D2 +0x48). Local model.
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
struct CMatrix3x4 {
    C3DVector Right, Up, Forward, Position;
    C3DVector Rotate(const C3DVector& v) const
    {
        return C3DVector(Forward.X * v.Z + Up.X * v.Y + Right.X * v.X,
                         Forward.Y * v.Z + Up.Y * v.Y + Right.Y * v.X,
                         Forward.Z * v.Z + Up.Z * v.Y + Right.Z * v.X);
    }
};
extern float g_epsilon_0129ba3c;                      // 0x0129BA3C
namespace QuadricOptimiserInternals {
class CQuadric {
public:
    double A2, AB, AC, AD, B2, BC, BD, C2, CD, D2;
    double GetDeterminant() const
    {
        return A2 * (B2 * C2 - BC * BC) + AC * (AB * BC - B2 * AC) + AB * (BC * AC - AB * C2);
    }
    C3DVector GetVectorPart() const { return C3DVector((float)AD, (float)BD, (float)CD); }
    CMatrix3x4 GetInverseMatrixPart() const;        // 0x00AC70F0
    double EvaluateError(const C3DVector& v) const; // 0x00AC6D50
};
class COptimisedPrimitive {
public:
    C3DVector ComputeOptimalVertexPlacement(const CQuadric& quadric, const C3DVector& a, const C3DVector& b);
};
C3DVector COptimisedPrimitive::ComputeOptimalVertexPlacement(const CQuadric& quadric, const C3DVector& a, const C3DVector& b)
{
    C3DVector mid = (a + b) * 0.5f;
    C3DVector edge = b - a;
    if (quadric.GetDeterminant() > g_epsilon_0129ba3c) {
        CMatrix3x4 inverse = quadric.GetInverseMatrixPart();
        C3DVector optimal = inverse.Rotate(-quadric.GetVectorPart());
        float optimalError = (float)quadric.EvaluateError(optimal);
        if (quadric.EvaluateError(mid) > optimalError && (optimal - mid).GetLength() < edge.GetLength() * 2.0f)
            return optimal;
    }
    return mid;
}
}
