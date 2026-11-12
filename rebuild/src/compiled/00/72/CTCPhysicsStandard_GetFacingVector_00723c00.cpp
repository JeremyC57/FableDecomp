// CTCPhysicsStandard::GetFacingVector() const -- returns RHSet's forward vector
// Leaf: ?GetFacingVector@CTCPhysicsStandard@@UBE?AVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00723C00, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 70 of CTCPhysicsNavigator's vtable 0x01266314 (the slot holds this body in 3 vtable(s), owner CTCPhysicsStandard); the Xbox 360 Test PDB virtual order (third source) aligned on 46 TLC-named slots of that vtable puts GetFacingVector(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetFacingVector@CTCPhysicsStandard@@UBE?AVC3DVector@@XZ; `ret` matches the signature.
// Was: CTCPhysicsStandard::vfunc_70 (rebuild/src/compiled/00/72/CTCPhysicsStandard_vfunc70_00723c00.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_OVERRIDES
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_FLAGS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsStandard.h"
C3DVector CTCPhysicsStandard::GetFacingVector() const
{
    return RHSet.GetForward();
}
