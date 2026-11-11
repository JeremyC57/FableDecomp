// CTCPhysicsBase::ApplyControlledAcceleration(C3DVector const &, float) -- does nothing
// Leaf: ?ApplyControlledAcceleration@CTCPhysicsBase@@UAEXABVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B1310, thiscall, `ret 8`.
// Identity: slot 82 of CTCPhysicsRigidBody's vtable 0x0126996C (defined here: the slot holds this body in 5 vtable(s), all of CTCPhysicsBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 38 TLC-named slots of that vtable puts ApplyControlledAcceleration(C3DVector const &, float) at this slot; Ego_r/FableWin name set: ?ApplyControlledAcceleration@CTCPhysicsBase@@UAEXABVC3DVector@@M@Z; `ret` matches the signature.
// Header: engine/CTCPhysicsBase.h (opt-ins as in the landed row CTCPhysicsBase_UpdateInterpolation_006af780.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/CTCPhysicsBase.h"
void CTCPhysicsBase::ApplyControlledAcceleration(C3DVector const &, float)
{
}
