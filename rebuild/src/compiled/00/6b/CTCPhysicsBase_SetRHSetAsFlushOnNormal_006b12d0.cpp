// CTCPhysicsBase::SetRHSetAsFlushOnNormal(C3DVector const &) -- does nothing
// Leaf: ?SetRHSetAsFlushOnNormal@CTCPhysicsBase@@UAEXABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B12D0, thiscall, `ret 4`.
// Identity: slot 77 of CTCPhysicsRigidBody's vtable 0x0126996C (defined here: the slot holds this body in 3 vtable(s), all of CTCPhysicsBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 38 TLC-named slots of that vtable puts SetRHSetAsFlushOnNormal(C3DVector const &) at this slot; Ego_r/FableWin name set: ?SetRHSetAsFlushOnNormal@CTCPhysicsBase@@UAEXABVC3DVector@@@Z; `ret` matches the signature.
// Header: engine/CTCPhysicsBase.h (opt-ins as in the landed row CTCPhysicsBase_UpdateInterpolation_006af780.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/CTCPhysicsBase.h"
void CTCPhysicsBase::SetRHSetAsFlushOnNormal(C3DVector const &)
{
}
