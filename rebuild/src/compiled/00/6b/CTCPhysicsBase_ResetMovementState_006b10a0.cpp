// CTCPhysicsBase::ResetMovementState() -- does nothing
// Leaf: ?ResetMovementState@CTCPhysicsBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B10A0, thiscall, `ret`.
// Identity: slot 35 of CTCPhysicsRigidBody's vtable 0x0126996C (defined here: the slot holds this body in 5 vtable(s), all of CTCPhysicsBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 38 TLC-named slots of that vtable puts ResetMovementState(void) at this slot; Ego_r/FableWin name set: ?ResetMovementState@CTCPhysicsBase@@UAEXXZ; `ret` matches the signature.
// Header: engine/CTCPhysicsBase.h (opt-ins as in the landed row CTCPhysicsBase_UpdateInterpolation_006af780.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/CTCPhysicsBase.h"
void CTCPhysicsBase::ResetMovementState()
{
}
