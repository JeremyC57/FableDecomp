// CTCPhysicsRigidBody::SetFacingAngleYZ(float) -- does nothing (a rigid body's facing comes from its simulation)
// Leaf: ?SetFacingAngleYZ@CTCPhysicsRigidBody@@UAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075BF50, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 67 of CTCPhysicsRigidBody's vtable 0x0126996C (the slot holds this body in 1 vtable(s), owner CTCPhysicsRigidBody); the Xbox 360 Test PDB virtual order (third source) aligned on 38 TLC-named slots of that vtable puts SetFacingAngleYZ(float) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetFacingAngleYZ@CTCPhysicsRigidBody@@UAEXM@Z; `ret` matches the signature.
// Was: CTCPhysicsRigidBody::vfunc_67 (rebuild/src/compiled/00/75/gapscan_sub0075bf50_0075bf50.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSRIGIDBODY
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsRigidBody.h"
void CTCPhysicsRigidBody::SetFacingAngleYZ(float)
{
}
