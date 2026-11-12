// CTCPhysicsBase::CreatePPhysicsMeshInfo(const CEngineGraphic& graphic) -- look the graphic's static (type 5) or animated (type 4)
// Grade: DIFFER(110v110) -- only the push/`mov ecx,esi` order before both virtual calls: retail `mov edx,[esi] ; mov ecx,esi ; push eax`, ours `mov edx,[esi] ; push eax ; mov ecx,esi` (that retail order occurs 3 times in all of Fable.exe); tried a named index local, if/else-if tail, a def raw-pointer local, /EHsc, /Oa, #pragma optimize s / y
// mesh info up in the world's main mesh bank; when the mesh has a physics mesh, hand its bank index to SetPhysicsMeshBankIndex,
// otherwise fall back to the physics def's interaction flags (SetInteractionFlags).
// Leaf: ?CreatePPhysicsMeshInfo@CTCPhysicsBase@@IAEXABVCEngineGraphic@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B05A0, 110 bytes.
// Callees: CTCBase::PeekWorld 0x00686A80; CMeshDataBank::PeekStaticMesh 0x00A24280 / PeekAnimatedMesh 0x00A242B0;
//   C3DMeshInfo::HasPhysicsMesh 0x00A23C40 / PeekPhysicsMesh 0x00A24A70 (all Ego_r.pdb names, retail bodies);
//   virtual SetPhysicsMeshBankIndex (slot 85, +0x154) and SetInteractionFlags (slot 43, +0xac).
// Layout: CWorld PMainMeshBank +0x3c; CEngineGraphic BankIndex +0 / Type +0xd; CPhysicsMeshInfo : CMeshDataBankEntry (RTTI),
//   Index +8; CTCPhysicsBase PhysicsDef +0x38 (CDefPointer<const CPhysicsDef>), CPhysicsDef InteractionFlags +0x3c.
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
    const C3DMeshInfo* info;
    if (graphic.Type == 5)
        info = bank->PeekStaticMesh(graphic.BankIndex);
    else if (graphic.Type == 4)
        info = bank->PeekAnimatedMesh(graphic.BankIndex);
    else
        return;
    if (info->HasPhysicsMesh()) {
        SetPhysicsMeshBankIndex(info->PeekPhysicsMesh()->GetIndex());
        return;
    }
    if (!PhysicsDef.IsNull())
        SetInteractionFlags(PhysicsDef->InteractionFlags);
}
