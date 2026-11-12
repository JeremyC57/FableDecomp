// CTCPhysicsStandard::GetOffsetVector() const -- returns the X / Y of OffsetVector3D (FableWin C2DVector(const C3DVector&), inline)
// Leaf: ?GetOffsetVector@CTCPhysicsStandard@@MBE?AVC2DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00724D30, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 28 of CTCPhysicsNavigator's vtable 0x01266314 (the slot holds this body in 3 vtable(s), owner CTCPhysicsStandard); the Xbox 360 Test PDB virtual order (third source) aligned on 46 TLC-named slots of that vtable puts GetOffsetVector(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetOffsetVector@CTCPhysicsStandard@@MBE?AVC2DVector@@XZ; `ret` matches the signature.
// Was: CTCPhysicsStandard::vfunc_28 (rebuild/src/compiled/00/72/CTCPhysicsStandard_vfunc28_00724d30.cpp).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_OVERRIDES
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_FLAGS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsStandard.h"
#include "engine/C2DVector.h"
C2DVector CTCPhysicsStandard::GetOffsetVector() const
{
    return C2DVector(OffsetVector3D);
}
