// CTCPhysicsBase::IsAffectedByAir() const -- returns false
// Leaf: ?IsAffectedByAir@CTCPhysicsBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B1150, thiscall, `ret`.
// Identity: slot 49 of CTCPhysicsRigidBody's vtable 0x0126996C (defined here: the slot holds this body in 3 vtable(s), all of CTCPhysicsBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 38 TLC-named slots of that vtable puts IsAffectedByAir(void) at this slot; Ego_r/FableWin name set: ?IsAffectedByAir@CTCPhysicsBase@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCPhysicsBase.h (opt-ins as in the landed row CTCPhysicsBase_UpdateInterpolation_006af780.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/CTCPhysicsBase.h"
bool CTCPhysicsBase::IsAffectedByAir() const
{
    return false;
}
