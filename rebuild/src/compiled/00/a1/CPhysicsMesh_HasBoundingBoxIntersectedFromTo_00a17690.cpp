// CPhysicsMesh::HasBoundingBoxIntersectedFromTo(float radius, const C3DVector& from, const C3DVector& to) const -- 2D (X/Y) overlap test between the mesh's bounding box and the box around `from` grown by the radius plus the from-to distance (+50) on each axis
// Leaf: ?HasBoundingBoxIntersectedFromTo@CPhysicsMesh@@QBE_NMABVC3DVector@@0@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A17690, 260 bytes, `ret 0Ch`. PDB: ?HasBoundingBoxIntersectedFromTo@CPhysicsMesh@@QBE_NMABVC3DVector@@0@Z.
//   Called from 0x00A120C1, 0x00A18773, 0x00A18803.
// Constants: 0x0122F59C = 0.5f, 0x0124E7F8 = 50.0f.
// Layout: Ego_r.pdb CPhysicsMesh with the retail container sizes (12-byte vectors/CArrays, 0x10 CVectorMaps): BoundingBox
//   (C3DBoundingBox, Min then Max) at retail +0x84 (PDB +0xA0).
// Fix (chunk 755): the box through the inline GetBoundingBox() reference + the retail C3DBoundingBox::GetCenter gives retail's Min.X-first centre.
// Shape: centre, half-size and delta are inline C3DVector operator expressions (each pair stays on the x87 stack; the dead Z
//   lanes are dropped but centre/half-size keep their 12-byte frame slots); the two padded deltas are named float locals
//   homed in the dead `to` / `from` argument slots; four sequential tests with one shared `xor al,al`
//   exit (`if (...) return true; return false;`).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DBOUNDINGBOX
#include <math.h>
#include "engine/C3DBoundingBox.h"
#pragma intrinsic(fabs)

class CPhysicsMesh {
public:
    bool HasBoundingBoxIntersectedFromTo(float radius, const C3DVector& from, const C3DVector& to) const;
    const C3DBoundingBox& GetBoundingBox() const { return BoundingBox; }   // Ego_r ?GetBoundingBox@CPhysicsMesh@@QBEABVC3DBoundingBox@@XZ (inline)
private:
    unsigned char  _pad00[0x84];
    C3DBoundingBox BoundingBox;                         // +0x84
};

bool CPhysicsMesh::HasBoundingBoxIntersectedFromTo(float radius, const C3DVector& from, const C3DVector& to) const
{
    const C3DBoundingBox& box = GetBoundingBox();
    C3DVector centre = box.GetCenter();
    C3DVector halfSize = (box.PeekMaxCoord() - box.PeekMinCoord()) * 0.5f;
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