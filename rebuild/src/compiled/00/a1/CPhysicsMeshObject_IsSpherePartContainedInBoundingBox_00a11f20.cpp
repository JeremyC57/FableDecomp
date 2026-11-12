// CPhysicsMeshObject::IsSpherePartContainedInBoundingBox(const C3DVector& pos, float radius) const -- whether any part of a world-space sphere lies inside the mesh's bounding box
// Leaf: ?IsSpherePartContainedInBoundingBox@CPhysicsMeshObject@@QBE_NABVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A11F20, 428 bytes, __fastcall, `ret 8`, frame 0x40, no SEH.
// What it does: scales the radius into mesh space (radius * OO_Scale, +0x3C, computed first and kept at [esp+0]), inverts the
//   object's PositionMatrix (+0x08) into a local CMatrix3x4, takes the sphere centre into mesh space through that inverse, and asks
//   the mesh (Mesh +0x04, raw pointer through CIVCountedPointer::operator->, no AddRef) whether its bounding box is intersected by
//   the sphere travelling from that point to itself.
// Callees: 0x00A17690 CPhysicsMesh::HasBoundingBoxIntersectedFromTo(float, const C3DVector&, const C3DVector&) const (out of line,
//   another TU; Ego_r.pdb + FableWin.pdb). Constant 0x0122DED8 = 1.0f (the reciprocal of the determinant).
// Inline in retail (FableWin.pdb names, no retail out-of-line copy): CMatrix3x4::SetInverse(const CMatrix3x4&) -- the three
//   cofactors shared with the determinant are computed once (0x00A11F2D..55), 1/det is homed in the dead `radius` argument slot
//   (0x00A11F70), E11/E21/E31/E12/E22/E32/E43 are stored and E13/E23/E33/E41/E42 stay on the x87 stack -- and the two-argument
//   CMatrix3x4::TransformPoint(const C3DVector&, C3DVector&) (0x00A1205C..BD).
// Shape notes: the frame is exactly scaled radius (4) + local point (12) + inverse (48) = 0x40, so the inverse is a named local
//   filled in place, not a returned temporary. The transform has to be the TransformPoint member written as one plain sum per
//   component (FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS): VC7.1 picks the retail term order (X, Z, Y; and E3k, E2k, E1k in
//   the inverse's translation) by itself from that form, while the same sums written in the row, an operator* returning a C3DVector,
//   or the accumulated header form each leave the first two terms of the determinant / E43 / Z-row sums swapped.
//   The scaled radius is a separate local: `radius *= OO_Scale` keeps it in the argument slot and moves 1/det to [esp+0].
// Names: Ego_r.pdb + FableWin.pdb ?IsSpherePartContainedInBoundingBox@CPhysicsMeshObject@@QBE_NABVC3DVector@@M@Z; parameter names
//   as the shared header declares them (pos, radius). Local names are descriptive.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_BOUNDINGBOXCONTAINS
#define FABLE_RETAIL_CPHYSICSMESHOBJECT
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_MATRIX
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS
#define FABLE_RETAIL_CPHYSICSMESH_BOUNDINGBOXINTERSECTED
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CPhysicsMesh.h"
#include "engine/CPhysicsMeshObject.h"

bool CPhysicsMeshObject::IsSpherePartContainedInBoundingBox(const C3DVector& pos, float radius) const
{
    float scaled_radius = radius * OO_Scale;
    CMatrix3x4 inverse;
    inverse.SetInverse(PositionMatrix);
    C3DVector local;
    inverse.TransformPoint(pos, local);
    return Mesh->HasBoundingBoxIntersectedFromTo(scaled_radius, local, local);
}