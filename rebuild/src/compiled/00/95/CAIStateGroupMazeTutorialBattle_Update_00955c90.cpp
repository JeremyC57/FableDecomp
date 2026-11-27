// 00955c90 CAIStateGroup_MazeTutorialBattle::Update
// Leaf: ?Update@CAIStateGroup_MazeTutorialBattle@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_MAZE_TUTORIAL_PASS29
#define FABLE_RETAIL_CAISTATEGROUP_MAZETUTORIALBATTLE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUP_MAZEBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CTCBOSS_ISREADYFORPHASECHANGE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER_PROTECTED
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CCREATUREACTION_COMBATBOAST_MAZE
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
// CAIStateGroup_MazeTutorialBattle::Update
#define FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
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
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_MAZEBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_COMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_COMBAT_DEFENDER_ZONE_PASS23
#define FABLE_RETAIL_CMAZEBATTLEDEF
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
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

#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CWorld.h"
#include "engine/CCombatManager.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CAIStateGroup_MazeTutorialBattle.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"
#include "engine/ECombatZone.h"
#include "engine/CTCBoss.h"
#include "engine/CTCAICreatureWillPowerIndicator.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_CombatBoast.h"
#include "engine/ECombatAbilityDataType.h"
#include "engine/CTCCombat.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/NCreatureMode.h"
#include "engine/campfire_types.h"
#include "engine/CAIBrain.h"
#include "engine/_quarantine/CMazeBattleDef.h"
#include "engine/CDefClassBase_GetSubDef.h"

inline bool CAIStateGroup_MazeTutorialBattle::IsStillRunnable()
{
    if (!IS_THING_ALIVE(PTargetThing) || IsStopping()) return false;
    return true;
}
inline bool CAIStateGroup_MazeBase::HasEnemyBeenKnockedDown()
{
    if (NCreatureMode::IsKnockedDown(*PTargetThing)) return true;
    return false;
}
void CAIStateGroup_MazeTutorialBattle::Update()
{
    if (!IsStillRunnable()) { Terminate(); return; }
    CTCBoss* ptc_boss; GetCreature().GetTC(&ptc_boss);
    CDefPointer<const CMazeBattleDef> p_maze_def;
    GetCreature().PeekBaseDef().GetSubDef(&p_maze_def);
    CTCCombat* ptc_combat; GetCreature().GetTC(&ptc_combat);
    switch (ptc_boss->GetBossPhase()) {
    case 0: case 1: case 2: {
        CTCAICreatureWillPowerIndicator* ptc_power_indicator;
        if (GetCreature().GetTC(&ptc_power_indicator)) ptc_power_indicator->Reset(10.0f);
        CDefPointer<const CCombatTypeDef> p_combat_type_def;
        PeekDefinitionManager().GetDef("HERO_MAZE_TUTORIAL_ATTACK_STYLE2", &p_combat_type_def);
        ptc_combat->SetCombatType(p_combat_type_def);
        float dis_to_target=(GetCreature().GetPos()-PTargetThing->GetPos()).GetMagnitude();
        if(dis_to_target < GetCreature().GetCombatTypeDef().FrontMiddleLimit) {
            if(GetCreature().IsInMeleeAsAttacker() && (PTargetThing->GetTypeFlags() & 8)) {
                CThingCreatureBase* pthing_creature=GFDebugPDynamicCast(PTargetThing.Get(),(CThingCreatureBase*)0);
                ECombatZone cur_zone=GetWorld().GetCombatManager().GetDefenderZone(*pthing_creature,GetCreature(),0);
                switch(cur_zone) {
                case COMBAT_ZONE_FRONT_FAR: break;
                case COMBAT_ZONE_FRONT_MIDDLE: case COMBAT_ZONE_FRONT_NEAR: break;
                default:
                    AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED,PTargetThing,0x21,1.0f/12.0f);
                    if(!IsStillRunnable()) { Terminate();return; }
                    break;
                }
                AIAction_PumpCombatActions();
            }
        } else {
            AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED,PTargetThing,0x21,1.0f/12.0f);
            if(!IsStillRunnable()) { Terminate();return; }
        }
        if(ptc_boss->IsReadyForPhaseChange()) ptc_boss->SetNextBossPhase(ptc_boss->GetBossPhase()+1,false);
        break;
    }
    default: AIAction_DoNothing();break;
    }
    if(IsStillRunnable() && (PTargetThing->GetTypeFlags() & 8)) {
        CThingCreatureBase* pthing_creature=GFDebugPDynamicCast(PTargetThing.Get(),(CThingCreatureBase*)0);
        if(HasEnemyBeenKnockedDown()) AIAction_PerformActionAndWait(CCreatureAction_CombatBoast(GetCreature(),*pthing_creature),false);
    }
}