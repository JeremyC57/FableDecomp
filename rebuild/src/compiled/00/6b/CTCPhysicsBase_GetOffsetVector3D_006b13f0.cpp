// CTCPhysicsBase::GetOffsetVector3D() const -- returns the zero vector
// Leaf: ?GetOffsetVector3D@CTCPhysicsBase@@MBE?AVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B13F0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 29 of CTCPhysicsRigidBody's vtable 0x0126996C (the slot holds this body in 3 vtable(s), owner CTCPhysicsBase); the Xbox 360 Test PDB virtual order (third source) aligned on 38 TLC-named slots of that vtable puts GetOffsetVector3D(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetOffsetVector3D@CTCPhysicsBase@@MBE?AVC3DVector@@XZ; `ret` matches the signature.
// Was: CTCPhysicsBase::vfunc_29 (rebuild/src/compiled/00/6b/CTCPhysicsBase_vfunc29_006b13f0.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsBase.h"
C3DVector CTCPhysicsBase::GetOffsetVector3D() const
{
    return C3DVector(0.0f, 0.0f, 0.0f);
}
