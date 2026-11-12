// CTCPhysicsBase::IsAffectedByGravity() const -- returns false
// Leaf: ?IsAffectedByGravity@CTCPhysicsBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B1140, thiscall, `ret`.
// Identity: slot 48 of CTCPhysicsBase's vtable 0x0125D2C4 (defined here: the slot holds this body in 1 vtable(s), all of CTCPhysicsBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 32 TLC-named slots of that vtable puts IsAffectedByGravity(void) at this slot; Ego_r/FableWin name set: ?IsAffectedByGravity@CTCPhysicsBase@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCPhysicsBase.h (opt-ins as in the landed row CTCPhysicsBase_UpdateInterpolation_006af780.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/CTCPhysicsBase.h"
bool CTCPhysicsBase::IsAffectedByGravity() const
{
    return false;
}
