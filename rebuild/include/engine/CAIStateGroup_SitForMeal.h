#ifndef FABLE_ENGINE_CAISTATEGROUP_SITFORMEAL_H
#define FABLE_ENGINE_CAISTATEGROUP_SITFORMEAL_H
#include "engine/CAIStateGroup_UsesSeat.h"
class CAIStateGroup_SitForMeal : public CAIStateGroup_UsesSeat {
private:
    bool GetFoodFromCookingPot();
    bool IsMealTime() const;
    CIntelligentPointer<CThing> PPot;
    long ActionPointIndex;
    bool HoldingBowl;
    long DayLastAte;
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_SitForMeal)==0x48);
#endif
