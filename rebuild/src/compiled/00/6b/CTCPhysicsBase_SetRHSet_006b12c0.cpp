// CTCPhysicsBase::SetRHSet(CRightHandedSet const &) -- does nothing
// Leaf: ?SetRHSet@CTCPhysicsBase@@UAEXABVCRightHandedSet@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B12C0, thiscall, `ret 4`.
// Identity: slot 75 of CTCPhysicsLight's vtable 0x01265DFC (defined here: the slot holds this body in 2 vtable(s), all of CTCPhysicsBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts SetRHSet(CRightHandedSet const &) at this slot; Ego_r/FableWin name set: ?SetRHSet@CTCPhysicsBase@@UAEXABVCRightHandedSet@@@Z; `ret` matches the signature.
// Header: engine/CTCPhysicsBase.h (opt-ins as in the landed row CTCPhysicsBase_UpdateInterpolation_006af780.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/CTCPhysicsBase.h"
void CTCPhysicsBase::SetRHSet(CRightHandedSet const &)
{
}
