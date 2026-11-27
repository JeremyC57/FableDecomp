#ifdef FABLE_RETAIL_DAMAGE_ACTION_PASS29
#ifndef FABLE_ENGINE_CTHINGFILTER_COMBATOVERLAPPINGSPHERE_H
#define FABLE_ENGINE_CTHINGFILTER_COMBATOVERLAPPINGSPHERE_H
#include "engine/CThing.h"
#include "engine/CTCHitLocations.h"
#include "engine/GFSqr.h"
class CThingFilter_CombatOverlappingSphere : public std::unary_function<CThing,bool> {
public:
    CThingFilter_CombatOverlappingSphere(const C3DVector& pos, float radius) : SpherePos(pos), Radius(radius) {}
    // Inline-budget: native 0085b2a0 and 0085b5b0 both expand the complete sphere test.
    FABLE_FORCEINLINE bool operator()(const CThing& test_thing) const
    {
        const CTCHitLocations* ptc_hit_locations;
        if (test_thing.PeekTC(&ptc_hit_locations)) {
            C3DVector centre_pos;
            float radius;
            ptc_hit_locations->GetBoundingSphere(centre_pos, radius, true);
            if ((centre_pos - SpherePos).GetSquaredMagnitude() < GFSqr(radius + Radius))
                return true;
        }
        return false;
    }
private:
    C3DVector SpherePos;                                                // +0x00
    float Radius;                                                       // +0x0c
};
#endif
#endif
