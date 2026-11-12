// CTCPhysicsBase::GetInterfaceType() const -- returns TCI_PHYSICS
// Leaf: ?GetInterfaceType@CTCPhysicsBase@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B1060, thiscall, `ret`.
// Identity: slot 21 of CTCPhysicsRigidBody's vtable 0x0126996C (defined here: the slot holds this body in 6 vtable(s), all of CTCPhysicsBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 38 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot; Ego_r/FableWin name set: ?GetInterfaceType@CTCPhysicsBase@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
// Header: engine/CTCPhysicsBase.h (opt-ins as in the landed row CTCPhysicsBase_UpdateInterpolation_006af780.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/CTCPhysicsBase.h"
#include "engine/ETCInterfaceType.h"
ETCInterfaceType CTCPhysicsBase::GetInterfaceType() const
{
    return TCI_PHYSICS;
}
