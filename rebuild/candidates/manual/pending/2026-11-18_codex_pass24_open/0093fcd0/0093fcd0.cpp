// CAIStateGroup_VillageTaskSecuritySweep::Update accepts and performs a village security sweep.
// Grade: DIFFER(1053v1053). The frame size, task-pointer CPU operand assignment and look timer spills differ. The emitted leaf has the retail length.
// Leaf: ?Update@CAIStateGroup_VillageTaskSecuritySweep@@UAEXXZ
// Flags: none
// Retail: Fable.exe 0x0093FCD0, 1053 bytes.

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CVILLAGETASKSECURITYSWEEP
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHEWEAPONS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_THINGFLAGS
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLEOPS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_VILLAGETASK_SECURITYSWEEP_UPDATE_PASS24
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/EUnsheatheWeaponType.h"
#include "engine/CCreatureAction_UnsheatheWeapons.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CVillageTaskSecuritySweep.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CAIStateGroup_VillageTaskSecuritySweep.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/ThingAlive.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/GFRandom.h"
#include "engine/GFFloatRandom.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"

void CAIStateGroup_VillageTaskSecuritySweep::Update()
{
    if (!PTask.Get()) {
        Terminate();
        return;
    }
    CTCVillage* pvillage = PTCVillageMember->GetPVillage();
    if (!pvillage->SetSecuritySweepTaskAsAcceptedByThing(*PTask, GetCreature())) {
        Terminate();
        return;
    }
    if (!CTCCarrying::IsThingCarryingWeapon(GetCreature()))
        AIAction_PerformActionAndWait(CCreatureAction_UnsheatheWeapons(GetCreature()), false);
    const CTCPerceiveThing* ptc_perception = 0;
    GetCreature().PeekTC(TCI_PERCEIVE_THING, &ptc_perception);
    EAIActionReturn ret;
    if (((IS_THING_ALIVE(ptc_perception->GetPerceivedThing()) && ptc_perception->IsThingHeard()) ||
         ptc_perception->IsThingLineOfSight() || ptc_perception->IsThingSmelt()) &&
        (ret = AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, ptc_perception->GetPerceivedThing(), 35, 0.0027777f))
            == AI_ACTION_TERMINATE_PROCESS) {
    } else {
        ret = AI_ACTION_IN_PROGRESS;
    }
    bool nav_route_calculated = false;
    while (ret == AI_ACTION_IN_PROGRESS) {
        if (!PTask.Get()) {
            Terminate();
            return;
        }
        ret = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK, PTask->GetPositionToGoTo(),
            nav_route_calculated, AI_MOVEMENT_RUNNING, true, true, 0, true, 0.0f);
    }
    if (ret != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    unsigned long* pworld_seed = GetPWorldSeed();
    do {
        long num_looks_before_moving = GFRandom(5, pworld_seed) + 2;
        while (num_looks_before_moving <= 0) {
            --num_looks_before_moving;
            if (ret == AI_ACTION_TERMINATE_PROCESS)
                break;
            if (!PTask.Get()) {
                Terminate();
                return;
            }
            float t_angle = GFFloatRandom(PTask->GetAngleToLookTowards(), pworld_seed);
            t_angle -= PTask->GetAngleToLookTowards() * 0.5f;
            C3DVector vector_to_look = PTask->GetPositionToLookTowards() - PTask->GetPositionToGoTo();
            vector_to_look.RotateXY(t_angle);
            C3DVector pos_to_look_at = PTask->GetPositionToGoTo() + vector_to_look;
            unsigned long frame_to_stop_looking = CMainGameComponent::GetConstantFPS() * 2;
            frame_to_stop_looking += CMainGameComponent::GetConstantFPS() * GFRandom(3, pworld_seed) + CWorld::GetFrame();
            do {
                ret = AIAction_LookAtPos(ACTION_REPEAT_CALLBACK, pos_to_look_at, 11, 0.0027777f);
            } while (static_cast<unsigned long>(CWorld::GetFrame()) < frame_to_stop_looking &&
                     ret != AI_ACTION_TERMINATE_PROCESS);
        }
        if (!PTask.Get()) {
            Terminate();
            return;
        }
        CTCCreatureNavigation* pnav;
        GetCreature().GetTC(TCI_NAVIGATION, &pnav);
        C3DVector next_pos;
        if (pnav->GetNearbyWanderPosition(PTask->GetPositionToGoTo(),
            PTask->GetRadiusAroundThePositionToStay(), next_pos, false)) {
            bool nav_route_calculated2;
            ret = AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED, next_pos, nav_route_calculated2,
                AI_MOVEMENT_RUNNING, true, true, 0, true, 0.0f);
        }
    } while (ret != AI_ACTION_TERMINATE_PROCESS);
    Terminate();
}
