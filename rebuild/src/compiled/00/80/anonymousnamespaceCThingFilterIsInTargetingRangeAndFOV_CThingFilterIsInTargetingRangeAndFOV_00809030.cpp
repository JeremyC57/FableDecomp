// CThingFilter_IsInTargetingRangeAndFOV::CThingFilter_IsInTargetingRangeAndFOV(const C3DVector&,const C3DVector&,const CThing&,const CTargetingDef&,const CThing*,float,bool) -- capture targeting context and normalize its facing vector.
// Retail: 0x00809030,166 bytes,thiscall.
// Leaf: ??0CThingFilter_IsInTargetingRangeAndFOV@?A0x51139a0f@@QAE@ABVC3DVector@@0ABVCThing@@ABVCTargetingDef@@PBV3@M_N@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#include "engine/CThing.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CThingFilter_IsInTargetingRangeAndFOV.h"
namespace {
CThingFilter_IsInTargetingRangeAndFOV::CThingFilter_IsInTargetingRangeAndFOV(
    const C3DVector& target_base_pos,const C3DVector& target_dir,const CThing& targeter,
    const CTargetingDef& targeting_def,const CThing* pcarried_thing,float targeting_fov,bool angle_test_3d)
    :Position(target_base_pos),FacingVector(target_dir),Targeter(targeter),PTargetLockThing(0),
     TargetingDef(targeting_def),PCarriedThing(pcarried_thing),TargetingFOV(targeting_fov),AngleTest3D(angle_test_3d)
{
    if(!angle_test_3d)
        FacingVector.Z=0;
    FacingVector.AccurateNormalise();
    const CTCTargetingBase* ptc_targ;
    if(targeter.PeekTC(&ptc_targ))
        PTargetLockThing=ptc_targ->GetPTargetLockedThing();
}
}