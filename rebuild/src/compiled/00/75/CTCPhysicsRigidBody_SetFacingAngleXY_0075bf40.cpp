// CTCPhysicsRigidBody::SetFacingAngleXY(float) -- does nothing (a rigid body's facing comes from its simulation)
// Leaf: ?SetFacingAngleXY@CTCPhysicsRigidBody@@UAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075BF40, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 66 of CTCPhysicsRigidBody's vtable 0x0126996C (the slot holds this body in 1 vtable(s), owner CTCPhysicsRigidBody); the Xbox 360 Test PDB virtual order (third source) aligned on 38 TLC-named slots of that vtable puts SetFacingAngleXY(float) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetFacingAngleXY@CTCPhysicsRigidBody@@UAEXM@Z; `ret` matches the signature.
// Was: CTCPhysicsRigidBody::vfunc_66 (rebuild/src/compiled/00/75/gapscan_sub0075bf40_0075bf40.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSRIGIDBODY
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsRigidBody.h"
void CTCPhysicsRigidBody::SetFacingAngleXY(float)
{
}
