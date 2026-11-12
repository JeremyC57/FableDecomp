// CTCPhysicsBase::SetPhysicsMeshBankIndex(long) -- does nothing
// Leaf: ?SetPhysicsMeshBankIndex@CTCPhysicsBase@@UAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B1340, thiscall, `ret 4`.
// Identity: slot 85 of CTCPhysicsLight's vtable 0x01265DFC (defined here: the slot holds this body in 2 vtable(s), all of CTCPhysicsBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts SetPhysicsMeshBankIndex(long) at this slot; Ego_r/FableWin name set: ?SetPhysicsMeshBankIndex@CTCPhysicsBase@@UAEXJ@Z; `ret` matches the signature.
// Header: engine/CTCPhysicsBase.h (opt-ins as in the landed row CTCPhysicsBase_UpdateInterpolation_006af780.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/CTCPhysicsBase.h"
void CTCPhysicsBase::SetPhysicsMeshBankIndex(long)
{
}
