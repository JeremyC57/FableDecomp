// CTCPhysicsBase::GetRenderZOffset(float) const -- returns 0.0f (no render offset; the interpolation argument is unused)
// Leaf: ?GetRenderZOffset@CTCPhysicsBase@@UBEMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B12F0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 79 of CTCPhysicsRigidBody's vtable 0x0126996C (the slot holds this body in 5 vtable(s), owner CTCPhysicsBase); the Xbox 360 Test PDB virtual order (third source) aligned on 38 TLC-named slots of that vtable puts GetRenderZOffset(float) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetRenderZOffset@CTCPhysicsBase@@UBEMM@Z; `ret` matches the signature.
// Was: CTCPhysicsBase::vfunc_79 (rebuild/src/compiled/00/6b/CTCPhysicsBase_vfunc79_006b12f0.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsBase.h"
float CTCPhysicsBase::GetRenderZOffset(float) const
{
    return 0.0f;
}
