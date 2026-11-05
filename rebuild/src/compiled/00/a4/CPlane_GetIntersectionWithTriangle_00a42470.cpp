// CPlane::GetIntersectionWithTriangle(const C3DVector& a, const C3DVector& b, const C3DVector& c, C3DVector& start, C3DVector& end) const -- find the segment where the plane cuts a triangle: pick the vertex alone on its side of the plane and intersect the plane with its two edges; false when no vertex pair shares a side
// Shape: the three plane distances are written as retail evaluates them: da through the inline GetDistanceToPoint, db as
//   Z + (X + Y), dc as a stepwise sum Z-first through a reference alias of Normal (`const C3DVector& n = Normal;`). The alias
//   is what makes VC7.1 load c.Z first (`fld [edi+8] ; fmul [esi+8]`); every direct `Normal.Z` spelling loaded Normal first.
//   C3DVector / CPlane are classes (PDB key V: `ABVC3DVector`), so the leaf binds the PDB symbol.
// Leaf: ?GetIntersectionWithTriangle@CPlane@@QBE_NABVC3DVector@@00AAV2@1@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Identity: Ego_r.pdb ?GetIntersectionWithTriangle@CPlane@@QBE_NABVC3DVector@@00AAV2@1@Z.
// Callees: CPlane::GetIntersectionWithLine(const C3DVector&, const C3DVector&, C3DVector*) const 0x00A422C0 (x2).
// Data: 0.0f 0x0122DEDC.
// Layout: Ego_r.pdb CPlane {C3DVector Normal +0, float Distance +0xc}.
#include "engine/C3DVector.h"
class CPlane { public:
    C3DVector Normal;                                   // +0x00
    float     Distance;                                 // +0x0c
    float GetDistanceToPoint(const C3DVector& p) const { return Normal.X * p.X + Normal.Y * p.Y + Normal.Z * p.Z - Distance; }
    bool GetIntersectionWithLine(const C3DVector& point, const C3DVector& direction, C3DVector* intersection) const;   // 0x00A422C0
    bool GetIntersectionWithTriangle(const C3DVector& a, const C3DVector& b, const C3DVector& c, C3DVector& start, C3DVector& end) const;
};

bool CPlane::GetIntersectionWithTriangle(const C3DVector& a, const C3DVector& b, const C3DVector& c, C3DVector& start, C3DVector& end) const
{
    float da = GetDistanceToPoint(a);
    float db = Normal.Z * b.Z + (Normal.X * b.X + Normal.Y * b.Y) - Distance;
    const C3DVector& n = Normal;
    float dc = c.Z * n.Z;
    dc += n.X * c.X + n.Y * c.Y;
    dc -= Distance;
    C3DVector origin;
    C3DVector edge1;
    C3DVector edge2;
    if ((da > 0.0f && db > 0.0f) || (da < 0.0f && db < 0.0f)) {
        origin = c;
        edge1 = a - origin;
        edge2 = b - origin;
    } else if ((da > 0.0f && dc > 0.0f) || (da < 0.0f && dc < 0.0f)) {
        origin = b;
        edge1 = a - origin;
        edge2 = c - origin;
    } else if ((db > 0.0f && dc > 0.0f) || (db < 0.0f && dc < 0.0f)) {
        origin = a;
        edge1 = b - origin;
        edge2 = c - origin;
    } else {
        return false;
    }
    GetIntersectionWithLine(origin, edge1, &start);
    GetIntersectionWithLine(origin, edge2, &end);
    return true;
}