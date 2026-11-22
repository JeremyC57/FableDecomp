#ifndef FABLE_ENGINE_CISNEARCAMPFIREWITHSPACE_H
#define FABLE_ENGINE_CISNEARCAMPFIREWITHSPACE_H
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
class CThing;class CThingObject;
// Both original PDBs: unary_function<CThing,bool>, float DistanceSq, C3DVector FromPos.
class CIsNearCampfireWithSpace:public std::unary_function<CThing,bool>{
public:
#ifdef FABLE_RETAIL_CISNEARCAMPFIREWITHSPACE_RANGE_SCOPE_PASS26
    // After the two position copies, the retail fallback stores the squared range at 0093AD3E.
    CIsNearCampfireWithSpace(float distance, const C3DVector& from_pos) : FromPos(from_pos)
    {
        DistanceSq = GFSqr(distance);
    }
#else
CIsNearCampfireWithSpace(float distance,const C3DVector& from_pos):DistanceSq(GFSqr(distance)),FromPos(from_pos){}
#endif // FABLE_RETAIL_CISNEARCAMPFIREWITHSPACE_RANGE_SCOPE_PASS26

 bool operator()(const CThingObject&)const;
private:float DistanceSq;C3DVector FromPos;
};
FABLE_STATIC_ASSERT(sizeof(CIsNearCampfireWithSpace)==16);
#endif
