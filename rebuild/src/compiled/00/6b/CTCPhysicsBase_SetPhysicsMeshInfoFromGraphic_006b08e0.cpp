// CTCPhysicsBase::SetPhysicsMeshInfoFromGraphic(const CEngineGraphic& graphic) -- build the physics mesh info from the graphic: CreatePPhysicsMeshInfo
// Leaf: ?SetPhysicsMeshInfoFromGraphic@CTCPhysicsBase@@QAEXABVCEngineGraphic@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B08E0, 5 bytes: `jmp 0x006B05A0` (CreatePPhysicsMeshInfo, the landed body).
// NF28 (2026-11-12, queue land1011ed): was landed as an extern "C" forwarder under the label CTCPhysicsBase::CreatePPhysicsMeshInfo
//   (one address, one name: the body 0x006B05A0 already carries that name). Identity: Ego_r.pdb 0x006ED880 (egor_pdb_names, a thunk-size
//   entry before the next function) and FableWin.pdb 0x02470D30 ?SetPhysicsMeshInfoFromGraphic@CTCPhysicsBase@@QAEXABVCEngineGraphic@@@Z,
//   which FableWin places right before ?CreatePPhysicsMeshInfo@CTCPhysicsBase@@IAEXABVCEngineGraphic@@@Z 0x02470D50 (protected); the
//   public wrapper is what the outside callers 0x008EC9C0 (CTCDegradableBase::ChangeMesh) and 0x007A69F0 (ReplaceMeshByIndex) call.
// Model: engine/CTCPhysicsBase.h opt-ins FABLE_RETAIL_CTCPHYSICSBASE_CREATEPPHYSICSMESHINFO + _SETPHYSICSMESHINFOFROMGRAPHIC.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_CREATEPPHYSICSMESHINFO
#define FABLE_RETAIL_CTCPHYSICSBASE_SETPHYSICSMESHINFOFROMGRAPHIC
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsBase.h"

void CTCPhysicsBase::SetPhysicsMeshInfoFromGraphic(const CEngineGraphic& graphic)
{
    CreatePPhysicsMeshInfo(graphic);
}
