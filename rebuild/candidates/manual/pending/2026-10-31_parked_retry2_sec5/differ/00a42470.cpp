// CPlane::GetIntersectionWithTriangle(const C3DVector& a, const C3DVector& b, const C3DVector& c, C3DVector& start, C3DVector& end) const -- find the segment where the plane cuts a triangle: pick the vertex alone on its side of the plane and intersect the plane with its two edges; false when no vertex pair shares a side
// Grade: DIFFER(750v750) -- one x87 load order: the third plane distance is now Z-product-first through a stepwise sum
//   (dc = c.Z * Normal.Z; dc += ...), which fixes the product order; the only residue is that retail loads c.Z first
//   (fld [edi+8]; fmul [esi+8]) where VC7.1 here loads Normal.Z first, in every operand spelling (sec5 retry: 7 grades)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A42470, length 750 bytes, `ret 14h`.
// Identity: Ego_r.pdb ?GetIntersectionWithTriangle@CPlane@@QBE_NABVC3DVector@@00AAV2@1@Z.
// Callees: CPlane::GetIntersectionWithLine(const C3DVector&, const C3DVector&, C3DVector*) const 0x00A422C0 (x2).
// Data: 0.0f 0x0122DEDC.
// Layout: Ego_r.pdb CPlane {C3DVector Normal +0, float Distance +0xc}.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
struct CPlane {
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
    float dc = c.Z * Normal.Z;
    dc += Normal.X * c.X + Normal.Y * c.Y;
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
