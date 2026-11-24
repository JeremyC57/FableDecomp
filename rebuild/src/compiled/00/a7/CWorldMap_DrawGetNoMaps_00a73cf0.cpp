// CPhysicsMesh::GetNumberOfNavigationLineLevels() const -- how many navigation line levels the mesh holds: the element count of
// NavigationLines (CArray<CArray<C2DLineF> >, one line list per level; STLport vector size, (finish - start) / 12)
// Leaf: ?GetNumberOfNavigationLineLevels@CPhysicsMesh@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A73CF0, 25 bytes, thiscall, plain `ret`; no callees.
// Identity (NF394, T3 nf391 q9): (1) Ego_r.pdb ?GetNumberOfNavigationLineLevels@CPhysicsMeshObject@@QBEJXZ 0x00B80500 (8 bytes, `mov ecx,
//   [ecx+4] ; jmp 0x00C12630`) tail-jumps to ?GetNumberOfNavigationLineLevels@CPhysicsMesh@@QBEJXZ 0x00C12630 (lib_physics_mesh_2_
//   navigation.obj); retail 0x00A127C0 is the same 8-byte forwarder and its jump target is this address (the only retail reference).
//   (2) FableWin.pdb 0x033C7680 ?GetNumberOfNavigationLineLevels@CPhysicsMesh@@QBEJXZ calls ?size@?$vector@V?$CArray@VC2DLineF@@@@..@@QBEIXZ
//   on the member; Ego_r.pdb types CPhysicsMesh::NavigationLines CArray<CArray<C2DLineF> >, which the shared header places at +0x48
//   (retail STLport 12-byte vectors), the offset this body reads. Both PDBs: public, const, returns long.
// NF394: was the propagated label "CWorldMap::DrawGetNoMaps" over a local CWorldMap model (?DrawGetNoMaps@CWorldMap@@QAEJXZ, also defined
//   by 0x004FB4B0; the PDB DrawGetNoMaps is the virtual ?DrawGetNoMaps@CWorldMap@@UBEJXZ). Model: the shared engine/CPhysicsMesh.h with
//   the NF394 opt-in FABLE_RETAIL_CPHYSICSMESH_NAVIGATIONLINELEVELS (the typed member + this declaration).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CPHYSICSMESH_NAVIGATIONLINELEVELS
#include "engine/CPhysicsMesh.h"

long CPhysicsMesh::GetNumberOfNavigationLineLevels() const
{
    return NavigationLines.size();
}
