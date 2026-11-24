// 00935e40 CAIStateGroup_MinionPutOutFires::PutOutFire
// Leaf: ?PutOutFire@CAIStateGroup_MinionPutOutFires@@AAE_NXZ
// Flags:
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#define FABLE_RETAIL_MINIONPUTOUTFIRES_PASS27
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTIONONCE
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
#include "engine/CAIStateGroup_MinionPutOutFires.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/ThingAlive.h"
#include "fable_string.h"
#include "engine/CSoundPair.h"

bool CAIStateGroup_MinionPutOutFires::PutOutFire()
{
    bool nav_route_calculated = false;
    EAIActionReturn ret;
    do {
        const CTCFlammable* ptc_flammable;
        if (IS_THING_DEAD(PBuildingOnFire) || !PBuildingOnFire->PeekTC(&ptc_flammable) || !ptc_flammable->IsOnFire())
            return false;
        ret = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, PBuildingOnFire->GetPos(), 3.0f,
            nav_route_calculated, AI_MOVEMENT_RUNNING, true, false, true, 0, true, 0.0f);
    } while (ret == AI_ACTION_IN_PROGRESS);
    if (ret == AI_ACTION_FINISHED && IS_THING_ALIVE(PBuildingOnFire)) {
        const CTCFlammable* ptc_flammable;
        if (PBuildingOnFire->PeekTC(&ptc_flammable) && ptc_flammable->IsOnFire()) {
            if (AIAction_PerformAction(CCreatureAction_PlayAnimation(GetCreature(), "ST_IDLE", CSoundPair(0), false, true, false, 0, 0)) == AI_ACTION_FINISHED)
                return true;
        }
    }
    return false;
}