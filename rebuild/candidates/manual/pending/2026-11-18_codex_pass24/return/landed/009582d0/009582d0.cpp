// CAIStateGroup_MinionReactToTrespassing::Update -- Alternate approaching and watching the hero while a trespass persists.
// Leaf: ?Update@CAIStateGroup_MinionReactToTrespassing@@UAEXXZ
// Retail: 0x009582D0, 1098 bytes, thiscall.
#define FABLE_RETAIL_MINION_TRESPASS_UPDATE_PASS24
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_MANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/ThingAlive.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"

#include "engine/CThingBuilding.h"
#include "engine/COpinionDeed.h"
#include "engine/COpinionDeedFilters_Trespassing.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CAIStateGroup_MinionReactToTrespassing.h"

namespace { long FRAMES_BETWEEN_CHECKING_IS_STILL_VALID = 30; }
class CTCTrespassable;

inline CThingBuilding* CThingAICreature::GetHomeBuilding() const { return HomeBuilding.Get(); }

inline bool CAIStateGroup_MinionReactToTrespassing::IsStillRunnable(bool ignore_frame_check)
{
    if (!ignore_frame_check && CWorld::GetFrame()-LastFrameCheckedIsStillValid < FRAMES_BETWEEN_CHECKING_IS_STILL_VALID)
        return true;
    LastFrameCheckedIsStillValid = CWorld::GetFrame();
    CThingBuilding* home_building = GetCreature().GetHomeBuilding();
    if (IS_THING_ALIVE(home_building)) {
        const CTCCreatureOpinionOfHero* ptc_opinion;
        if (GetCreature().GetTC(TCI_CREATURE_OPINION_OF_HERO, &ptc_opinion)) {
            if (ptc_opinion->FindMostRecentDeed(OPINION_DEED_TYPE_CRIME_TRESPASS_FIRST, GFFilter_Bind2(COpinionDeedFilter_IsStillHappening(), COpinionDeedFilter_TargetIs(*home_building)), 0, true)
                || ptc_opinion->FindMostRecentDeed(OPINION_DEED_TYPE_CRIME_TRESPASS_SECOND, GFFilter_Bind2(COpinionDeedFilter_IsStillHappening(), COpinionDeedFilter_TargetIs(*home_building)), 0, true)
                || ptc_opinion->FindMostRecentDeed(OPINION_DEED_TYPE_CRIME_TRESPASS_THIRD, GFFilter_Bind2(COpinionDeedFilter_IsStillHappening(), COpinionDeedFilter_TargetIs(*home_building)), 0, true))
                return true;
        }
    }
    return false;
}

void CAIStateGroup_MinionReactToTrespassing::Update()
{
    CTCTrespassable* trespassable;
    if (IS_THING_ALIVE(GetCreature().GetHomeBuilding()))
        GetCreature().GetHomeBuilding()->GetTC(TCI_TRESPASSABLE, &trespassable);
    PHero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    bool moving = true;
    EAIActionReturn ret;
    do {
        if (!IS_THING_ALIVE(PHero)) break;
        if (moving) {
            do {
                if (!IS_THING_ALIVE(PHero)) { Terminate(); return; }
                bool finished_calculating_route;
                ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,PHero,3.0f,finished_calculating_route,AI_MOVEMENT_WALKING,true,false,true,true,0.0f);
                if (!IsStillRunnable(false) || !IS_THING_ALIVE(PHero)) ret = AI_ACTION_TERMINATE_PROCESS;
            } while (ret == AI_ACTION_IN_PROGRESS);
            moving = false;
        } else {
            do {
                if (!IS_THING_ALIVE(PHero)) { Terminate(); return; }
                ret = AIAction_LookAtThing(ACTION_REPEAT_CALLBACK,PHero,19,0.0027777f);
                if (!IsStillRunnable(false) || !IS_THING_ALIVE(PHero)) ret = AI_ACTION_TERMINATE_PROCESS;
            } while (ret == AI_ACTION_IN_PROGRESS);
            moving = true;
        }
    } while (ret != AI_ACTION_TERMINATE_PROCESS);
    Terminate();
}
