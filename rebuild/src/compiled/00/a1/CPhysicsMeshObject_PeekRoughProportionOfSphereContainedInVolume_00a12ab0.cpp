// CPhysicsMeshObject::PeekRoughProportionOfSphereContainedInVolume(const CCharString& name, const C3DVector& pos, float radius) const -- rough fraction of a world-space sphere that lies inside the mesh's named containment volume
// Leaf: ?PeekRoughProportionOfSphereContainedInVolume@CPhysicsMeshObject@@QBEMABVCCharString@@ABVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A12AB0, 468 bytes, __fastcall, `ret 0Ch`, frame 0x40, no SEH. Flags: none.
// What it does: inverts the object's PositionMatrix (+0x08) into a local CMatrix3x4, takes the sphere centre into mesh space
//   through that inverse, scales the radius into mesh space (radius * OO_Scale, +0x3C), takes a counted copy of the mesh pointer
//   (Mesh +0x04: `inc [esi+4]` when non-null) and returns the mesh's rough proportion of that mesh-space sphere inside the named
//   containment volume; the counted copy is released on the way out (`dec [esi+4]`, Delete through vtable slot 1 at zero) with the
//   float result kept at [esp+4Ch] across the Delete call.
// Callees: 0x00A6CA80 CPhysicsMesh::PeekRoughProportionOfSphereContained(const CCharString&, const C3DVector&, float) const (out of
//   line, another TU, landed; Ego_r.pdb + FableWin.pdb); vtable slot 1 (+4) of the pointee = CIVCountedPointeeBase::Delete() const
//   (0x0042D930), through the inline CIVCountedPointer destructor. Constant 0x0122DED8 = 1.0f (the reciprocal of the determinant).
// Inline in retail (FableWin.pdb names, no retail out-of-line copy): CMatrix3x4::SetInverse(const CMatrix3x4&), the two-argument
//   CMatrix3x4::TransformPoint(const C3DVector&, C3DVector&) const (plain row sums), the CIVCountedPointer copy constructor,
//   operator-> and destructor.
// Shape notes: the same body as the twin 0x00A128E0 IsInContainmentVolume (frame = 1/det (4) + mesh-space point (12) + inverse (48)
//   = 0x40; the scaled radius is a separate local declared AFTER the transform and BEFORE the counted copy, homed by VC7.1 in the
//   dead `pos` argument slot [esp+4Ch], which the float result then reuses). One thing is specific to the float result: retail
//   stores it AFTER the release's null test (`call ; test esi,esi ; fst [esp+4Ch] ; je`, 0x00A12C63..69 -- the only such site in
//   Fable.exe). A plain `return mesh->Peek...(...)` (and every named-result / scope / GetMesh() spelling of it) stores first
//   (`call ; fst ; test ; je`), because the call's result is stored the moment it is produced. VC7.1 only lets the integer test
//   overtake the store when an x87 operation sits between the call and the store (compare landed 0x006F9130: `call ; fdiv ; mov ;
//   test ; fstp ; je`). Here that operation emits no instruction, so it is a float -> double -> float round trip of the result:
//   the result is used in a double-precision expression that folds away. `* 1.0` is the spelling that reproduces it; the exact
//   original double expression cannot be recovered from the bytes (`* 1.0f` and `(float)(double)` casts fold too early and leave
//   the store first; a named `double` local gets the tail right but reorders the cofactor products).
// Names: Ego_r.pdb + FableWin.pdb ?PeekRoughProportionOfSphereContainedInVolume@CPhysicsMeshObject@@QBEMABVCCharString@@ABVC3DVector@@M@Z;
//   no PDB parameter names (name / pos / radius follow the sibling declarations in the shared header). Local names are descriptive.
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

float CPhysicsMeshObject::PeekRoughProportionOfSphereContainedInVolume(const CCharString& name, const C3DVector& pos, float radius) const
{
    CMatrix3x4 inverse;
    inverse.SetInverse(PositionMatrix);
    C3DVector local;
    inverse.TransformPoint(pos, local);
    float scaled_radius = radius * OO_Scale;
    CIVCountedPointer<const CPhysicsMesh> mesh(Mesh);
    return (float)(mesh->PeekRoughProportionOfSphereContained(name, local, scaled_radius) * 1.0);
}