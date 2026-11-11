// CTCPhysicsStandard::SetAsPushableByHero(bool) -- stores PushableByHero
// Leaf: ?SetAsPushableByHero@CTCPhysicsStandard@@UAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00724D00, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 84 of CTCPhysicsNavigator's vtable 0x01266314 (the slot holds this body in 3 vtable(s), owner CTCPhysicsStandard); the Xbox 360 Test PDB virtual order (third source) aligned on 46 TLC-named slots of that vtable puts SetAsPushableByHero(bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetAsPushableByHero@CTCPhysicsStandard@@UAEX_N@Z; `ret` matches the signature.
// Was: CTCPhysicsStandard::vfunc_84 (rebuild/src/compiled/00/72/CTCPhysicsStandard_vfunc84_00724d00.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_OVERRIDES
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_FLAGS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsStandard.h"
void CTCPhysicsStandard::SetAsPushableByHero(bool pushable)
{
    PushableByHero = pushable;
}
