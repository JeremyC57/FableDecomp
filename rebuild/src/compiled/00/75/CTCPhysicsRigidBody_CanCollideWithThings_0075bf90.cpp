// CTCPhysicsRigidBody::CanCollideWithThings() const -- returns true
// Leaf: ?CanCollideWithThings@CTCPhysicsRigidBody@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075BF90, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 57 of CTCPhysicsRigidBody's vtable 0x0126996C (the slot holds this body in 1 vtable(s), owner CTCPhysicsRigidBody); the Xbox 360 Test PDB virtual order (third source) aligned on 38 TLC-named slots of that vtable puts CanCollideWithThings(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?CanCollideWithThings@CTCPhysicsRigidBody@@UBE_NXZ; `ret` matches the signature.
// Was: CTCPhysicsRigidBody::vfunc_57 (rebuild/src/compiled/00/75/gapscan_sub0075bf90_0075bf90.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSRIGIDBODY
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsRigidBody.h"
bool CTCPhysicsRigidBody::CanCollideWithThings() const
{
    return true;
}
