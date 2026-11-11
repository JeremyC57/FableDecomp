// CTCPhysicsRigidBody::CanInteractWithThings() const -- returns true
// Leaf: ?CanInteractWithThings@CTCPhysicsRigidBody@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075BFC0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 60 of CTCPhysicsRigidBody's vtable 0x0126996C (the slot holds this body in 1 vtable(s), owner CTCPhysicsRigidBody); the Xbox 360 Test PDB virtual order (third source) aligned on 38 TLC-named slots of that vtable puts CanInteractWithThings(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?CanInteractWithThings@CTCPhysicsRigidBody@@UBE_NXZ; `ret` matches the signature.
// Was: CTCPhysicsRigidBody::vfunc_60 (rebuild/src/compiled/00/75/gapscan_sub0075bfc0_0075bfc0.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSRIGIDBODY
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsRigidBody.h"
bool CTCPhysicsRigidBody::CanInteractWithThings() const
{
    return true;
}
