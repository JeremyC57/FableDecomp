// 0094dd90 CAIStateGroup_RockTroll::Update
// Leaf: ?Update@CAIStateGroup_RockTroll@@MAEXXZ
// Flags:
#define FABLE_RETAIL_ROCKTROLL_UPDATE_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE_OUTOFLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION_NOWAIT
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_SUSPENDABLE_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION
#define FABLE_RETAIL_CCREATUREACTION_TROLLGROUNDPOUND_CTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_RESETCOMBO
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETHANDEDNESS
#define FABLE_RETAIL_CAISTATEGROUP_TROLLBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#include "fable_string.h"
#include "engine/EAIAction.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTrollBattleDef.h"
#include "engine/CCreatureAction_PlayCombatAnimation.h"
#include "engine/CCreatureAction_TrollGroundPound.h"
#include "engine/CWorld.h"
#include "engine/ECombatAbilityDataType.h"
#include "engine/CTCCombat.h"
#include "engine/ThingAlive.h"
#include "engine/CAIStateGroup_TrollBase.h"
#include "engine/CAIStateGroup_RockTroll.h"

void CAIStateGroup_RockTroll::Update()
{
    ForceRiseUp();
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    CDefPointer<const CTrollBattleDef> ptroll_battle_def;
    GetCreature().PeekBaseDef().GetSubDef(&ptroll_battle_def);
    bool knocked_down = false;
    EAIActionReturn ret;
    if (IS_THING_ALIVE(PEnemy)) {
        if (!IsStillRunnable()) {
            Terminate();
            return;
        }
        C3DVector vector_to_enemy = GetEnemyPredictedPos() - GetCreature().GetPos();
        float dis_to_enemy = vector_to_enemy.GetMagnitude2D();
        if (dis_to_enemy < TROLL_MIN_THROWING_DISTANCE) {
            C3DVector enemy_direction = vector_to_enemy;
            enemy_direction.Normalise();
            if (GetCreature().GetPhysicsTC()->GetFacingVector().Dot(enemy_direction) < 0.5f) {
                ret = AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), "LOOK_BACKWARD", true), false);
            } else {
                ret = AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), "LOOK_FORWARD", true), false);
            }
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            vector_to_enemy = GetEnemyPredictedPos() - GetCreature().GetPos();
            dis_to_enemy = vector_to_enemy.GetMagnitude2D();
            if (dis_to_enemy < TROLL_MIN_THROWING_DISTANCE) {
                ret = AIAction_PerformAction(CCreatureAction_TrollGroundPound(GetCreature(), *PEnemy));
                while (GetCreature().IsPerformingAnyAction()) {
                    if (!IsStillRunnable()) {
                        Terminate();
                        return;
                    }
                    if (HasEnemyBeenKnockedDown())
                        knocked_down = true;
                    PBrain->SuspendSuspendableProcess();
                }
            }
        } else if (dis_to_enemy <= TROLL_MAX_THROWING_DISTANCE) {
            AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, PEnemy, 0x11, 0.2f);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            ShotCounter = 0;
            do {
                if (ShotCounter == 0) {
                    EnemyKnockDownFrame = GetWorld().GetFrame();
                    CTCCombat* ptc_combat;
                    GetCreature().GetTC(&ptc_combat);
                    ptc_combat->ResetComboChainPos();
                    GetCreature().SetLastActionHandedness(static_cast<ECombatAnimationHandedness>(0));
                }
                ret = ThrowRocks();
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                if (HasEnemyBeenKnockedDown())
                    knocked_down = true;
                if (ShotCounter == 4 && ret == AI_ACTION_FINISHED) {
                    CCreatureAction_PlayCombatAnimation action(GetCreature(), "SPECIAL_IDLE", true);
                    while (GetCreature().IsPerformingAnyAction()) {
                        if (!IsStillRunnable()) {
                            Terminate();
                            return;
                        }
                        if (HasEnemyBeenKnockedDown())
                            knocked_down = true;
                        PBrain->SuspendSuspendableProcess();
                    }
                    ShotCounter = 0;
                    if (!knocked_down)
                        ret = AIAction_PerformActionAndWait(action, false);
                }
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                vector_to_enemy = PEnemy->GetPos() - GetCreature().GetPos();
                dis_to_enemy = vector_to_enemy.GetMagnitude2D();
                if (HasEnemyBeenKnockedDown())
                    knocked_down = true;
            } while ((ret == AI_ACTION_IN_PROGRESS || ret == AI_ACTION_FINISHED) && dis_to_enemy >= TROLL_MIN_THROWING_DISTANCE && dis_to_enemy <= TROLL_MAX_THROWING_DISTANCE && !knocked_down);
            if (dis_to_enemy > TROLL_MAX_THROWING_DISTANCE) {
                Terminate();
                return;
            }
        } else {
            Terminate();
            return;
        }
        while (GetCreature().IsPerformingAnyAction()) {
            AIAction_DoNothing();
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
        }
        long time_since_knockdown = GetWorld().GetFrame() - EnemyKnockDownFrame;
        if (!IsStopping() && dis_to_enemy >= ptroll_battle_def->MinLaughDistance && knocked_down && time_since_knockdown < ptroll_battle_def->MaxTimeBetweenKnockdownAndBoast) {
            ret = AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), "SPECIAL_BOAST", true), false);
            EnemyKnockDownFrame = GetWorld().GetFrame();
        }
    } else {
        Terminate();
    }
    if (IsStopping()) {
        Terminate();
        return;
    }
}