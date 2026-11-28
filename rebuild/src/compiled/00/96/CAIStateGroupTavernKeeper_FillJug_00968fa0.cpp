// Leaf: ?FillJug@CAIStateGroup_TavernKeeper@@QAE_NXZ
// Flags: /O2 /Oy /W3
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_SETPUSHABLEBYHERO
#define FABLE_RETAIL_CAISTATEGROUP_TAVERNKEEPER
#define FABLE_RETAIL_CAISTATEGROUP_TAVERNKEEPER_RECEIVEPAYMENT
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_SUSPENDABLE_BASE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCANIMATINGOBJECT_PLAYANIMATION
#define FABLE_RETAIL_CTCANIMATINGOBJECT_PLAYANIMATION_NAME
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_TAVERN_FILL_JUG_PASS30
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS_FIRST
#define FABLE_RETAIL_CTHING_GETPOS
#define FABLE_RETAIL_CTHINGOBJECT
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_THISEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#include "fable_string.h"
#include "engine/EAIAction.h"
#include "engine/C3DVector.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CAIStateGroup_TavernKeeper.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasDefName.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PickUpJugToFill.h"
#include "engine/CCreatureAction_PutDownJugFromFilling.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CTCAnimatingObject.h"
#include "engine/CTCTavernJug.h"
#include "engine/ThingAlive.h"

bool CAIStateGroup_TavernKeeper::FillJug()
{
    if (!IS_THING_ALIVE(PJug))
        return false;

    SetPushableByHero(false);
    if (AIAction_PerformActionAndWait(CCreatureAction_PickUpJugToFill(GetCreature(), *PJug), false) != AI_ACTION_FINISHED) {
        SetPushableByHero(true);
        return false;
    }

    const long box_width_and_height = 2;
    C2DBoxI area(GetCreature().GetPos().ToC2DCoordI(), box_width_and_height);
    const CThingSearchTools& search_tools = CMainGameComponent::Get()->GetWorld().PeekThingSearchTools();
    CThing* bar_tap;
    search_tools.GetPBestThingInRoughArea(area, CThingFilter_HasDefName("OBJECT_GENERIC_BARTAP_01"),
        CThingCompare_Nearest(GetCreature().GetPos()), &bar_tap);

    const long amount_to_fill = 7;
    const long fill_until = CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * amount_to_fill;
    CTCAnimatingObject* tc_animating_object = 0;
    if (!bar_tap->GetTC(&tc_animating_object)) {
        FABLE_RETAIL_ASSERT(false);
    }
    int priority = 0;
    do {
        if (tc_animating_object)
            tc_animating_object->PlayAnimation("DRAW", priority++, false);
        if (AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "FILL_JUG_LOOP", CSoundPair(0),
                false, true, false, 0, 0), false) == AI_ACTION_TERMINATE_PROCESS) {
            SetPushableByHero(true);
            return false;
        }
    } while (fill_until > CWorld::GetFrame());

    if (IS_THING_DEAD(PJug))
        return false;

    CTCTavernJug* tc_tavern_jug;
    if (!PJug->GetTC(&tc_tavern_jug)) {
        FABLE_RETAIL_ASSERT(false);
    }
    tc_tavern_jug->Fullness = 3;
    if (AIAction_PerformActionAndWait(CCreatureAction_PutDownJugFromFilling(GetCreature(), *PJug), false) != AI_ACTION_FINISHED) {
        SetPushableByHero(true);
        return false;
    }
    SetPushableByHero(true);
    return true;
}