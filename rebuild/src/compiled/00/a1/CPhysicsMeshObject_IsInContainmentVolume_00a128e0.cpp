// CPhysicsMeshObject::IsInContainmentVolume(const CCharString& name, const C3DVector& pos, float radius) const -- whether a world-space sphere lies inside the mesh's named containment volume
// Leaf: ?IsInContainmentVolume@CPhysicsMeshObject@@QBE_NABVCCharString@@ABVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A128E0, 464 bytes, __fastcall, `ret 0Ch`, frame 0x40, no SEH. Flags: none.
// What it does: inverts the object's PositionMatrix (+0x08) into a local CMatrix3x4, takes the sphere centre into mesh space
//   through that inverse, scales the radius into mesh space (radius * OO_Scale, +0x3C), takes a counted copy of the mesh pointer
//   (Mesh +0x04: `inc [esi+4]` when non-null) and asks the mesh whether the mesh-space sphere is in the named containment volume;
//   the counted copy is released on the way out (`dec [esi+4]`, Delete through vtable slot 1 at zero).
// Callees: 0x00A6CA20 CPhysicsMesh::IsInContainmentVolume(const CCharString&, const C3DVector&, float) const (out of line, another
//   TU, landed; Ego_r.pdb + FableWin.pdb); vtable slot 1 (+4) of the pointee = CIVCountedPointeeBase::Delete() const (0x0042D930),
//   through the inline CIVCountedPointer destructor. Constant 0x0122DED8 = 1.0f (the reciprocal of the determinant).
// Inline in retail (FableWin.pdb names, no retail out-of-line copy): CMatrix3x4::SetInverse(const CMatrix3x4&), the two-argument
//   CMatrix3x4::TransformPoint(const C3DVector&, C3DVector&) const (plain row sums), the CIVCountedPointer copy constructor,
//   operator-> and destructor.
// Shape notes: the frame is 1/det (4) + mesh-space point (12) + inverse (48) = 0x40, so the inverse is a named local filled in
//   place, as in the landed siblings 0x00A11D10 and 0x00A11F20. The scaled radius is a separate local declared AFTER the transform
//   and BEFORE the counted copy: VC7.1 homes it in the dead `pos` argument slot ([esp+50h], 0x00A12A75); `radius *= OO_Scale`
//   homes it in the `radius` slot instead ([esp+54h], the only residue of that spelling), and the product written inline at the
//   call or the GetMesh() temporary each grade 459 v 464. The counted copy is a named local constructed last, so the null test
//   of Mesh is hoisted into the translation sums (0x00A129CD..D0) and its `je` lands after the radius store (0x00A12A79).
// Names: Ego_r.pdb + FableWin.pdb ?IsInContainmentVolume@CPhysicsMeshObject@@QBE_NABVCCharString@@ABVC3DVector@@M@Z; no PDB
//   parameter names (name / pos / radius follow the sibling declarations in the shared header). Local names are descriptive.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPHYSICSMESHOBJECT
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_MATRIX
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS
#define FABLE_RETAIL_CPHYSICSMESH_CONTAINMENTVOLUME
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_CONTAINMENTVOLUME
class CCharString;
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CPhysicsMesh.h"
#include "engine/CPhysicsMeshObject.h"

bool CPhysicsMeshObject::IsInContainmentVolume(const CCharString& name, const C3DVector& pos, float radius) const
{
    CMatrix3x4 inverse;
    inverse.SetInverse(PositionMatrix);
    C3DVector local;
    inverse.TransformPoint(pos, local);
    float scaled_radius = radius * OO_Scale;
    CIVCountedPointer<const CPhysicsMesh> mesh(Mesh);
    return mesh->IsInContainmentVolume(name, local, scaled_radius);
}