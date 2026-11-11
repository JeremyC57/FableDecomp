// CTCPhysicsStandard::IsAffectedByWind() const -- returns false
// Leaf: ?IsAffectedByWind@CTCPhysicsStandard@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00724C20, thiscall, `ret`.
// Identity: slot 50 of CTCPhysicsNavigator's vtable 0x01266314 (defined here: the slot holds this body in 3 vtable(s), all of CTCPhysicsStandard or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 46 TLC-named slots of that vtable puts IsAffectedByWind(void) at this slot; Ego_r/FableWin name set: ?IsAffectedByWind@CTCPhysicsStandard@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCPhysicsStandard.h (opt-ins as in the landed row CTCPhysicsStandard_ResetInterpolation_007236e0.cpp).
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_VSLOTS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsStandard.h"
#include "engine/CWorld.h"
bool CTCPhysicsStandard::IsAffectedByWind() const
{
    return false;
}
