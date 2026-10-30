// Grade: DIFFER(497v495) (retry1; was 506v495) -- with the callee bodies in the TU (the landed EvaluateError 0x00AC6D50 and the parked cofactor inverse 0x00AC70F0, both noinline) VC7.1 keeps the quadric in edx across both calls and reuses ecx exactly as retail; the PDB member names (AA/BB/CC/DD, Weight) replace A2/B2/C2/D2. Residue: (1) two determinant products come out with swapped commutative operands (`fld BC ; fmul AB` / `fld AC ; fmul BC`, retail `fld AB ; fmul BC` / `fld BC ; fmul AC`) for either source operand order; (2) the midpoint/edge/optimal temporaries sit 0xC higher in the frame (retail packs them from [esp+14h]) | tried: callee bodies before and after the caller (identical output: VC7.1 compiles callees first), swapped source operand order (identical) | next: frame-slot order of the C3DVector temporaries (named locals vs temporaries)
// QuadricOptimiserInternals::COptimisedPrimitive::ComputeOptimalVertexPlacement(const CQuadric& quadric, const C3DVector&
// a, const C3DVector& b) -- the contracted vertex goes to the quadric's minimum (inverse matrix part applied to the
// negated vector part) when the matrix part is invertible, the minimum's error beats the edge midpoint's, and it lies
// within twice the edge length of the midpoint; otherwise to the midpoint
// Leaf: ?ComputeOptimalVertexPlacement@COptimisedPrimitive@QuadricOptimiserInternals@@
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
    double AA, AB, AC, AD, BB, BC, BD, CC, CD, DD;  // Ego_r.pdb CQuadric (+ Weight +0x50)
    double Weight;
    double GetDeterminant() const
    {
        return AA * (BB * CC - BC * BC) + AC * (AB * BC - BB * AC) + AB * (BC * AC - AB * CC);
    }
    C3DVector GetVectorPart() const { return C3DVector((float)AD, (float)BD, (float)CD); }
    CMatrix3x4 GetInverseMatrixPart() const;        // 0x00AC70F0
    double EvaluateError(const C3DVector& v) const; // 0x00AC6D50
};

// 0x00AC6D50 (landed) and 0x00AC70F0 (the parked cofactor inverse, retry1 base): defined first so VC7.1 knows their
// register use (neither touches edx).
__declspec(noinline) double CQuadric::EvaluateError(const C3DVector& v) const
{
    float x = v.X, y = v.Y, z = v.Z;
    return (z * z) * CC + (y * z * 2.0f) * BC + (y * y) * BB + (x * z * 2.0f) * AC + (x * y * 2.0f) * AB
         + (z * 2.0f) * CD + (y * 2.0f) * BD + (x * 2.0f) * AD + (x * x) * AA + DD;
}
__declspec(noinline) CMatrix3x4 CQuadric::GetInverseMatrixPart() const
{
    CMatrix3x4 m;
    double c13 = AC * BC - CC * AB;
    double c23 = BC * AB - AC * BB;
    double c33 = CC * BB - BC * BC;
    double det = c33 * AA;
    det += c23 * AC;
    det += c13 * AB;
    double inv = 1.0 / det;
    m.Right.X = (float)(c33 * inv);
    m.Up.X = (float)(c13 * inv);
    m.Forward.X = (float)(c23 * inv);
    m.Right.Y = m.Up.X;
    m.Right.Z = m.Forward.X;
    m.Up.Y = (float)((CC * AA - AC * AC) * inv);
    m.Forward.Y = (float)((AC * AB - AA * BC) * inv);
    m.Up.Z = m.Forward.Y;
    m.Forward.Z = (float)((AA * BB - AB * AB) * inv);
    return m;
}
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
