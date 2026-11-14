// CAIStateGroup_JackOfBladesStage2::Update() -- the second Jack of Blades battle phase: face the arena, float up and grow to battle scale, wait a second, summon the pillars, then alternate fireball shots, flashes and idles (summoning minions when none live and taunting a knocked-down hero) until the boss is ready for phase 4
// Grade: DIFFER(3072v3080) -- wfD4: ported onto the shared CAIStateGroup_JackOfBladesBase / CJackOfBladesBattleDef / CTCBoss / CTCWillResponse headers; visible GetSubDef<T> body (CDEFCLASSBASE_GETSUBDEF_BODY, escape analysis keeps battleDef in edi, 2988->3056), implicit const char* CCharString conversions, HasTC materialised, GET_ROUNDED_ANGLE forceinline in the SetFacingAngleXY call; left: facing blocks keep diff X/Y in x87 regs (retail spills Y [esp+10h] / X [esp+28h], computes Z,Y,X), frame 0x808 v 0x80c, scale-up / fireball blocks, slot layout
// Next idea: find the GetTC spelling that stores the HasTC bit first (the CThing.h note: "a const bool& local stores it first", inside the GetTC body, i.e. a header opt-in), then walk the action-temporary slots and the fireball loop
// Leaf: ?Update@CAIStateGroup_JackOfBladesStage2@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00974AA0, 3080 bytes, thiscall `ret`, 0x80C frame; switch tables 0x00975678 (7 cases, counter % 7)
//   and 0x00975694 (5 cases, SpeechCounter % 7). Ego_r.pdb ?Update@CAIStateGroup_JackOfBladesStage2@@UAEXXZ (donor 0x00999CF0).
// Callees: CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410,
//   CVectorMap::LowerBound 0x0040F020 (inline CThing::GetTC for TCI_BOSS 173 / TCI_COMBAT 48 / TCI_TALK 15), CDefClassBase::GetSubDef
//   <CJackOfBladesBattleDef> 0x008C33E0, IsStillRunnable 0x00971B00, Terminate 0x008FCF40, IsStopping 0x008FD010, GetWorld 0x008FCFC0,
//   CWorld::GetFrame 0x0049D870, AIAction_PerformActionAndWait 0x0097DB70 / DoNothing 0x0097E200 / LookAtThing 0x0097D6F0, the
//   CCreatureAction_JackFloatUp 0x00977290 / JackShotFloat 0x00976F30 / JackFlash 0x00976E80 / PlayCombatAnimation 0x008B7F80 ctors and
//   ~CCombatActionBase 0x0062E900, CTCBoss SetNextBossPhase 0x007AEDA0 / IsReadyForPhaseChange 0x007AEE90 / CountLivingHelpers 0x007AEEC0 /
//   KillHelpers 0x007AF490, Summon 0x00974110, SummonPillars 0x00973DC0 / SummonPillarsOld 0x00973900, CTCWillResponse statics 0x007E4040 /
//   0x007E3D50, NCreatureMode::IsKnockedDown 0x0085F010, CTCTalk::IsTalking 0x006E28A0 / PlayAISpeech 0x006E3B30,
//   CTCGraphicAppearance::SetScale 0x004BFA50, CThing::GetPos 0x004C73D0, CBaseIntelligentPointer GetPItem / SetPItem, _CIfmod.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE
#define FABLE_RETAIL_JACKOFBLADESBASE_STAGE2_UPDATE
#define FABLE_RETAIL_JACKOFBLADESBASE_SUMMONPILLARS
#define FABLE_RETAIL_JACKOFBLADESBASE_SUMMONPILLARSOLD
#define FABLE_RETAIL_CTCBOSS_ISREADYFORPHASECHANGE
#define FABLE_RETAIL_CTCBOSS_KILLHELPERS
#define FABLE_RETAIL_CTCBOSS_LIVINGHELPERS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_GFANGLES_GET_ROUNDED_ANGLE_FORCEINLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CTHING_HASTC_MATERIALISED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_HITSETTERS
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_SHOTOVERRIDE
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_AISPEECHTEXT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SCALE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CWORLD
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCCombat.h"
#include "engine/CTCTalk.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CThingBaseDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFAngles.h"
#include "engine/CJackOfBladesBattleDef.h"
#include "engine/CTCBoss.h"
#include "engine/CTCWillResponse.h"
#include "engine/CAIStateGroup_JackOfBladesBase.h"

extern "C" double __cdecl atan2(double y, double x);
extern "C" double __cdecl fmod(double x, double y);
#pragma intrinsic(atan2, fmod)

enum EAIActionReturn { AI_ACTION_RETURN_2 = 2 };                     // enumerator names unproven (2 = the action completed)
enum EAIActionRepeat { AI_ACTION_REPEAT_1 = 1 };                     // enumerator names unproven




inline CCharString FABLE_FASTCALL CJackOfBladesBattleDef::StatGetTypeName() { return CCharString("CJackOfBladesBattleDef", -1); }   // FableWin ?StatGetTypeName@CJackOfBladesBattleDef@@SI?AVCCharString@@XZ, expanded in GetSubDef<CJackOfBladesBattleDef> 0x008C33E7

namespace NCreatureMode { bool __fastcall IsKnockedDown(const CThing& thing); }   // 0x0085F010

class CCreatureAction_JackFloatUp : public CCombatActionBase {      // RTTI: CCombatActionBase@0, retail sizeof 0x124
public:
    CCreatureAction_JackFloatUp(CThingCreatureBase& creature, float height);   // 0x00977290
    unsigned char _pad_0x114[0x124 - 0x114];
};
class CCreatureAction_JackShotFloat : public CCombatActionBase {    // retail sizeof 0x114
public:
    CCreatureAction_JackShotFloat(CThingCreatureBase& creature, CThingCreatureBase& target, const CCharString& animation);   // 0x00976F30
};
class CCreatureAction_JackFlash : public CCombatActionBase {        // retail sizeof 0x114
public:
    CCreatureAction_JackFlash(CThingCreatureBase& creature, CThingCreatureBase& target);   // 0x00976E80
};
class CCreatureAction_PlayCombatAnimation : public CCombatActionBase {   // retail sizeof 0x118
public:
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase& creature, CThingCreatureBase& target, const CCharString& animation, bool);   // 0x008B7F80
    unsigned char _pad_0x114[0x118 - 0x114];
};

class CAIStateGroup_JackOfBladesStage2 : public CAIStateGroup_JackOfBladesBase {
public:
    virtual void Update();
protected:
    CIntelligentPointer<CThing> PCutsceneLookAtMarker;               // +0x4c
};


void CAIStateGroup_JackOfBladesStage2::Update()
{
    long familyIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("JACK_BATTLE_PHASE_2_MINIONS_FAMILY");
    CTCBoss* boss;
    GetCreature().GetTC(&boss);
    CDefPointer<const CJackOfBladesBattleDef> battleDef;
    GetCreature().PeekBaseDef().GetSubDef(&battleDef);
    CTCCombat* combat;
    GetCreature().GetTC(&combat);
    long fireball = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("FIREBALL_JACK_OF_BLADES");
    long fireball2 = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("FIREBALL_JACK_OF_BLADES2");
    long fireball3 = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("FIREBALL_JACK_OF_BLADES3");
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    GetCreature().GetPhysicsTC()->ResetPosition(C3DVector(PArenaCentre->GetPos()));
    if (PCutsceneLookAtMarker.Get() && !PCutsceneLookAtMarker->IsDead()) {
        C3DVector diff = PCutsceneLookAtMarker->GetPos() - GetCreature().GetPos();
        if (diff.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
            GetCreature().GetPhysicsTC()->SetFacingAngleXY(GET_ROUNDED_ANGLE((float)atan2(diff.X, diff.Y) * 0.15915494f));
    }
    else {
        C3DVector diff = PTargetThing->GetPos() - GetCreature().GetPos();
        if (diff.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
            GetCreature().GetPhysicsTC()->SetFacingAngleXY(GET_ROUNDED_ANGLE((float)atan2(diff.X, diff.Y) * 0.15915494f));
    }
    GetCreature().GetPhysicsTC()->EnableGravity(false);
    if (AIAction_PerformActionAndWait(CCreatureAction_JackFloatUp(GetCreature(), battleDef->BattleFloatHeight), false) != AI_ACTION_RETURN_2) {
        Terminate();
        return;
    }
    float step = (battleDef->BattleScale - GetCreature().GetTCGraphicAppearance()->GetScale()) /
                 (CMainGameComponent::GetConstantFPS() * battleDef->ScaleUpTime);
    if (GetCreature().GetTCGraphicAppearance()->GetScale() < battleDef->BattleScale) {
        while (PTargetThing.Get() && !PTargetThing->IsDead() && !IsStopping() && PArenaCentre.Get() && !PArenaCentre->IsDead()) {
            float scale = step + GetCreature().GetTCGraphicAppearance()->GetScale();
            GetCreature().GetTCGraphicAppearance()->SetScale(scale);
            AIAction_DoNothing();
            if (!(scale < battleDef->BattleScale))
                break;
        }
    }
    GetCreature().GetTCGraphicAppearance()->SetScale(battleDef->BattleScale);
    boss->SetNextBossPhase(2, false);
    long startFrame = GetWorld().GetFrame();
    do {
        if (!PTargetThing.Get() || PTargetThing->IsDead() || IsStopping() || !PArenaCentre.Get() || PArenaCentre->IsDead()) {
            Terminate();
            return;
        }
        AIAction_DoNothing();
    } while ((float)(GetWorld().GetFrame() - startFrame) < (float)CMainGameComponent::GetConstantFPS());
    if (Pillars.size() > 0)
        SummonPillars();
    else
        SummonPillarsOld();
    boss->SetNextBossPhase(3, false);
    CTCWillResponse::SetThingAsNotBloodyWillableAtAll(&GetCreature(), false);
    CTCWillResponse::SetThingIsForcePushable(&GetCreature(), false);
    long counter = 6;
    long fireballs = 0;
    do {
        if (!PTargetThing.Get() || PTargetThing->IsDead() || !PArenaCentre.Get() || PArenaCentre->IsDead()) {
            Terminate();
            continue;
        }
        GetCreature().GetTCGraphicAppearance()->SetScale(battleDef->BattleScale);
        const C3DVector& arena = PArenaCentre->GetPos();
        GetCreature().GetPhysicsTC()->SetPosition(C3DVector(arena.X, arena.Y, battleDef->BattleFloatHeight + arena.Z));
        GetCreature().GetPhysicsTC()->EnableGravity(false);
        const C3DVector& from = GetCreature().GetPos();
        const C3DVector& to = PTargetThing->GetPos();
        float dy = to.Y - from.Y;
        float dx = to.X - from.X;
        if (dx * dx + dy * dy > 1.0f) {
            AIAction_LookAtThing(AI_ACTION_REPEAT_1, CIntelligentPointer<const CThing>(PTargetThing.Get()), 0x11, 0.05f);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
        }
        switch (counter % 7) {
        case 0:
            ++fireballs;
        case 1:
        case 2: {
            combat->ShotOverrideType = fireball;
            CCharString animation("ST_STRIKE_LEFT_FIREBALL", -1);
            AIAction_PerformActionAndWait(CCreatureAction_JackShotFloat(GetCreature(), *PTargetThing.Get(), animation), true);
            break;
        }
        case 3:
        case 4: {
            combat->ShotOverrideType = fireball2;
            CCharString animation("ST_STRIKE_COMBO_FIREBALL", -1);
            AIAction_PerformActionAndWait(CCreatureAction_JackShotFloat(GetCreature(), *PTargetThing.Get(), animation), true);
            break;
        }
        case 5:
            AIAction_PerformActionAndWait(CCreatureAction_JackFlash(GetCreature(), *PTargetThing.Get()), true);
            if (!boss->IsReadyForPhaseChange() && fireballs > 1) {
                if (boss->CountLivingHelpers() == 0)
                    Summon(familyIndex, false);
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                CCharString animation("ST_IDLE", -1);
                AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), *PTargetThing.Get(), animation, true), true);
            }
            break;
        case 6: {
            CCharString animation("ST_IDLE", -1);
            AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), *PTargetThing.Get(), animation, true), true);
            break;
        }
        }
        if (!IsStillRunnable()) {
            Terminate();
            return;
        }
        if (!boss->IsReadyForPhaseChange()) {
            {
                CCharString animation("ST_IDLE", -1);
                AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), *PTargetThing.Get(), animation, true), true);
            }
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            if (NCreatureMode::IsKnockedDown(*PTargetThing.Get())) {
                CTCTalk* talk;
                if (GetCreature().GetTC(&talk) && !talk->IsTalking()) {
                    switch (SpeechCounter % 7) {
                    case 0: talk->PlayAISpeech("TEXT_QST_039_JACK_BOSS_FINAL_TAUNT_10", true, (NSpeechGainManager::EDialogueLayer)0, false, true); break;
                    case 1: talk->PlayAISpeech("TEXT_QST_039_JACK_BOSS_FINAL_TAUNT_20", true, (NSpeechGainManager::EDialogueLayer)0, false, true); break;
                    case 2: talk->PlayAISpeech("TEXT_QST_039_JACK_BOSS_FINAL_TAUNT_30", true, (NSpeechGainManager::EDialogueLayer)0, false, true); break;
                    case 3: talk->PlayAISpeech("TEXT_QST_039_JACK_BOSS_FINAL_TAUNT_40", true, (NSpeechGainManager::EDialogueLayer)0, false, true); break;
                    case 4: talk->PlayAISpeech("TEXT_QST_039_JACK_BOSS_FINAL_TAUNT_50", true, (NSpeechGainManager::EDialogueLayer)0, false, true); break;
                    }
                    ++SpeechCounter;
                }
                {
                    CCharString animation("SPECIAL_BOAST", -1);
                    AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), *PTargetThing.Get(), animation, true), true);
                }
                EnemyKnockDownFrame = GetWorld().GetFrame();
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
            }
        }
        ++counter;
    } while (!boss->IsReadyForPhaseChange() && IsStillRunnable());
    if (boss->IsReadyForPhaseChange()) {
        boss->KillHelpers();
        boss->SetNextBossPhase(4, false);
        GetCreature().SetAsDamageable(false);
        GetCreature().SetAsRespondToHits(false);
        Terminate();
    }
}
