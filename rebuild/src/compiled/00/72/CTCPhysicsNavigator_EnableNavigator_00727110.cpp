// CTCPhysicsNavigator::EnableNavigator(bool) -- stores Navigating
// Leaf: ?EnableNavigator@CTCPhysicsNavigator@@UAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00727110, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 40 of CTCPhysicsNavigator's vtable 0x01266314 (the slot holds this body in 1 vtable(s), owner CTCPhysicsNavigator); the Xbox 360 Test PDB virtual order (third source) aligned on 46 TLC-named slots of that vtable puts EnableNavigator(bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?EnableNavigator@CTCPhysicsNavigator@@UAEX_N@Z; `ret` matches the signature.
// Was: CTCPhysicsNavigator::vfunc_40 (rebuild/src/compiled/00/72/gapscan_sub00727110_00727110.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSNAVIGATOR
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsNavigator.h"
void CTCPhysicsNavigator::EnableNavigator(bool enable)
{
    Navigating = enable;
}
