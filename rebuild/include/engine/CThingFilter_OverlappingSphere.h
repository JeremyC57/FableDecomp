#ifndef FABLE_ENGINE_CTHINGFILTER_OVERLAPPINGSPHERE_H
#define FABLE_ENGINE_CTHINGFILTER_OVERLAPPINGSPHERE_H
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
class CThing;
// Ego_r TPI original functor base/members; native stack +80 stores vector
// then Radius=0 at+8c in AutoPlaceNearBuilding.
class CThingFilter_OverlappingSphere : public std::unary_function<CThing,bool> {
private:
    C3DVector SpherePos;
    float Radius;
public:
    CThingFilter_OverlappingSphere(const C3DVector& p,float r):SpherePos(p),Radius(r){}
    bool operator()(const CThing& test_thing) const {
        float radius;
        C3DVector test_pos;
        test_thing.GetPhysicsTC()->GetBoundingInfo(test_pos,radius);
        if ((test_pos-SpherePos).GetSquaredMagnitude() <
            (radius+Radius)*(radius+Radius))
            return true;
        return false;
    }
};
#endif
