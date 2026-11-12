// CTCPhysicsBase::GetFacingAngleXZ() const -- returns 0.0f (the base physics body has no facing)
// Leaf: ?GetFacingAngleXZ@CTCPhysicsBase@@UBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B1250, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 65 of CTCPhysicsLight's vtable 0x01265DFC (the slot holds this body in 2 vtable(s), owner CTCPhysicsBase); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts GetFacingAngleXZ(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetFacingAngleXZ@CTCPhysicsBase@@UBEMXZ; `ret` matches the signature.
// Was: CTCPhysicsBase::vfunc_65 (rebuild/src/compiled/00/6b/CTCPhysicsBase_vfunc65_006b1250.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsBase.h"
float CTCPhysicsBase::GetFacingAngleXZ() const
{
    return 0.0f;
}
