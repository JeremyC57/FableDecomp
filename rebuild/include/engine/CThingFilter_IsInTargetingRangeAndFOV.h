// Retail opt-in (Codex pass21): FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST exposes original targeting helpers; row00804020 retains both-PDB/donor/native proof.
#ifndef FABLE_ENGINE_CTHINGFILTER_ISINTARGETINGRANGEANDFOV_H
#define FABLE_ENGINE_CTHINGFILTER_ISINTARGETINGRANGEANDFOV_H
#include "engine/C3DVector.h"
class CThing;class CTargetingDef;
namespace {
// Original anonymous-namespace class, both PDBs: size48 and const reference members.
class CThingFilter_IsInTargetingRangeAndFOV {
    C3DVector Position;
    C3DVector FacingVector;
    const CThing& Targeter;
    const CThing* PTargetLockThing;
    const CTargetingDef& TargetingDef;
    const CThing* PCarriedThing;
    float TargetingFOV;
    bool AngleTest3D;
public:
    CThingFilter_IsInTargetingRangeAndFOV(const C3DVector&,const C3DVector&,const CThing&,const CTargetingDef&,const CThing*,float,bool);
    bool operator()(const CThing&)const;
};
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
CThingFilter_IsInTargetingRangeAndFOV::CThingFilter_IsInTargetingRangeAndFOV(const C3DVector& target_base_pos,const C3DVector& target_dir,const CThing& targeter,const CTargetingDef& targeting_def,const CThing* pcarried_thing,float targeting_fov,bool angle_test_3d)
 :Position(target_base_pos),FacingVector(target_dir),Targeter(targeter),PTargetLockThing(0),TargetingDef(targeting_def),PCarriedThing(pcarried_thing),TargetingFOV(targeting_fov),AngleTest3D(angle_test_3d)
{
 if(!angle_test_3d)FacingVector.Z=0.0f;
 FacingVector.AccurateNormalise();
 const CTCTargetingBase* ptc_targ;
 if(targeter.PeekTC(&ptc_targ))PTargetLockThing=ptc_targ->GetPTargetLockedThing();
}
#endif
FABLE_STATIC_ASSERT(sizeof(CThingFilter_IsInTargetingRangeAndFOV)==48);
}
#endif
