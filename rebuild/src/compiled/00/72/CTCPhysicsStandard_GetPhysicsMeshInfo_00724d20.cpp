// CTCPhysicsStandard::GetPhysicsMeshInfo() const -- returns PPhysicsMeshInfo
// Leaf: ?GetPhysicsMeshInfo@CTCPhysicsStandard@@UBEPBVCThingPhysicsMeshInfo@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00724D20, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 93 of CTCPhysicsNavigator's vtable 0x01266314 (the slot holds this body in 3 vtable(s), owner CTCPhysicsStandard); the Xbox 360 Test PDB virtual order (third source) aligned on 46 TLC-named slots of that vtable puts GetPhysicsMeshInfo(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetPhysicsMeshInfo@CTCPhysicsStandard@@UBEPBVCThingPhysicsMeshInfo@@XZ; `ret` matches the signature.
// Was: CTCPhysicsStandard::vfunc_93 (rebuild/src/compiled/00/72/gapscan_sub00724d20_00724d20.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_OVERRIDES
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_FLAGS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsStandard.h"
const CThingPhysicsMeshInfo* CTCPhysicsStandard::GetPhysicsMeshInfo() const
{
    return PPhysicsMeshInfo;
}
