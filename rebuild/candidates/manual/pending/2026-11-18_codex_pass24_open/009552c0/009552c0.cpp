// CAIStateGroup_MazeShieldedChase::Update() -- chase a runnable target and choose a spell and follow-up combat action.
// Grade: DIFFER(861v861) -- definition cleanup and follow-up combat action tail merging differ.
// Leaf: ?Update@CAIStateGroup_MazeShieldedChase@@UAEXXZ
// Retail: 0x009552C0, 861 bytes, thiscall.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_MAZESHIELDEDCHASE
#define FABLE_RETAIL_CAISTATEGROUP_MAZEBASE
#define FABLE_RETAIL_CAISTATEGROUP_MAZEBASE_ENEMY_KNOCKDOWN
#define FABLE_RETAIL_CMAZEBATTLEDEF
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTION_WILLENFLAMEINTO_MAZE
#define FABLE_RETAIL_CCREATUREACTION_CASTWILLLIGHTNINGSPELL_MAZE
#define FABLE_RETAIL_CCREATUREACTION_COMBATBOAST_MAZE
#define FABLE_RETAIL_CCREATUREACTION_COMBATIDLE_MAZE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CWorld.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCombatAction_WillEnflameInto.h"
#include "engine/CCreatureAction_CastWillLightningSpell.h"
#include "engine/CCreatureAction_CombatBoast.h"
#include "engine/CCreatureAction_CombatIdle.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_MazeShieldedChase.h"
#include "engine/_quarantine/CMazeBattleDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/GFRandom.h"

void CAIStateGroup_MazeShieldedChase::Update()
{
    CDefPointer<const CMazeBattleDef> p_maze_def;
    GetCreature().PeekBaseDef().GetSubDef(&p_maze_def);
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }

    float max_distance_for_close_will = p_maze_def->LightningRange;
    float max_distance_for_will = p_maze_def->EnflameRange;
    float ideal_distance_for_will;
    if (GFRandom(100, GetWorld().GetWorldSeed()) > 50)
        ideal_distance_for_will = p_maze_def->EnflameRange + 0.5f;
    else
        ideal_distance_for_will = p_maze_def->EnflameRange - 1.0f;

    float dis_to_target = (GetCreature().GetPos() - PTargetThing->GetPos()).GetMagnitude();
    if (dis_to_target < 30.0f) {
        EAIActionReturn ret = AI_ACTION_FINISHED;
        if (dis_to_target > max_distance_for_close_will) {
            bool nav;
            ret = AIAction_MoveToThingProximity(ACTION_REPEAT_UNTIL_FINISHED, PTargetThing, ideal_distance_for_will, nav,
                AI_MOVEMENT_RUNNING, true, false, false, false, 0.0f);
        }
        if (ret == AI_ACTION_FINISHED) {
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, PTargetThing, 0x21, 1.0f / 12.0f);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            if ((GetCreature().GetPos() - PTargetThing->GetPos()).GetMagnitude() < max_distance_for_will)
                ret = AIAction_PerformActionAndWait(CCombatAction_WillEnflameInto(GetCreature(), 3), false);
            else
                ret = AIAction_PerformActionAndWait(CCreatureAction_CastWillLightningSpell(GetCreature(), 3,
                    p_maze_def->LightningDuration, *PTargetThing.Get()), false);
        }
        if (!IsStillRunnable() || ret != AI_ACTION_FINISHED) {
            Terminate();
            return;
        }
        if (PTargetThing->GetTypeFlags() & 8) {
            CThingCreatureBase* pthing_creature = static_cast<CThingCreatureBase*>(PTargetThing.Get());
            if (HasEnemyBeenKnockedDown())
                AIAction_PerformActionAndWait(CCreatureAction_CombatBoast(GetCreature(), *pthing_creature), false);
            else if ((GetCreature().GetPos() - PTargetThing->GetPos()).GetMagnitude2D() < max_distance_for_close_will)
                AIAction_PerformActionAndWait(CCreatureAction_CombatIdle(GetCreature(), *pthing_creature), false);
        }
    } else {
        AIAction_DoNothing();
    }
}
