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
FABLE_STATIC_ASSERT(sizeof(CThingFilter_IsInTargetingRangeAndFOV)==48);
}
#endif
