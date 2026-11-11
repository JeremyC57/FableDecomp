// CTCPhysicsNavigator::IsNavigationEnabled() const -- returns Navigating
// Leaf: ?IsNavigationEnabled@CTCPhysicsNavigator@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00727100, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 41 of CTCPhysicsNavigator's vtable 0x01266314 (the slot holds this body in 1 vtable(s), owner CTCPhysicsNavigator); the Xbox 360 Test PDB virtual order (third source) aligned on 46 TLC-named slots of that vtable puts IsNavigationEnabled(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsNavigationEnabled@CTCPhysicsNavigator@@UBE_NXZ; `ret` matches the signature.
// Was: CTCPhysicsNavigator::vfunc_41 (rebuild/src/compiled/00/72/gapscan_sub00727100_00727100.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSNAVIGATOR
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsNavigator.h"
bool CTCPhysicsNavigator::IsNavigationEnabled() const
{
    return Navigating;
}
