// CTCPhysicsBase::GetPhysicsMeshInfo() const -- returns 0
// Leaf: ?GetPhysicsMeshInfo@CTCPhysicsBase@@UBEPBVCThingPhysicsMeshInfo@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B13B0, thiscall, `ret`.
// Identity: slot 93 of CTCPhysicsLight's vtable 0x01265DFC (defined here: the slot holds this body in 2 vtable(s), all of CTCPhysicsBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts GetPhysicsMeshInfo(void) at this slot; Ego_r/FableWin name set: ?GetPhysicsMeshInfo@CTCPhysicsBase@@UBEPBVCThingPhysicsMeshInfo@@XZ; `ret` matches the signature.
// Header: engine/CTCPhysicsBase.h (opt-ins as in the landed row CTCPhysicsBase_UpdateInterpolation_006af780.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/CTCPhysicsBase.h"
CThingPhysicsMeshInfo const * CTCPhysicsBase::GetPhysicsMeshInfo() const
{
    return 0;
}
