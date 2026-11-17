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
#ifdef FABLE_RETAIL_CTHINGFILTER_OVERLAPPINGSPHERE_OUTOFLINE
    // NF59 opt-in (row 0x006689E0): Ego_r.pdb 0x005451C0 ??RCThingFilter_OverlappingSphere@@QBE_NABVCThing@@@Z is out of line in both the
    // PDB build and retail (0x006689E0; its landed caller 0x00785DD0 binds it). Declaration only; the default keeps the inline body.
    bool operator()(const CThing& test_thing) const;
#else
    bool operator()(const CThing& test_thing) const {
        float radius;
        C3DVector test_pos;
        test_thing.GetPhysicsTC()->GetBoundingInfo(test_pos,radius);
        if ((test_pos-SpherePos).GetSquaredMagnitude() <
            (radius+Radius)*(radius+Radius))
            return true;
        return false;
    }
#endif
};
#endif
