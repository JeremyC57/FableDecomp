// CAIStateGroup_SitRoundCampfire::Update()
// Leaf: ?Update@CAIStateGroup_SitRoundCampfire@@UAEXXZ
// Flags:
#define FABLE_RETAIL_SIT_ROUND_CAMPFIRE_PASS27
#define FABLE_RETAIL_CANIMFILTERBASE_SIZE_PASS27
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMFILTER
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_MATERIALISED
#define FABLE_RETAIL_CTCUSERACTIONPOINTBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION_NOWAIT
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_SITDOWN
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_DTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_GFRANDOM_OUTOFLINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#include "engine/CMainGameComponent.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CAIStateGroup_SitRoundCampfire.h"
#include "engine/CAIBrain.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/ThingAlive.h"
#include "engine/CTCUserActionPointBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CAnimFilterAnyGroup.h"
#include "engine/CAnimationEntry.h"
#include "engine/CCreatureAction_SitDown.h"
#include "engine/NCreatureMode_CModeSitFloor.h"
#include "engine/GFRandom.h"
#include "engine/CWorld.h"

// The retail conversion at 0x00415DD0 stays out of line. Its visible body
// preserves the CAnimFilterBase destructor store as the next string temporary is prepared.
#ifdef _MSC_VER
__declspec(noinline)
#endif
inline CDefString::operator CCharString() const
{
    return GDefStringTable.GetString(TablePos);
}

namespace {
CCharString SIT_ON_FLOOR_ANIM_NAME("ST_SIT_ON_FLOOR");
const unsigned long MIN_TIME_TO_SIT = CMainGameComponent::GetConstantFPS() * 20;
const unsigned long RANGE_TIME_TO_SIT = CMainGameComponent::GetConstantFPS() * 120;
}

void CAIStateGroup_SitRoundCampfire::Update()
{
    if (!IS_THING_ALIVE(PCampfire)) {
        Terminate();
        return;
    }

    CTCSingleUserActionPoints* ptc_action_points;
    PCampfire->GetTC(&ptc_action_points);
    C3DVector pos_to_sit;
    CRightHandedSet rhset;
    if (!ptc_action_points->GetFreeActionPoint(GetCreature(), pos_to_sit, rhset, SeatPosIndex)) {
        Terminate();
        return;
    }
    GotSeatPos = true;

    EAIActionReturn ret = AI_ACTION_FAILED;
    do {
        if (!IS_THING_ALIVE(PCampfire)) {
            Terminate();
            return;
        }
        bool nav_route_calculated;
        ret = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK, pos_to_sit, nav_route_calculated,
                                 AI_MOVEMENT_WALKING, true, true, 0, true, 0.0f);
    } while (ret == AI_ACTION_IN_PROGRESS);

    if (ret != AI_ACTION_FINISHED || !IS_THING_ALIVE(PCampfire)) {
        Terminate();
        return;
    }

    C3DVector required_facing_dir(rhset.GetForward());
    C3DVector pos_to_look_at = GetCreature().PeekPos() + required_facing_dir;
    ret = AI_ACTION_FAILED;
    do {
        if (!IS_THING_ALIVE(PCampfire)) {
            Terminate();
            return;
        }
        ret = AIAction_LookAtPos(ACTION_REPEAT_CALLBACK, pos_to_look_at, 0x11, 0.0027777f);
    } while (ret == AI_ACTION_IN_PROGRESS);

    if (ret != AI_ACTION_FINISHED || !IS_THING_ALIVE(PCampfire)) {
        Terminate();
        return;
    }

    const CAnimationEntry& anim_entry = GetCreature().GetRandomAnimFromNameWithFilter(SIT_ON_FLOOR_ANIM_NAME, CAnimFilterAnyGroup());
    CCountedPointer<NCreatureMode::CModeSitFloor> psit_mode(new NCreatureMode::CModeSitFloor(GetCreature(), anim_entry.GroupName));
    Sitting = true;
    if (AIAction_PerformAction(CCreatureAction_SitDown(GetCreature(), anim_entry, psit_mode)) != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }

    unsigned long frame_to_stop_sitting = PeekWorld().GetFrame() + MIN_TIME_TO_SIT + GFRandom(RANGE_TIME_TO_SIT, GetPWorldSeed());
    while ((unsigned long)PeekWorld().GetFrame() < frame_to_stop_sitting && AIAction_DoNothing() == AI_ACTION_IN_PROGRESS) {
    }
    Terminate();
}