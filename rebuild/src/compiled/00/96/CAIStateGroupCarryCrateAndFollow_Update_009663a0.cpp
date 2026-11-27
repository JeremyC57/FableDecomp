// 009663a0 CAIStateGroup_CarryCrateAndFollow::Update
// Flags:
// Leaf: ?Update@CAIStateGroup_CarryCrateAndFollow@@UAEXXZ
#include "fable_assert.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_CARRYCRATEANDFOLLOW
#define FABLE_RETAIL_CAISTATEGROUP_CARRYCRATEANDFOLLOW_EXPRESSION
#define FABLE_RETAIL_CARRYCRATE_UPDATE_PASS29
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CTCUSERACTIONPOINTBASE
#define FABLE_RETAIL_CTCCARRYABLE_ISCARRIED
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_GETACTIONID
#define FABLE_RETAIL_CCREATUREACTION_GENERICBOX
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_CarryCrateAndFollow.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCUserActionPointBase.h"
#include "engine/CTCCarryable.h"
#include "engine/CTCCarrying.h"
#include "engine/CCreatureAction_PickUpGenericBox.h"
#include "engine/ThingAlive.h"

void CAIStateGroup_CarryCrateAndFollow::Update()
{
    if (!IS_THING_ALIVE(PCrate)) {
        Terminate();
        return;
    }
    CTCSingleUserActionPoints* ptc_single_user_action_points;
    if (!PCrate->GetTC(&ptc_single_user_action_points)) {
        FABLE_RETAIL_ASSERT(false);
    }
    C3DVector pick_up_pos;
    CRightHandedSet pick_up_rhset;
    if (!ptc_single_user_action_points->GetFreeActionPoint(GetCreature(), pick_up_pos, pick_up_rhset, ActionPointIndex)) {
        Terminate();
        return;
    }
    GotActionPoint = true;
    C3DVector facing_vector = pick_up_rhset.GetForward();
    CTCCarryable* ptc_carryable;
    if (!PCrate->GetTC(&ptc_carryable)) {
        FABLE_RETAIL_ASSERT(false);
    }
    EAIActionReturn move_to_item_return_status = AI_ACTION_FAILED;
    do {
        if (!IS_THING_ALIVE(PCrate) || ptc_carryable->IsCarried()) {
            Terminate();
            return;
        }
        bool finished_calculating_route;
        move_to_item_return_status = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK, pick_up_pos, finished_calculating_route, AI_MOVEMENT_RUNNING, true, true, PCrate.Get(), true, 0.0f);
    } while (move_to_item_return_status == AI_ACTION_IN_PROGRESS);
    if (move_to_item_return_status == AI_ACTION_FAILED) {
        do {
            if (!IS_THING_ALIVE(PCrate) || ptc_carryable->IsCarried()) {
                Terminate();
                return;
            }
            bool finished_calculating_route;
            move_to_item_return_status = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, PCrate->GetPos(), 1.0f, finished_calculating_route, AI_MOVEMENT_RUNNING, true, false, true,
                PCrate.Get(), true, 0.0f);
        } while (move_to_item_return_status == AI_ACTION_IN_PROGRESS);
        if (IS_THING_ALIVE(PCrate)) {
            facing_vector = PCrate->GetPos() - GetCreature().GetPos();
        }
    }
    if (!IS_THING_ALIVE(PCrate) || ptc_carryable->IsCarried() || move_to_item_return_status != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    EAIActionReturn face_direction_return_status = AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED, GetCreature().GetPos() + facing_vector, 0x21, 0.0027777f);
    if (!IS_THING_ALIVE(PCrate) || ptc_carryable->IsCarried() || face_direction_return_status != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    CCreatureAction_PickUpGenericBox pick_up_action(GetCreature(), *PCrate);
    PickUpActionID = pick_up_action.GetActionID();
    CTCCarrying* ptc_carrying;
    if (!GetCreature().GetTC(&ptc_carrying)) {
        FABLE_RETAIL_ASSERT(false);
    }
    PPreviouslyCarriedItem = ptc_carrying->GetThingInPrimarySlot();
    ptc_carrying->RemoveThingFromPrimarySlot(false);
    if (AIAction_PerformActionAndWait(pick_up_action, false) != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    do {
        if (IsExpressionPerfomedTo("EXPRESSION_FOLLOW", PThingToFollow)) {
            EAIActionReturn follow_thing_return_status = AI_ACTION_FAILED;
            do {
                if (!IS_THING_ALIVE(PThingToFollow) || IsExpressionPerfomedTo("EXPRESSION_WAIT", PThingToFollow))
                    break;
                bool finished_calculating_route;
                follow_thing_return_status = AIAction_MoveToThingProximity(ACTION_REPEAT_UNTIL_FINISHED, PThingToFollow, 2.0f, finished_calculating_route, AI_MOVEMENT_RUNNING, true, false, true, true, 0.0f);
            } while (follow_thing_return_status == AI_ACTION_FINISHED);
        }
    } while (AIAction_DoNothing() == AI_ACTION_IN_PROGRESS);
    Terminate();
}