#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
// Original ret/nav_route_calculated and scoped ptc_flammable locals; true shared owner/action types.
// Leaf: ?Update@CAIStateGroup_MinionWatchFires@@UAEXXZ
// Flags: (none)
#define FABLE_RETAIL_MINIONWATCHFIRES_UPDATE_PASS26
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCFlammable.h"
#include "engine/CAIStateGroup_MinionWatchFires.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureAction_AlertToPlayer.h"
#include "engine/ThingAlive.h"
void CAIStateGroup_MinionWatchFires::Update()
{
    bool nav_route_calculated = false;
    EAIActionReturn ret;
    do {
        if (!IS_THING_ALIVE(PBuildingOnFire))
            { Terminate(); return; }
        CTCFlammable* ptc_flammable;
        if (!PBuildingOnFire->GetTC(&ptc_flammable) || !ptc_flammable->IsOnFire())
            { Terminate(); return; }
        ret = AIAction_MoveToPosProximity((EAIActionRepeat)2, PBuildingOnFire->GetPos(), 5.0f, nav_route_calculated,
                                             (EMovementType)5, true, false, true, 0, true, 0.0f);
    } while (ret == 4);
    if(ret!=2){Terminate();return;}
    {
        for (;;) {
            if (!IS_THING_ALIVE(PBuildingOnFire))
                {Terminate();return;}
            const CTCFlammable* ptc_flammable;
            if (!PBuildingOnFire->PeekTC(&ptc_flammable) || !ptc_flammable->IsOnFire())
                {Terminate();return;}
            if (AIAction_PerformActionAndWait(CCreatureAction_AlertToPlayer(GetCreature()), false) != 2)
                {Terminate();return;}
            if (AIAction_DoNothing() != 4)
                return;
        }
    }

}