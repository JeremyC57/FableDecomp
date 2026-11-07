// NShape::CSubdivisionTriangle::GetUpDirection() -- set UpDirection to +Z, flipped when the triangle's winding faces down
// Grade: RELOCATION_MATCH (332 bytes) -- the dot product is a sequenced running sum in Z, Y, X order (`float d = a.Z*b.Z; d += a.Y*b.Y; d += a.X*b.X;`: the only sum form that keeps retail's fmul operand pairs) with each product wrapped `(float)(double)(...)` (an identity on float products, GOTCHAS 007a1870 lever: the extra folded x87 IR nodes move the independent `pop esi` two x87 instructions earlier, into retail's slot); the plain `a.X*b.X + a.Y*b.Y + a.Z*b.Z` was DIFFER(332v332), 6 masked lines (hand757)
// Leaf: GetUpDirection
// Retail 0x00760A70, 336 bytes, `ret`. No callees. Pools: 0.5f 0x0122F59C, 0.0f 0x0122DEDC, -1.0f 0x0122DEE0.
// Identity: Ego_r.pdb ?GetUpDirection@CSubdivisionTriangle@NShape@@AAEXXZ (private; header debt: no shared NShape headers yet; the local
//   CShape model carries only the point / edge vectors read here).
// Shape: UpDirection = +Z; the midpoint of the first and third points, the second point minus it crossed with +Z gives the side
//   vector; a negative dot with the third-minus-second edge flips UpDirection (`*= -1.0f`, three fmul by the pooled -1.0f).
// Layout: the landed NShape::CShape model (0x00762440 ctor, 0x00765370 deleting dtor): point vector +0x08 (first
// pointer = the three corner C3DVectors), UpDirection +0x20.
// Shape: the corners walk an iterator (`a = it[0]; ++it; b = it[0]; c = it[1];`, retail `add eax,0Ch`) and C3DVector needs a
// memberwise user copy ctor; UpDirection is assigned a (0,0,1) temporary; the three corners are copied into locals; the side vector
// is cross(b - (a + c) * 0.5, Up) and the up direction is negated (`*= -1`) when dot(c - b, side) < 0.
#include <stddef.h>

#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
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

struct CRetailVector_pod {
    C3DVector* m_pFirst;
    C3DVector* m_pLast;
    C3DVector* m_pEnd;
};

namespace NShape {
class CShape { public:
    virtual ~CShape();
    long              m_id;                           // +0x04
    CRetailVector_pod m_points;                       // +0x08
    CRetailVector_pod m_edges;                        // +0x14
};
struct CClosedShapeBase : CShape { };
class CSubdivisionTriangle : public CClosedShapeBase { public:
    C3DVector UpDirection;                            // +0x20
private:
    void GetUpDirection();                            // Ego_r.pdb AAEXXZ (private)
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