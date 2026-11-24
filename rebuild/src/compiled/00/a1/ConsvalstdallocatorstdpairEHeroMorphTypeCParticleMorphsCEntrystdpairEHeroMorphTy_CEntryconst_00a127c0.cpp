// CPhysicsMeshObject::GetNumberOfNavigationLineLevels() const -- the mesh's navigation line level count (forwarded to the counted mesh)
// Leaf: ?GetNumberOfNavigationLineLevels@CPhysicsMeshObject@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A127C0, 8 bytes: `mov ecx,[ecx+4] ; jmp 0x00A73CF0` (Mesh at +4, tail call).
// Identity (NF394, T3 nf391 q9): (1) Ego_r.pdb ?GetNumberOfNavigationLineLevels@CPhysicsMeshObject@@QBEJXZ 0x00B80500 is the same 8-byte
//   forwarder into ?GetNumberOfNavigationLineLevels@CPhysicsMesh@@QBEJXZ 0x00C12630, and Ego_r's CThingPhysical::GetNoNavCollisionLineLists
//   0x00683F90 calls it at the instruction where retail 0x006A4F10 calls this address (0x006A4F37). (2) FableWin.pdb
//   ?GetNoNavCollisionLineLists@CThingPhysical@@QBEJXZ (thing_physical.obj) calls ?GetNumberOfNavigationLineLevels@CPhysicsMeshObject@@QBEJXZ
//   on the GetPhysicsMeshObject temporary, as does CTCGraphicAppearance::DrawNavigationMesh (retail callers 0x004BD4C4 / 0x004BD5FB, in
//   the CTCGraphicAppearance block). Both PDBs: public, const, returns long.
// NF394: was the stand-in ?Run@T@@QAEXXZ (a pad struct forwarding to ?Run_00a73cf0@Sub@@QAEXXZ, catalog label
//   "_global::Forward_DrawGetNoMaps_via0x4_00a127c0"). Model: the shared engine/CPhysicsMeshObject.h (FABLE_RETAIL_CPHYSICSMESHOBJECT: Mesh
//   typed CIVCountedPointer<const CPhysicsMesh>; operator-> inline) with the NF394 opt-ins declaring both members.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CPHYSICSMESH_NAVIGATIONLINELEVELS
#define FABLE_RETAIL_CPHYSICSMESHOBJECT
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_NAVIGATIONLINELEVELS
#include "engine/CPhysicsMesh.h"
#include "engine/CPhysicsMeshObject.h"

long CPhysicsMeshObject::GetNumberOfNavigationLineLevels() const
{
    return Mesh->GetNumberOfNavigationLineLevels();
}
