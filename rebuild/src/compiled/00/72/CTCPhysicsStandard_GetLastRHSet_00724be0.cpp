// CTCPhysicsStandard::GetLastRHSet() const -- returns OldRHSet
// Leaf: ?GetLastRHSet@CTCPhysicsStandard@@UBEABVCRightHandedSet@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00724BE0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 73 of CTCPhysicsNavigator's vtable 0x01266314 (the slot holds this body in 3 vtable(s), owner CTCPhysicsStandard); the Xbox 360 Test PDB virtual order (third source) aligned on 46 TLC-named slots of that vtable puts GetLastRHSet(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetLastRHSet@CTCPhysicsStandard@@UBEABVCRightHandedSet@@XZ; `ret` matches the signature.
// Was: CTCPhysicsStandard::vfunc_73 (rebuild/src/compiled/00/72/gapscan_sub00724be0_00724be0.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_OVERRIDES
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_FLAGS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsStandard.h"
const CRightHandedSet& CTCPhysicsStandard::GetLastRHSet() const
{
    return OldRHSet;
}
