// CAIStateGroup_MazeDefensiveMeleeCombat::Update
// Leaf: ?Update@CAIStateGroup_MazeDefensiveMeleeCombat@@UAEXXZ
// Flags:
#define FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_HEALTH_PERCENTAGE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_MATERIALISED
#define FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND
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
#define FABLE_RETAIL_CAISTATEGROUPBASE_PUMPCOMBATACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_SETOUTPUT
#define FABLE_RETAIL_CAISTATEGROUP_MAZEBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_COMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_COMBAT_DEFENDER_ZONE_PASS23
#define FABLE_RETAIL_CMAZEBATTLEDEF
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCBOSS_GETPHASESTARTPOS
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
#define FABLE_RETAIL_CCOMBATACTION_WILLSHIELDINTO
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLASS
#define FABLE_RETAIL_CCREATUREACTION_COMBATIDLE_MAZE
#define FABLE_RETAIL_CBRAINOUTPUT_ONLY

#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CWorld.h"
#include "engine/CCombatManager.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CAIStateGroup_MazeDefensiveMeleeCombat.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/ECombatZone.h"
#include "engine/CTCBoss.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCAICreatureWillPowerIndicator.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatAction_WillShieldInto.h"
#include "engine/CCombatAction_WillHealInto.h"
#include "engine/CCreatureAction_ThrowShotObject.h"
#include "engine/CCreatureAction_CombatIdle.h"
#include "engine/campfire_types.h"
#include "engine/CAIBrain.h"
#include "engine/_quarantine/CMazeBattleDef.h"
#include "engine/CDefClassBase_GetSubDef.h"

bool CAIStateGroup_MazeDefensiveMeleeCombat::IsStillRunnable()
{
    if (!IS_THING_ALIVE(PTargetThing) || IsStopping())
        return false;
    return true;
}

void CAIStateGroup_MazeDefensiveMeleeCombat::Update()
{
    CDefPointer<const CMazeBattleDef> p_maze_def;
    GetCreature().PeekBaseDef().GetSubDef(&p_maze_def);
    while (IsStillRunnable()) {
        const C3DVector vector_to_target = PTargetThing->GetPos() - GetCreature().GetPos();
        const float dis_to_target = vector_to_target.GetMagnitude2D();
        const CTCAICreatureWillPowerIndicator* ptc_power_indicator;
        if (GetCreature().PeekTC(&ptc_power_indicator) && ptc_power_indicator->IsPoweredUp()) {
            AIAction_PerformActionAndWait(CCombatAction_WillShieldInto(GetCreature(), p_maze_def->ShieldUpTime, 5.0f), false);
            Terminate();
            return;
        }
        if (dis_to_target < GetCreature().GetCombatTypeDef().FrontMiddleLimit) {
            if (GetCreature().IsInMeleeAsAttacker()) {
                if (PTargetThing->GetTypeFlags() & 8) {
                    CThingCreatureBase* pthing_creature = GFDebugPDynamicCast(PTargetThing.Get(), (CThingCreatureBase*)0);
                    ECombatZone cur_zone = GetWorld().GetCombatManager().GetDefenderZone(*pthing_creature, GetCreature(), 0);
                    switch (cur_zone) {
                    case COMBAT_ZONE_FRONT_FAR:
                        break;
                    case COMBAT_ZONE_FRONT_MIDDLE:
                    case COMBAT_ZONE_FRONT_NEAR:
                        break;
                    default:
                        AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, PTargetThing, 0x21, 1.0f / 12.0f);
                        if (!IsStillRunnable()) {
                            Terminate();
                            return;
                        }
                        break;
                    }
                    AIAction_PumpCombatActions();
                }
            }
        } else if (dis_to_target < p_maze_def->MinThrowShotRange) {
            CTCBoss* ptc_boss;
            GetCreature().GetTC(&ptc_boss);
            CTCCreatureModeManager* ptc_cmm;
            GetCreature().GetTC(&ptc_cmm);
            if (GetCreature().IsAbleToStrafe() && !ptc_cmm->IsModeActive(0x19))
                ptc_cmm->AddMode(0x19);
            C3DVector phase_start_pos = ptc_boss->GetPhaseStartPos();
            bool nav;
            AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, phase_start_pos, 1.0f, nav, AI_MOVEMENT_RUNNING,
                                        false, false, true, 0, true, 0.0f);
            if (GetCreature().IsAbleToStrafe() && IsStillRunnable()) {
                CBrainOutput output(PBrain->GetOutput());
                output.SetAsLookAtThing(*PTargetThing, 0xa7);
                PBrain->SetOutput(output);
            }
        } else if (dis_to_target < 20.0f) {
            float time_since_last_heal = (float)(GetWorld().GetFrame() - LastHealFrame) / CMainGameComponent::Get()->GetConstantFPS();
            if (GetCreature().GetHealthPercentage() < 1.0f && time_since_last_heal > p_maze_def->MinTimeBetweenHeals) {
                AIAction_PerformActionAndWait(CCombatAction_WillHealInto(GetCreature(), p_maze_def->HealSpellLevel), false);
                LastHealFrame = GetWorld().GetFrame();
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
            } else {
                AIAction_PerformActionAndWait(CCreatureAction_ThrowShotObject(GetCreature(), *PTargetThing), false);
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                if (PTargetThing->GetTypeFlags() & 8) {
                    CThingCreatureBase* pthing_creature = GFDebugPDynamicCast(PTargetThing.Get(), (CThingCreatureBase*)0);
                    AIAction_PerformActionAndWait(CCreatureAction_CombatIdle(GetCreature(), *pthing_creature), false);
                }
            }
        } else {
            if (PTargetThing->GetTypeFlags() & 8) {
                CThingCreatureBase* pthing_creature = GFDebugPDynamicCast(PTargetThing.Get(), (CThingCreatureBase*)0);
                AIAction_PerformActionAndWait(CCreatureAction_CombatIdle(GetCreature(), *pthing_creature), false);
            }
        }
    }
    Terminate();
}