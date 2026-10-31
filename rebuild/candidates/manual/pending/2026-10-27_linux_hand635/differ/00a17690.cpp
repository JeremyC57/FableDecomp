// CPhysicsMesh::HasBoundingBoxIntersectedFromTo(float radius, const C3DVector& from, const C3DVector& to) const -- 2D (X/Y) overlap test between the mesh's bounding box and the box around `from` grown by the radius plus the from-to distance (+50) on each axis
// Grade: DIFFER(262v262) -- one x87 commutation in the centre X lane: retail `fld [ecx+84h] ; fadd [ecx+90h]` (MinCoord.X first) while the Y lane loads MaxCoord.Y first; VC7.1 orders both lanes the same way (member operator+: Max first in both; friend operator+: Min first in both). Tried this session: operand swap inside operator+ (inert: VC canonicalises), named-return operator+ (314), box reference local, inline C3DBoundingBox GetCentre/GetHalfSize helpers, by-value operand (281), friend operator+ (flips both lanes), friend with the Y lane swapped (inert). Members respelled to the Ego_r.pdb MinCoord/MaxCoord.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A17690, 262 bytes, `ret 0Ch`. PDB: ?HasBoundingBoxIntersectedFromTo@CPhysicsMesh@@QBE_NMABVC3DVector@@0@Z.
//   Called from 0x00A120C1, 0x00A18773, 0x00A18803.
// Constants: 0x0122F59C = 0.5f, 0x0124E7F8 = 50.0f.
// Layout: Ego_r.pdb CPhysicsMesh with the retail container sizes (12-byte vectors/CArrays, 0x10 CVectorMaps): BoundingBox
//   (C3DBoundingBox, Min then Max) at retail +0x84 (PDB +0xA0).
// Shape: centre, half-size and delta are inline C3DVector operator expressions (each pair stays on the x87 stack; the dead Z
//   lanes are dropped but centre/half-size keep their 12-byte frame slots); the two padded deltas are named float locals
//   homed in the dead `to` / `from` argument slots; four sequential tests with one shared `xor al,al`
//   exit (`if (...) return true; return false;`).
#include <math.h>
#pragma intrinsic(fabs)

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};
struct C3DBoundingBox {
    C3DVector MinCoord;
    C3DVector MaxCoord;
};

class CPhysicsMesh {
public:
    unsigned char  _pad00[0x84];
    C3DBoundingBox BoundingBox;                         // +0x84
    bool HasBoundingBoxIntersectedFromTo(float radius, const C3DVector& from, const C3DVector& to) const;
};

bool CPhysicsMesh::HasBoundingBoxIntersectedFromTo(float radius, const C3DVector& from, const C3DVector& to) const
{
    C3DVector centre = (BoundingBox.MinCoord + BoundingBox.MaxCoord) * 0.5f;
    C3DVector halfSize = (BoundingBox.MaxCoord - BoundingBox.MinCoord) * 0.5f;
    C3DVector delta = to - from;
    float dx = (float)fabs(delta.X) + 50.0f;
    float dy = (float)fabs(delta.Y) + 50.0f;
    if (from.X - radius - dx < centre.X + halfSize.X &&
        from.Y - radius - dy < centre.Y + halfSize.Y &&
        from.X + radius + dx > centre.X - halfSize.X &&
        from.Y + radius + dy > centre.Y - halfSize.Y)
        return true;
    return false;
}
