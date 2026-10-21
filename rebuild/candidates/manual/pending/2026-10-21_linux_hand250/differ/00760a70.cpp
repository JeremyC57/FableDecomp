// Grade: DIFFER(332v332) -- only `pop esi` scheduled after the last faddp where retail pops it two x87 instructions earlier (before the final fxch/fmul)
// NShape::CSubdivisionTriangle::GetUpDirection() -- set UpDirection to +Z, flipped when the triangle's winding faces down
// Leaf: GetUpDirection
// Retail 0x00760A70, 336 bytes, `ret`. No callees. Pools: 0.5f 0x0122F59C, 0.0f 0x0122DEDC, -1.0f 0x0122DEE0.
// Layout: the landed NShape::CShape model (0x00762440 ctor, 0x00765370 deleting dtor): point vector +0x08 (first
// pointer = the three corner C3DVectors), UpDirection +0x20.
// Shape: the corners walk an iterator (`a = it[0]; ++it; b = it[0]; c = it[1];`, retail `add eax,0Ch`) and C3DVector needs a
// memberwise user copy ctor; UpDirection is assigned a (0,0,1) temporary; the three corners are copied into locals; the side vector
// is cross(b - (a + c) * 0.5, Up) and the up direction is negated (`*= -1`) when dot(c - b, side) < 0.
#include <stddef.h>

struct C3DVector {
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
    return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}

struct CRetailVector_pod {
    C3DVector* m_pFirst;
    C3DVector* m_pLast;
    C3DVector* m_pEnd;
};

namespace NShape {
struct CShape {
    virtual ~CShape();
    long              m_id;                           // +0x04
    CRetailVector_pod m_points;                       // +0x08
    CRetailVector_pod m_edges;                        // +0x14
};
struct CClosedShapeBase : CShape { };
struct CSubdivisionTriangle : CClosedShapeBase {
    C3DVector UpDirection;                            // +0x20
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
