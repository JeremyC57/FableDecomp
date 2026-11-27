// 00935c40 CAIStateGroup_MinionPutOutFires::GoToWell
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CTCPHYSICSBASE
// Leaf: ?GoToWell@CAIStateGroup_MinionPutOutFires@@AAE_NXZ
// Inline-budget: the CThingObject pointer constructor and deleting destructor use the size setting. The remaining shared owners and the primary use the speed setting.
// Flags: /Os
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND
#define FABLE_RETAIL_CCREATUREACTION_FILLBUCKET_VIRTUAL_MODEL
#define FABLE_RETAIL_GOTOWELL_PASS29
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTLEAF
#define FABLE_RETAIL_CTHINGOBJECT_STATGETTYPE

#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_WELL_POINTER_CTOR_OWNER_PASS29
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#define FABLE_RETAIL_MINIONPUTOUTFIRES_PASS27
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTIONONCE
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

#ifdef _MSC_VER
#pragma optimize("t",on)
#endif
#include "engine/CThingObject.h"

#ifdef _MSC_VER
#pragma optimize("t",on)
#endif
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCFlammable.h"
#include "engine/CAIStateGroup_MinionPutOutFires.h"
#include "engine/CAIBrain.h"
#include "engine/ThingAlive.h"
#include "fable_string.h"

#include "engine/CThingCompare_Nearest.h"
#include "engine/CThingFilter_ObjectHasProperty.h"
#include "engine/CThingSearchTools.h"
#include "engine/CCreatureAction_FillBucket.h"

#include "engine/CTCPhysicsBase.h"

bool CAIStateGroup_MinionPutOutFires::GoToWell()
{
    EAIActionReturn ret = AI_ACTION_FAILED;
    CThingObject* pwell = 0;
    if (PeekThingSearchTools().GetPBestLeafThing(CThingFilter_ObjectHasProperty(THING_OBJECT_PROPERTY_WELL),
                                                 CThingCompare_Nearest(GetCreature().GetPos()), &pwell)) {
        CIntelligentPointer<CThingObject> ipwell(pwell);
        bool nav_route_calculated = false;
        do {
            const CTCFlammable* ptc_flammable;
            if (IS_THING_DEAD(ipwell) || IS_THING_DEAD(PBuildingOnFire) ||
                !PBuildingOnFire->PeekTC(&ptc_flammable) || !ptc_flammable->IsOnFire())
                return false;
            ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, ipwell, 4.0f,
                nav_route_calculated, AI_MOVEMENT_RUNNING, true, false, true, true, 0.0f);
        } while (ret == AI_ACTION_IN_PROGRESS);
        if (ret == AI_ACTION_FINISHED) {
            if (AIAction_PerformAction(CCreatureAction_FillBucket(GetCreature())) == AI_ACTION_FINISHED)
                return true;
        }
    }
    return false;
}
