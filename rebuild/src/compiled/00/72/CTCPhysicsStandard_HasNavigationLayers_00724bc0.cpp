// CTCPhysicsStandard::HasNavigationLayers() const -- returns NavigationLayers
// Leaf: ?HasNavigationLayers@CTCPhysicsStandard@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00724BC0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 42 of CTCPhysicsNavigator's vtable 0x01266314 (the slot holds this body in 3 vtable(s), owner CTCPhysicsStandard); the Xbox 360 Test PDB virtual order (third source) aligned on 46 TLC-named slots of that vtable puts HasNavigationLayers(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?HasNavigationLayers@CTCPhysicsStandard@@UBE_NXZ; `ret` matches the signature.
// Was: CTCPhysicsStandard::vfunc_42 (rebuild/src/compiled/00/72/CTCPhysicsStandard_vfunc42_00724bc0.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_OVERRIDES
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_FLAGS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsStandard.h"
bool CTCPhysicsStandard::HasNavigationLayers() const
{
    return NavigationLayers;
}
