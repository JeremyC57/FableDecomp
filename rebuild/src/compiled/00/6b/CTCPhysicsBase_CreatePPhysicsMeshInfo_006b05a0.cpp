// CTCPhysicsBase::CreatePPhysicsMeshInfo(const CEngineGraphic& graphic) -- look the graphic's static (type 5) or animated (type 4)
// mesh info up in the world's main mesh bank; when the mesh has a physics mesh, hand its bank index to SetPhysicsMeshBankIndex,
// otherwise fall back to the physics def's interaction flags (SetInteractionFlags). Other graphic types do nothing.
// Leaf: ?CreatePPhysicsMeshInfo@CTCPhysicsBase@@IAEXABVCEngineGraphic@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B05A0, 110 bytes.
// Callees: CTCBase::PeekWorld 0x00686A80; CMeshDataBank::PeekStaticMesh 0x00A24280 / PeekAnimatedMesh 0x00A242B0;
//   C3DMeshInfo::HasPhysicsMesh 0x00A23C40 / PeekPhysicsMesh 0x00A24A70 (all Ego_r.pdb names, retail bodies);
//   virtual SetPhysicsMeshBankIndex (slot 85, +0x154) and SetInteractionFlags (slot 43, +0xac).
// Layout: CWorld PMainMeshBank +0x3c; CEngineGraphic BankIndex +0 / Type +0xd; CPhysicsMeshInfo : CMeshDataBankEntry (RTTI),
//   Index +8; CTCPhysicsBase PhysicsDef +0x38 (CDefPointer<const CPhysicsDef>), CPhysicsDef InteractionFlags +0x3c.
// Shape: the body after the mesh lookup is written out once per graphic type. Retail sets `this` up before pushing the argument at
//   both virtual calls (`mov edx,[esi] ; mov ecx,esi ; push eax`, an order found at 12 of the 1,942 register-object virtual call
//   sites of Fable.exe); a single shared tail gives the usual `mov edx,[esi] ; push eax ; mov ecx,esi`. With the tail in both
//   arms VC7.1 emits the two copies, merges them from the end (the `jmp` after PeekStaticMesh lands on the shared `mov edi,eax`),
//   and the merged code keeps the this-first order. Retail bytes: one copy of the tail, both lookups in front of it.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_PEEKWORLD
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_COLLISION_RESPONSE
#define FABLE_RETAIL_CTCPHYSICSBASE_CREATEPPHYSICSMESHINFO
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_C3DMESHINFO_PHYSICSMESH
#define FABLE_RETAIL_CMESHDATABANK
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANK_PEEKSTATICANIMATED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESH_BANK
#define FABLE_RETAIL_CWORLD_PEEKMESHBANK
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/CMeshDataBank.h"
#include "engine/CMeshDataBankEntry.h"
#include "engine/C3DMeshInfo.h"
#include "engine/CEngineGraphic.h"

// No engine header: RTTI .?AVCPhysicsMeshInfo@@ : CMeshDataBankEntry; only the base's Index is read here.
class CPhysicsMeshInfo : public CMeshDataBankEntry {
};

void CTCPhysicsBase::CreatePPhysicsMeshInfo(const CEngineGraphic& graphic)
{
    const CMeshDataBank* bank = PeekWorld().PeekMeshBank();
    if (graphic.Type == 5) {
        const C3DMeshInfo* info = bank->PeekStaticMesh(graphic.BankIndex);
        if (info->HasPhysicsMesh()) {
            SetPhysicsMeshBankIndex(info->PeekPhysicsMesh()->GetIndex());
            return;
        }
        if (!PhysicsDef.IsNull())
            SetInteractionFlags(PhysicsDef->InteractionFlags);
    } else if (graphic.Type == 4) {
        const C3DMeshInfo* info = bank->PeekAnimatedMesh(graphic.BankIndex);
        if (info->HasPhysicsMesh()) {
            SetPhysicsMeshBankIndex(info->PeekPhysicsMesh()->GetIndex());
            return;
        }
        if (!PhysicsDef.IsNull())
            SetInteractionFlags(PhysicsDef->InteractionFlags);
    }
}