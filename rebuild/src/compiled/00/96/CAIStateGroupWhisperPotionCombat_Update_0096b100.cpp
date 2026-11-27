// 0096b100 CAIStateGroup_WhisperPotionCombat::Update
// Leaf: ?Update@CAIStateGroup_WhisperPotionCombat@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_NSTEALTH_EDANGERSTATUS
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_SQUARED_SUM
#define FABLE_RETAIL_WHISPER_POTION_COMBAT_PASS29
#define FABLE_RETAIL_CAISTATEGROUP_WHISPERPOTIONCOMBAT_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF
#define FABLE_RETAIL_CTHINGCREATUREBASE_UNSHEATHEMELEEWEAPON
#define FABLE_RETAIL_CTCBOSS_GETGENERICCOUNTER
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCENEMY
#define FABLE_RETAIL_CTCENEMY_ARETHINGSENEMIES
#define FABLE_RETAIL_CTCSTEALTH
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLASS
#define FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27
#define FABLE_RETAIL_CCREATUREACTION_COMBATBOAST_MAZE
#define FABLE_RETAIL_CCREATUREACTION_COMBATIDLE_MAZE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CBRAINOUTPUT_ONLY
#define FABLE_RETAIL_CCREATUREACTIONBASE_MELEE
#define FABLE_RETAIL_CCREATUREACTIONBASE_VSLOTS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAIBRAIN
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/ThingAlive.h"
#include "engine/CAIStateGroup_WhisperPotionCombat.h"
#include "engine/CCreatureActionBase.h"
#include "engine/campfire_types.h"
#include "engine/CAIBrain.h"

#include "engine/CThingBaseDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCBoss.h"
#include "engine/CTCStealth.h"
#include "engine/CTCEnemy.h"
#include "engine/NCreatureMode.h"
#include "engine/CWhisperBattleDef.h"
#include "engine/CSoundPair.h"
class CTCWeaponTrail;
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_ThrowShotObject.h"
#include "engine/CCreatureAction_CombatBoast.h"
#include "engine/CCreatureAction_CombatIdle.h"
#include "engine/CAIStateGroup_WhisperPotionCombat.h"
void CAIStateGroup_WhisperPotionCombat::Update()
{
    if (!CTCCarrying::IsThingCarryingWeapon(GetCreature()))
        GetCreature().UnsheatheMeleeWeapon();
    EAIActionReturn ret = AI_ACTION_NULL;
    CTCBoss* pBossTC;
    GetCreature().GetTC(TCI_BOSS, &pBossTC);
    if (!IS_THING_ALIVE(PTargetThing) || IsStopping()) { Terminate(); return; }
    CTCStealth* ptc_stealth;
    if (PTargetThing->GetTC(TCI_STEALTH, &ptc_stealth))
        ptc_stealth->SetAsInDanger(GetCreature(), NStealth::DANGER_STATUS_COMBAT);
    C3DVector vectorToTarget = GetCreature().GetPos() - PTargetThing->GetPos();
    bool can_jump = true;
    switch (pBossTC->GetBossPhase()) {
    case 1: can_jump = GetValidJumpPosition(Jump_to_pos); break;
    }
    if (can_jump && vectorToTarget.GetMagnitude2D() < GetCreature().GetCombatTypeDef().FrontFarLimit) {
        DisappearJump();
        if (!IS_THING_ALIVE(PTargetThing) || IsStopping()) { Terminate(); return; }
    }
    if (!IS_THING_ALIVE(PTargetThing)) { Terminate(); return; }
    if (NCreatureMode::IsAbleToBeAttacked(*PTargetThing)) {
        vectorToTarget = GetCreature().GetPos() - PTargetThing->GetPos();
        if (vectorToTarget.GetMagnitude2D() > GetCreature().GetCombatTypeDef().FrontFarLimit) {
            CThingCreatureBase* p_hero=GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            if (IS_THING_ALIVE(p_hero) &&
                (p_hero == PTargetThing.Get() ||
                 (p_hero->GetPos()-PTargetThing->GetPos()).GetMagnitude2D()>3.5f ||
                 CTCEnemy::AreThingsEnemies(*p_hero,GetCreature())))
                ret=AIAction_PerformActionAndWait(CCreatureAction_ThrowShotObject(GetCreature(),*PTargetThing),false);
            if (!IS_THING_ALIVE(PTargetThing) || IsStopping() || ret != AI_ACTION_FINISHED) { Terminate(); return; }
            switch (pBossTC->GetBossPhase()) {
            case 1: {
                CDefPointer<const CWhisperBattleDef> pdef;
                GetCreature().PeekBaseDef().GetSubDef(&pdef);
                pBossTC->SetGenericCounter(pBossTC->GetGenericCounter()+1);
                if (pBossTC->GetGenericCounter() >= pdef->NumPhase2Potions)
                    pBossTC->SetNextBossPhase(pBossTC->GetBossPhase()+1,false);
                break;
            }
            }
            vectorToTarget = GetCreature().GetPos() - PTargetThing->GetPos();
            if (vectorToTarget.GetMagnitude2D() < GetCreature().GetCombatTypeDef().FrontFarLimit && GetValidJumpPosition(Jump_to_pos)) {
                DisappearJump();
                if (!IS_THING_ALIVE(PTargetThing) || IsStopping()) { Terminate(); return; }
            }
        }
    } else {
        if((PTargetThing->GetPos()-GetCreature().GetPos()).GetMagnitude2D()>GetCreature().GetCombatTypeDef().FrontFarLimit)
            AIAction_PerformActionAndWait(CCreatureAction_CombatBoast(GetCreature(),*PTargetThing),false);
        else AIAction_PerformActionAndWait(CCreatureAction_CombatIdle(GetCreature(),*PTargetThing),false);
    }
    Terminate();
}