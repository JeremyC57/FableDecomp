// CTCTargetingBase::BulletTimeFrameUpdate() -- the bullet-time update is the frame update (the slot-9 FrameUpdate)
// Leaf: ?BulletTimeFrameUpdate@CTCTargetingBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DEBF7, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 10 of CTCTargetingBase's vtable 0x01271E74 (the slot holds this body in 3 vtable(s), owner CTCTargetingBase); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts BulletTimeFrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?BulletTimeFrameUpdate@CTCTargetingBase@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCTargetingSpirit::vfunc_10 (rebuild/src/compiled/00/4d/CTCTargetingSpirit_vfunc10_004debf7.cpp).
// Opt-ins as in the landed row anonymousnamespaceCThingFilterIsInTargetingRangeAndFOV_CThingFilterIsInTargetingRangeAndFOV_00809030.cpp.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#include "engine/CThing.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CThingFilter_IsInTargetingRangeAndFOV.h"
void CTCTargetingBase::BulletTimeFrameUpdate()
{
    FrameUpdate();
}
