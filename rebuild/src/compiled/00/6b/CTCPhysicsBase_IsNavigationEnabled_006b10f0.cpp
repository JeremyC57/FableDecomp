// CTCPhysicsBase::IsNavigationEnabled() const -- returns false
// Leaf: ?IsNavigationEnabled@CTCPhysicsBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B10F0, thiscall, `ret`.
// Identity: slot 41 of CTCPhysicsRigidBody's vtable 0x0126996C (defined here: the slot holds this body in 5 vtable(s), all of CTCPhysicsBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 38 TLC-named slots of that vtable puts IsNavigationEnabled(void) at this slot; Ego_r/FableWin name set: ?IsNavigationEnabled@CTCPhysicsBase@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCPhysicsBase.h (opt-ins as in the landed row CTCPhysicsBase_UpdateInterpolation_006af780.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/CTCPhysicsBase.h"
bool CTCPhysicsBase::IsNavigationEnabled() const
{
    return false;
}
