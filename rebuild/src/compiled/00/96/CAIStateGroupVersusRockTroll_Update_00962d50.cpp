// 00962d50 CAIStateGroup_VersusRockTroll::Update
// Leaf: ?Update@CAIStateGroup_VersusRockTroll@@MAEXXZ
// Flags: /O2 /Oy /Gr
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_HASTC_MATERIALISED
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_SETOUTPUT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_CBRAINOUTPUT_ONLY

#define FABLE_RETAIL_VERSUS_ENEMY_ACTIONS_PASS29
#define FABLE_RETAIL_CAISTATEGROUP_VERSUSENEMYBASE
#define FABLE_RETAIL_CAISTATEGROUP_VERSUSSCORPIONKING
#define FABLE_RETAIL_CCREATUREACTION_INTERRUPTABLEMIDATTACK_PROTOTYPE
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_ACCESS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CBRAINOUTPUT_INLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CCLASS
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CWorld.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/NCreatureMode.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_InterruptableMidAttack.h"
#include "engine/campfire_types.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/GFRandom.h"
#include "engine/CAIStateGroup_VersusEnemyBase.h"
#include "engine/ThingAlive.h"
#include "engine/CAIStateGroup_VersusRockTroll.h"
namespace {
const float ROCK_TROLL_SAFE_DISTANCE = 10.5f;
const float MOVE_SEGMENT_DISTANCE = 2.0f;
}
void CAIStateGroup_VersusRockTroll::Update()
{
    EAIActionReturn ret=AI_ACTION_FINISHED;
    if (IsStillRunnable()) {
        CTCCreatureModeManager* ptc_cmm;
        GetCreature().GetTC(&ptc_cmm);
        C3DVector move_to_pos;
        C3DVector vector_to_target=PTargetThing->GetPos()-GetCreature().GetPos();
        C3DVector dir_to_target(vector_to_target);
        dir_to_target.Normalise();
        if (vector_to_target.GetMagnitude2D()>0.8f && GFRandom(100,GetWorld().GetWorldSeed())<30) {
            if (ptc_cmm->IsModeActive(NCreatureMode::MODE_STRAFE)) ptc_cmm->RemoveMode(NCreatureMode::MODE_STRAFE);
            const float attack_dis=PTargetThing->GetPhysicsTC()->GetRadius()+3.5f;
            const C3DVector start_pos=GetCreature().GetPos();
            bool nav;
            ret=AIAction_MoveToThingProximity(ACTION_REPEAT_UNTIL_FINISHED,PTargetThing,attack_dis,nav,AI_MOVEMENT_RUNNING,true,true,true,false,0.0f);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            vector_to_target=PTargetThing->GetPos()-GetCreature().GetPos();
            if (vector_to_target.GetMagnitude2D()<attack_dis+0.2f)
                ret=AIAction_PerformActionAndWait(CCreatureAction_InterruptableMidAttack(GetCreature(),*PTargetThing),false);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            ret=AIAction_MoveToPosProximity(ACTION_REPEAT_UNTIL_FINISHED,start_pos,2.0f,nav,AI_MOVEMENT_RUNNING,true,true,true,0,false,0.0f);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
        } else {
            if (GetCreature().IsAbleToStrafe() && !ptc_cmm->IsModeActive(NCreatureMode::MODE_STRAFE)) ptc_cmm->AddMode(NCreatureMode::MODE_STRAFE);
            C3DVector dir_to_move(dir_to_target);
            if (GoClockwise) dir_to_move.RotateXY(0.25f); else dir_to_move.RotateXY(-0.25f);
            move_to_pos=PTargetThing->GetPos()-dir_to_target*ROCK_TROLL_SAFE_DISTANCE+dir_to_move*MOVE_SEGMENT_DISTANCE;
            CTCCreatureNavigation* ptc_nav;
            GetCreature().GetTC(&ptc_nav);
            if (!ptc_nav->IsAbleToNavigateFromPosToPos(GetCreature().GetPos(),move_to_pos,2.0f)) {
                dir_to_move.RotateXY(-0.5f);
                move_to_pos=PTargetThing->GetPos()-dir_to_target*ROCK_TROLL_SAFE_DISTANCE+dir_to_move*MOVE_SEGMENT_DISTANCE;
            }
            do {
                bool nav;
                ret=AIAction_MoveToPos(ACTION_REPEAT_CALLBACK,move_to_pos,nav,AI_MOVEMENT_WALKING,!GetCreature().IsAbleToStrafe(),true,0,false,0.0f);
                if (GetCreature().IsAbleToStrafe() && IsStillRunnable()) {
                    CBrainOutput output(PBrain->GetOutput());
                    output.SetAsLookAtThing(*PTargetThing,0xa7);
                    PBrain->SetOutput(output);
                }
            } while(ret==AI_ACTION_IN_PROGRESS && IsStillRunnable());
        }
    }
    if (!IsStillRunnable()) Terminate();
}