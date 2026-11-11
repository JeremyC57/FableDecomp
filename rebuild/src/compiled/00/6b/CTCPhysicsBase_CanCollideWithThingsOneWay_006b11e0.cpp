// CTCPhysicsBase::CanCollideWithThingsOneWay() const -- returns false
// Leaf: ?CanCollideWithThingsOneWay@CTCPhysicsBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B11E0, thiscall, `ret`.
// Identity: slot 58 of CTCPhysicsLight's vtable 0x01265DFC (defined here: the slot holds this body in 2 vtable(s), all of CTCPhysicsBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts CanCollideWithThingsOneWay(void) at this slot; Ego_r/FableWin name set: ?CanCollideWithThingsOneWay@CTCPhysicsBase@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCPhysicsBase.h (opt-ins as in the landed row CTCPhysicsBase_UpdateInterpolation_006af780.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/CTCPhysicsBase.h"
bool CTCPhysicsBase::CanCollideWithThingsOneWay() const
{
    return false;
}
