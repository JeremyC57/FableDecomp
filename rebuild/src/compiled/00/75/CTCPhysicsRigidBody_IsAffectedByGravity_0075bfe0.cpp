// CTCPhysicsRigidBody::IsAffectedByGravity() const -- returns true
// Leaf: ?IsAffectedByGravity@CTCPhysicsRigidBody@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075BFE0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 48 of CTCPhysicsRigidBody's vtable 0x0126996C (the slot holds this body in 1 vtable(s), owner CTCPhysicsRigidBody); the Xbox 360 Test PDB virtual order (third source) aligned on 38 TLC-named slots of that vtable puts IsAffectedByGravity(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsAffectedByGravity@CTCPhysicsRigidBody@@UBE_NXZ; `ret` matches the signature.
// Was: CTCPhysicsRigidBody::vfunc_48 (rebuild/src/compiled/00/75/gapscan_sub0075bfe0_0075bfe0.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSRIGIDBODY
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsRigidBody.h"
bool CTCPhysicsRigidBody::IsAffectedByGravity() const
{
    return true;
}
