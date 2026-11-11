// CTCPhysicsStandard::EnableGravity(bool) -- marks the body as moving; FeelsGravity = enable && the physics def's mass > DELTA_NOTIONAL_ZERO
// Leaf: ?EnableGravity@CTCPhysicsStandard@@UAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007235E0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 38 of CTCPhysicsNavigator's vtable 0x01266314 (the slot holds this body in 3 vtable(s), owner CTCPhysicsStandard); the Xbox 360 Test PDB virtual order (third source) aligned on 46 TLC-named slots of that vtable puts EnableGravity(bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?EnableGravity@CTCPhysicsStandard@@UAEX_N@Z; `ret` matches the signature.
// Was: CTCPhysicsStandard::vfunc_38 (rebuild/src/compiled/00/72/CTCPhysicsStandard_vfunc38_007235e0.cpp).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCPHYSICSBASE_SETASMOVING
#define FABLE_RETAIL_CTCPHYSICSBASE_GETMASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_OVERRIDES
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_FLAGS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsStandard.h"
void CTCPhysicsStandard::EnableGravity(bool enable)
{
    SetAsMoving(true);
    if (enable)
        FeelsGravity = GetMass() > DELTA_NOTIONAL_ZERO;
    else
        FeelsGravity = false;
}
