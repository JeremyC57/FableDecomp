#ifndef FABLE_ENGINE_CAISTATEGROUP_MINIONPUTOUTFIRES_H
#define FABLE_ENGINE_CAISTATEGROUP_MINIONPUTOUTFIRES_H
#ifdef FABLE_RETAIL_MINIONPUTOUTFIRES_PASS27
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
class CThingBuilding;
// Original Ego_r layout and retail Update agree on the building handle at 0x14.
class CAIStateGroup_MinionPutOutFires : public CAIStateGroupBase {
    bool PutOutFire();
#ifdef FABLE_RETAIL_GOTOWELL_PASS29
    bool GoToWell();
#endif
    CIntelligentPointer<CThingBuilding> PBuildingOnFire;
};
FABLE_STATIC_ASSERT_AT(12, sizeof(CAIStateGroup_MinionPutOutFires)==0x1c);
#endif
#endif
