// CTCPhysicsRigidBody::GetFacingVector() const -- returns RHSet's forward vector
// Leaf: ?GetFacingVector@CTCPhysicsRigidBody@@UBE?AVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00759B20, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 70 of CTCPhysicsRigidBody's vtable 0x0126996C (the slot holds this body in 1 vtable(s), owner CTCPhysicsRigidBody); the Xbox 360 Test PDB virtual order (third source) aligned on 38 TLC-named slots of that vtable puts GetFacingVector(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetFacingVector@CTCPhysicsRigidBody@@UBE?AVC3DVector@@XZ; `ret` matches the signature.
// Was: CTCPhysicsRigidBody::vfunc_70 (rebuild/src/compiled/00/75/CTCPhysicsRigidBody_vfunc70_00759b20.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSRIGIDBODY
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsRigidBody.h"
C3DVector CTCPhysicsRigidBody::GetFacingVector() const
{
    return RHSet.GetForward();
}
