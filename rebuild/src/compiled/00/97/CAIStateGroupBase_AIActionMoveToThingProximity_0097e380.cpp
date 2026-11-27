// 0097e380 CAIStateGroupBase::AIAction_MoveToThingProximity
// Leaf: ?AIAction_MoveToThingProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@$$CBVCThing@@@@MAA_NW4EMovementType@@_N444M@Z
// Flags: none
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ
#define FABLE_RETAIL_IS_THING_DEAD_DECL_ONLY
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
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
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_MOVETOTHINGPROXIMITY_PASS29
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_GFGET2DDISTANCESQUARED
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CAIBRAIN_SUSPENDABLE_BASE
#define FABLE_RETAIL_CAIBRAIN_SETOUTPUT
#define FABLE_RETAIL_CBRAINOUTPUT_ONLY
#define FABLE_RETAIL_CBRAINOUTPUT_ACTION_CALLED_PASS24
#define FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_ACCESS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define FABLE_RETAIL_CLOOKDEF
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/ENavigatorReturn.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CCreatureActionBase.h"
#include "engine/campfire_types.h"
#include "engine/CTCLook.h"
#include "engine/C2DVector.h"
#include "engine/GFSqr.h"
inline ETCInterfaceType FABLE_FASTCALL CTCLook::StatGetInterfaceType() { return TCI_LOOK; }
inline CNavigationInput::CNavigationInput(const C3DVector& destination,float proximity,EMovementType movement) : Destination(destination),Proximity(proximity),MovementType(movement),Speed(0.0f) {}
inline void CBrainOutput::SetAsMoveToPos(const C3DVector& pos,const CNavigationInput& input) { Types |= 1; MoveToPos = pos; NavigationInput = input; }
inline void CBrainOutput::SetAsFaceMovementDirection() { Types |= 0x10; LookFlags = 0x51; }
namespace { CThing* FABLE_FASTCALL GetPBestTargetToFaceForCreature(const CThingCreatureBase&); }
template<class T> inline bool FABLE_FASTCALL IS_THING_DEAD(const T& thing){if(thing!=0) { if(!thing->IsDead()) return false; } return true;}
EAIActionReturn CAIStateGroupBase::AIAction_MoveToThingProximity(
    EAIActionRepeat type, CIntelligentPointer<const CThing> target_thing,
    float proximity, bool& finished_calculating_route, EMovementType movement_type,
    bool face_movement_direction, bool want_line_of_sight, bool avoid_dynamic_obstacles,
    bool reduce_speed_when_near, float proximity_for_direct_approach)
{
    const float MIN_DISTANCE_OF_OLD_AND_NEW_TARGET_POS_TO_RESTART_NAV = 8.5f;
    const float MIN_DISTANCE_FROM_TARGET_TO_NOT_RESTART_NAV = 3.5f;
    SetThingToNotAvoid(target_thing.Get());
    SetProximityForDirectApproach(proximity_for_direct_approach);
    if (IsStopping())
        return AI_ACTION_TERMINATE_PROCESS;
    EAIActionReturn ret = AI_ACTION_IN_PROGRESS;
    if (IS_THING_DEAD(target_thing))
        return AI_ACTION_FAILED;
    bool look_at_interesting_things = true;
    do {
        const CThing* plookat_target = GetPBestTargetToFaceForCreature(GetCreature());
        CTCLook* ptc_look;
        if (GetCreature().GetTC(&ptc_look))
            ptc_look->TrackNearestInterestingThing(look_at_interesting_things);
        if (IS_THING_ALIVE(target_thing)) {
            C3DVector target_pos = target_thing->GetPos();
            CTCCreatureNavigation* pnav;
            GetCreature().GetTC(&pnav);
            if (pnav->IsNewDestinationGoingToResetNavigation(target_pos, proximity)) {
                C3DVector old_target_pos = pnav->GetCurrentDestination();
                if ((GetCreature().GetPos() - old_target_pos).GetSquaredMagnitude() >
                        GFSqr(GetCreature().GetPhysicsTC()->GetRadius() + proximity) &&
                    GFGet2DDistanceSquared(old_target_pos, target_pos) <
                        GFSqr(MIN_DISTANCE_OF_OLD_AND_NEW_TARGET_POS_TO_RESTART_NAV) &&
                    GFGet2DDistanceSquared(GetCreature().GetPos(), target_pos) >
                        GFSqr(MIN_DISTANCE_FROM_TARGET_TO_NOT_RESTART_NAV))
                    target_pos = old_target_pos;
            }
            CNavigationInput nav_input(target_pos, proximity, movement_type);
            CTCCreatureNavigation::CCreatureNavOutput nav_out = pnav->GetNextNavPosOnRoute(
                nav_input, want_line_of_sight, avoid_dynamic_obstacles, reduce_speed_when_near, true, false);
            finished_calculating_route = nav_out.NavResult == NAV_REACHED_TARGET || nav_out.MovementVector.IsNonZero();
            if (!GetCreature().IsPerformingAnyAction() || !GetCreature().PeekPCurrentAction()->IsPreventingNormalMovement()) {
                switch (nav_out.NavResult) {
                case NAV_MOVING_TO_TARGET: {
                    CBrainOutput output;
                    C3DVector move_to_pos = GetCreature().PeekPos() + nav_out.MovementVector;
                    output.SetAsMoveToPos(move_to_pos, nav_input);
                    if (IS_THING_ALIVE(plookat_target))
                        output.SetAsLookAtThing(*plookat_target, 0x97);
                    else if (face_movement_direction) {
                        output.SetAsFaceMovementDirection();
                        look_at_interesting_things = false;
                    }
                    PBrain->SetOutput(output);
                }
                    ret = AI_ACTION_IN_PROGRESS;
                    break;
                case NAV_REACHED_TARGET:
                    ret = AI_ACTION_FINISHED;
                    break;
                case NAV_CANNOT_REACH_TARGET:
                    ret = AI_ACTION_FAILED;
                    break;
                }
            }
        } else {
            ret = AI_ACTION_FAILED;
        }
        PBrain->SuspendSuspendableProcess();
        if (IsStopping())
            return AI_ACTION_TERMINATE_PROCESS;
    } while (type == ACTION_REPEAT_UNTIL_FINISHED && ret == AI_ACTION_IN_PROGRESS);
    return ret;
}