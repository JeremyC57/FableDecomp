// CPhysicsMeshObject::IsPointContainedInBoundingBox(const C3DVector& pos) const -- the point test is the sphere test with a zero radius
// Leaf: ?IsPointContainedInBoundingBox@CPhysicsMeshObject@@QBE_NABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A12890, 15 bytes: `mov eax,[esp+4] ; push 0 ; push eax ; call 0x00A11F20 ; ret 4` (ecx, the mesh
//   object, passed straight through).
// Names (agentNF27, 2026-11-12; was CMainGameComponent::BeginInputLoading, a propagated manifest label, with the callee as the free
//   stand-in bil_helper_00a11f20): Ego_r.pdb + FableWin.pdb ?IsPointContainedInBoundingBox@CPhysicsMeshObject@@QBE_NABVC3DVector@@@Z, and
//   the landed callers 0x0063F150 / 0x0063F1E0 call it as that with a CPhysicsMeshObject `this` (the GetPhysicsMeshObject 0x006B0850
//   result) and the filter's point. Callee 0x00A11F20 = Ego_r.pdb / FableWin.pdb
//   ?IsSpherePartContainedInBoundingBox@CPhysicsMeshObject@@QBE_NABVC3DVector@@M@Z: its body scales the float argument by OO_Scale (+0x3C)
//   and takes the point into mesh space through the inverse PositionMatrix (+0x08..+0x34) before the mesh test.
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_BOUNDINGBOXCONTAINS
#include "engine/C3DVector.h"
#include "engine/CPhysicsMeshObject.h"

bool CPhysicsMeshObject::IsPointContainedInBoundingBox(const C3DVector& pos) const
{
    return IsSpherePartContainedInBoundingBox(pos, 0.0f);
}
