// Behaviour test for NShape::CSubdivisionTriangle::GetUpDirection (0x00760A70): UpDirection is +Z, flipped to -Z when the
// triangle's winding (second point relative to the first/third midpoint, against the third-minus-second edge) faces down.
#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
};
static inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
static inline float DotProduct(const C3DVector& a, const C3DVector& b)
{
    float d = (float)(double)(a.Z * b.Z);
    d += (float)(double)(a.Y * b.Y);
    d += (float)(double)(a.X * b.X);
    return d;
}
struct CRetailVector_pod { C3DVector* m_pFirst; C3DVector* m_pLast; C3DVector* m_pEnd; };
namespace NShape {
class CShape { public:
    virtual ~CShape() {}
    long              m_id;
    CRetailVector_pod m_points;
    CRetailVector_pod m_edges;
};
struct CClosedShapeBase : CShape { };
class CSubdivisionTriangle : public CClosedShapeBase { public:
    C3DVector UpDirection;
    void GetUpDirection();
};
void CSubdivisionTriangle::GetUpDirection()
{
    UpDirection = C3DVector(0.0f, 0.0f, 1.0f);
    const C3DVector* it = m_points.m_pFirst;
    C3DVector a = it[0];
    ++it;
    C3DVector b = it[0];
    C3DVector c = it[1];
    C3DVector side = CrossProduct(b - (a + c) * 0.5f, UpDirection);
    if (DotProduct(c - b, side) < 0.0f)
        UpDirection *= -1.0f;
}
}
int main()
{
    int fails = 0;
    NShape::CSubdivisionTriangle tri;
    C3DVector ccw[3] = { C3DVector(0.0f, 0.0f, 0.0f), C3DVector(1.0f, 0.0f, 0.0f), C3DVector(0.0f, 1.0f, 0.0f) };
    C3DVector cw[3]  = { C3DVector(0.0f, 0.0f, 0.0f), C3DVector(0.0f, 1.0f, 0.0f), C3DVector(1.0f, 0.0f, 0.0f) };
    tri.m_points.m_pFirst = ccw; tri.m_points.m_pLast = ccw + 3; tri.m_points.m_pEnd = ccw + 3;
    tri.UpDirection = C3DVector(5.0f, 5.0f, 5.0f);
    tri.GetUpDirection();
    float ccwZ = tri.UpDirection.Z;
    if (tri.UpDirection.X != 0.0f || tri.UpDirection.Y != 0.0f || (ccwZ != 1.0f && ccwZ != -1.0f)) { std::printf("ccw fail\n"); ++fails; }
    tri.m_points.m_pFirst = cw; tri.m_points.m_pLast = cw + 3; tri.m_points.m_pEnd = cw + 3;
    tri.GetUpDirection();
    // the opposite winding gives the opposite up direction
    if (tri.UpDirection.Z != -ccwZ || tri.UpDirection.X != 0.0f || tri.UpDirection.Y != 0.0f) { std::printf("cw fail\n"); ++fails; }
    // counter-clockwise seen from +Z (b - mid crossed with +Z, dotted with c - b) is negative -> flipped
    if (ccwZ != -1.0f) { std::printf("sign fail %f\n", ccwZ); ++fails; }
    // a point set raised in Z does not change the answer (only the XY winding matters)
    for (int i = 0; i < 3; ++i) cw[i].Z = 7.0f;
    tri.GetUpDirection();
    if (tri.UpDirection.Z != 1.0f) { std::printf("raised fail\n"); ++fails; }
    if (fails) { std::printf("00760a70_TEST FAIL\n"); return 1; }
    std::printf("00760a70_TEST PASS\n");
    return 0;
}