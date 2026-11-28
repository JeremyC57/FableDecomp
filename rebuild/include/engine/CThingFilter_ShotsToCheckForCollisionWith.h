#ifndef FABLE_ENGINE_SHOTSCOLLISIONFILTER_H
#define FABLE_ENGINE_SHOTSCOLLISIONFILTER_H
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_SHOT_COLLISION_PASS30
// tc_shot_definition_base.obj: Ego_d lines 74 through 102, FableWin 0x027079B0.
// Inline-budget: the out-of-line Bind3 and search copies both expand this predicate.
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCDriverBase.h"
#include "engine/NCreatureMode.h"

namespace {
class CThingFilter_ShotsToCheckForCollisionWith : public std::unary_function<CThing,bool> {
public:
    FABLE_FORCEINLINE bool operator()(const CThing& test_thing) const {
        long type_flags = test_thing.GetTypeFlags();
        const CTCPhysicsBase* ptc_physics = test_thing.GetPhysicsTC();
        if (NCreatureMode::IsToEvadeAllHits(test_thing))
            return false;
        if (!ptc_physics || !(ptc_physics->CanInteractWithThings() || ptc_physics->CanCollideWithThings()))
            return false;
        else if (type_flags & 8)
            return true;
        else if (type_flags & 4)
            return true;
        else if (test_thing.HasTCDriver() && test_thing.GetTCDriver().GetInterfaceType() == TCI_EXPERIENCE_ORB)
            return true;
        return false;
    }
};
}
#endif
#endif
