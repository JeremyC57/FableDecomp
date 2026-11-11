// CTCPhysicsBase::GetImpulse() const -- returns the zero vector
// Leaf: ?GetImpulse@CTCPhysicsBase@@UBE?AVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B1490, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 76 of CTCPhysicsRigidBody's vtable 0x0126996C (the slot holds this body in 5 vtable(s), owner CTCPhysicsBase); the Xbox 360 Test PDB virtual order (third source) aligned on 38 TLC-named slots of that vtable puts GetImpulse(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetImpulse@CTCPhysicsBase@@UBE?AVC3DVector@@XZ; `ret` matches the signature.
// Was: CTCPhysicsBase::vfunc_76 (rebuild/src/compiled/00/6b/CTCPhysicsBase_vfunc76_006b1490.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsBase.h"
C3DVector CTCPhysicsBase::GetImpulse() const
{
    return C3DVector(0.0f, 0.0f, 0.0f);
}
