// CPhysicsMeshObject::GetNavigationLevelAt(const C3DVector& pos) const -- the navigation level of the mesh under a world position:
// 0 when the mesh has no navigation containment volumes, else the position taken into mesh space through the inverse of the
// object's position matrix and handed to CPhysicsMesh::GetNavigationLevelAt.
// Leaf: ?GetNavigationLevelAt@CPhysicsMeshObject@@QBEJABVC3DVector@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) 0x00A11D10, 422 bytes (__fastcall, `ret 4`, frame `sub esp,40h`, no saved registers);
// 1.0f @ 0x0122DED8 (`fdivr`).
// Identity: Ego_r.pdb / FableWin.pdb ?GetNavigationLevelAt@CPhysicsMeshObject@@QBEJABVC3DVector@@@Z (returns long, const).
// Callee: 0x00A73D10 long CPhysicsMesh::GetNavigationLevelAt(const C3DVector&) const (another TU; ecx = Mesh).
// Inlines (FableWin.pdb names, no retail out-of-line copy): CIVCountedPointer<const CPhysicsMesh>::operator->,
// CPhysicsMesh::GetNavigationContainmentVolumes() + STLport vector::empty() (`mov edx,[ecx+54h] ; cmp edx,[ecx+58h]`),
// CMatrix3x4::SetInverse (cofactors over the determinant, reciprocal kept at [esp], then the translation through the result) and
// the two-argument CMatrix3x4::TransformPoint(const C3DVector&, C3DVector&) (three plain row sums).
// Shape: frame 0x40 = the reciprocal [esp+0], the mesh-space point [esp+4] (a named local filled through TransformPoint's
// reference, passed by address) and the inverse matrix [esp+10h]; E13/E23/E33/E41/E42 of the inverse never reach memory (x87
// stack). It is the two-argument TransformPoint on a named C3DVector that gives retail's 1,3,2 term order in the determinant and
// in the stack-held translation sum (0x00A11D4E, 0x00A11E29): with `inverse * pos` (operator*) the same instructions come out
// with those two sums as 3,1,2. The debug copy (FableWin 0x031D2480, 113 bytes / 39 instructions) fits this statement list
// (39 instructions by count; the operator* form counts 38).
// The positive `if` puts `xor eax,eax` at the tail (0x00A11EAE) behind a near `je`; the early-return spelling inverts it.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CPHYSICSMESHOBJECT
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_MATRIX
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_NAVIGATION
#define FABLE_RETAIL_CPHYSICSMESH_NAVIGATION
#include "engine/CPhysicsMesh.h"
#include "engine/CPhysicsMeshObject.h"
long CPhysicsMeshObject::GetNavigationLevelAt(const C3DVector& pos) const
{
    if (!Mesh->GetNavigationContainmentVolumes().empty()) {
        CMatrix3x4 inverse;
        inverse.SetInverse(PositionMatrix);
        C3DVector local_pos;
        inverse.TransformPoint(pos, local_pos);
        return Mesh->GetNavigationLevelAt(local_pos);
    }
    return 0;
}