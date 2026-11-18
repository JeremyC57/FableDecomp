// CAIStateGroup_HoverHornetQueen::DiveBomb() -- mark the timing critical; fly to the next circle marker in the route direction (reversing it when the route fails) or, with no markers, close in on the target; then, when the target is within 12 units on the ground plane and the point 0.75 short of it is in navigable line of sight, perform the dive attack and return to the current circle marker, reversing the route
// Leaf: ?DiveBomb@CAIStateGroup_HoverHornetQueen@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0095B9E0, 782 bytes, `ret`. Ego_r.pdb / FableWin.pdb ?DiveBomb@CAIStateGroup_HoverHornetQueen@@IAEXXZ;
//   Ego_d.pdb hover queen object (locals pTCNav / ret / lineToTarget / lineOfSightPos / directionToTarget, block { nav }).
// Callees: IsStillRunnable 0x0095A420, CAIStateGroupBase::Terminate 0x008FCF40, GotoCirclePoint 0x0095A6C0 (Get/GotoNext/PreviousCirclePoint
//   are the class's own .cpp helpers, defined below in Ego_d's object order and expanded here; retail also keeps them out of line at 0x0095A3D0 / 0x0095A3F0 / 0x0095A9A0 / 0x0095A9D0, and this text grades MATCH / MATCH / RELOCATION_MATCH / RELOCATION_MATCH against them (audit e11); the catalog address 0x00C1B460 given for GetNextCirclePoint is an unrelated function),
//   AIAction_MoveToThingProximity 0x0097E380 (the CIntelligentPointer<const CThing> argument built in its slot by the inline converting
//   constructor: 0x00A01B10 / vptr 0x01237F1C / GetPItem 0x00A01B50 / SetPItem 0x00A01B90), the expanded const PeekTC (LowerBound
//   0x004365B0), CThing::GetPos 0x004C73D0, the inline C3DVector Normalise / GFFastOneOverSqrt (table 0x013CE558) / GetMagnitude2D,
//   CTCCreatureNavigation::IsPosNavLineOfSightTo 0x006CBA20, CCreatureAction_WaspQueenDiveAttack ctor 0x008C5410 / ~CCombatActionBase
//   0x0062E900, AIAction_CombatPerformActionAndWait 0x0097DA40.
// Shape: the TU's landed sibling Attack 0x0095BCF0 include set (pruned to what the row uses) with the DEFAULT C3DVector subtraction /
//   scale and GFFastOneOverSqrt texts (Attack's SUBTRACT_XY_GROUP / NEWTON_GROUPED give a one-load scheduling swap at 0x0095BC13..
//   0x0095BC45 here; a 45-way sweep of the existing no-code opt-ins matched only without them), GetMagnitude2D as the one-expression
//   FableWin body (the two-statement body swaps the X*X / Y*Y frame words). Ego_d locals: lineToTarget / directionToTarget are
//   copy-initialised where they are computed, lineOfSightPos is assigned (retail builds it in a temporary and copies it, 0x0095BC13).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_COMBATPERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_HOVERHORNETQUEEN_ATTACK_PASS23
#define FABLE_RETAIL_CAISTATEGROUP_HOVERMOVEMENTBASE_ATTACK_PASS23
#define FABLE_RETAIL_CAISTATEGROUP_HOVERHORNETQUEEN_DIVEBOMB
#define FABLE_RETAIL_CCREATUREACTION_WASPQUEENDIVEATTACK_BASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_ONEPOINTLOS
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_HoverHornetQueen.h"
#include "engine/CCreatureAction_WaspQueenDiveAttack.h"
#include "engine/CTCCreatureNavigation.h"

long CAIStateGroup_HoverHornetQueen::GetNextCirclePoint()
{
    long nextCirclePoint = CurrentCirclePoint + 1;
    if (nextCirclePoint == (long)CircleMarkers.size())
        nextCirclePoint = 0;
    return nextCirclePoint;
}

long CAIStateGroup_HoverHornetQueen::GetPreviousCirclePoint()
{
    long nextCirclePoint = CurrentCirclePoint - 1;
    if (nextCirclePoint == -1)
        nextCirclePoint = (long)CircleMarkers.size() - 1;
    return nextCirclePoint;
}

EAIActionReturn CAIStateGroup_HoverHornetQueen::GotoNextCirclePoint(EMovementType movement_type)
{
    return GotoCirclePoint(GetNextCirclePoint(), movement_type);
}

EAIActionReturn CAIStateGroup_HoverHornetQueen::GotoPreviousCirclePoint(EMovementType movement_type)
{
    return GotoCirclePoint(GetPreviousCirclePoint(), movement_type);
}

void CAIStateGroup_HoverHornetQueen::DiveBomb()
{
    const CTCCreatureNavigation* pTCNav;
    EAIActionReturn ret;
    C3DVector lineOfSightPos;
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    TimingCritical = true;
    if (CircleMarkers.size() > 0) {
        if (RouteDirection == WASP_DIRECTION_CLOCKWISE)
            ret = GotoNextCirclePoint(AI_MOVEMENT_HOVER_MEDIUM);
        else
            ret = GotoPreviousCirclePoint(AI_MOVEMENT_HOVER_MEDIUM);
        if (ret == AI_ACTION_FAILED)
            RouteDirection = (eWaspDirection)(RouteDirection == WASP_DIRECTION_CLOCKWISE);
        if (!IsStillRunnable()) {
            Terminate();
            TimingCritical = false;
            return;
        }
    } else {
        do {
            bool nav;
            ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, PTargetThing, 10.0f, nav, AI_MOVEMENT_HOVER_MEDIUM, true, false, true, true, 0.0f);
        } while (ret == AI_ACTION_IN_PROGRESS && IsStillRunnable());
    }
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    GetCreature().PeekTC(TCI_NAVIGATION, &pTCNav);
    C3DVector lineToTarget = PTargetThing.Get()->GetPos() - GetCreature().GetPos();
    C3DVector directionToTarget = lineToTarget;
    directionToTarget.Normalise();
    lineOfSightPos = PTargetThing.Get()->GetPos() - directionToTarget * 0.75f;
    if (lineToTarget.GetMagnitude2D() < 12.0f && pTCNav->IsPosNavLineOfSightTo(lineOfSightPos)) {
        AIAction_CombatPerformActionAndWait(CCreatureAction_WaspQueenDiveAttack(GetCreature(), *PTargetThing.Get()), false);
        if (!IsStillRunnable()) {
            TimingCritical = false;
            Terminate();
            return;
        }
        GotoCirclePoint(CurrentCirclePoint, AI_MOVEMENT_HOVER_MEDIUM);
        RouteDirection = (eWaspDirection)(RouteDirection == WASP_DIRECTION_CLOCKWISE);
    }
    TimingCritical = false;
    if (!IsStillRunnable())
        Terminate();
}