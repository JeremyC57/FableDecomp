// CTCPhysicsRigidBody::GetOffsetVector() const -- returns OffsetVector
// Leaf: ?GetOffsetVector@CTCPhysicsRigidBody@@UBE?AVC2DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075BFF0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 28 of CTCPhysicsRigidBody's vtable 0x0126996C (the slot holds this body in 1 vtable(s), owner CTCPhysicsRigidBody); the Xbox 360 Test PDB virtual order (third source) aligned on 38 TLC-named slots of that vtable puts GetOffsetVector(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetOffsetVector@CTCPhysicsRigidBody@@UBE?AVC2DVector@@XZ; `ret` matches the signature.
// Was: CTCPhysicsRigidBody::vfunc_28 (rebuild/src/compiled/00/75/CTCPhysicsRigidBody_vfunc28_0075bff0.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSRIGIDBODY
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsRigidBody.h"
C2DVector CTCPhysicsRigidBody::GetOffsetVector() const
{
    return OffsetVector;
}
