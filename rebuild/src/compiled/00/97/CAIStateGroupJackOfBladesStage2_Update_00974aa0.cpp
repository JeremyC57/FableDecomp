// CAIStateGroup_JackOfBladesStage2::Update() -- the second Jack of Blades battle phase: face the arena, float up and grow to battle scale, wait a second, summon the pillars, then alternate fireball shots, flashes and idles (summoning minions when none live and taunting a knocked-down hero) until the boss is ready for phase 4
// Grade: RELOCATION_MATCH (wfD4 successor 9). FITTED spellings (chosen by grading, all ordinary C++, behaviour unchanged): the arena reset's named physics / arenaPos / resetPos (successor 5); the LookAt block's named creature / target references; the wait loop's `stopped` flag with the Terminate after the loop; `const C3DVector& diff` bound to the facing difference; the main loop's named physics and `C3DVector position`; `scale = GetScale(); scale += step;` in the scale-up loop. EVIDENCED: call order / frame homes at the cited retail addresses (NOTES.md).
// Inline-budget: /d2inlS28 (device b, smallest N; S<=27 is the default) -- the only effect is the inline form of the entry HasTC tests of the boss / combat GetTC expansions (retail 0x00974ADD / 0x00974B3B store the bit in a stack byte, `shr ; and cl,1 ; mov [esp+23h],cl`); without it they test the word in place and the frame shrinks to 0x808.
// Flags: /d2inlS28
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
// no MATERIALISED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE
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
#define FABLE_RETAIL_JACK_STAGE2_UPDATE
#define FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
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
#include "engine/CAIStateGroup_JackOfBladesStage2.h"
#include "engine/CCreatureAction_JackFloatUp.h"
#include "engine/CCreatureAction_JackShotFloat.h"
#include "engine/CCreatureAction_JackFlash.h"
#include "engine/CCreatureAction_PlayCombatAnimation.h"
#include "engine/NCreatureMode.h"
#include "engine/fable_ai_action_enums.h"

extern "C" double __cdecl atan2(double y, double x);
extern "C" double __cdecl fmod(double x, double y);
#pragma intrinsic(atan2, fmod)

inline CCharString FABLE_FASTCALL CJackOfBladesBattleDef::StatGetTypeName() { return CCharString("CJackOfBladesBattleDef", -1); }   // FableWin ?StatGetTypeName@CJackOfBladesBattleDef@@SI?AVCCharString@@XZ, expanded in GetSubDef<CJackOfBladesBattleDef> 0x008C33E7

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
    {
        CTCPhysicsBase* physics = GetCreature().GetPhysicsTC();
        const C3DVector& arenaPos = PArenaCentre->GetPos();
        C3DVector resetPos(arenaPos.X, arenaPos.Y, arenaPos.Z);
        physics->ResetPosition(resetPos);
    }
    if (PCutsceneLookAtMarker.Get() && !PCutsceneLookAtMarker->IsDead()) {
        const C3DVector& diff = PCutsceneLookAtMarker->GetPos() - GetCreature().GetPos();
        if (diff.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
            GetCreature().GetPhysicsTC()->SetFacingAngleXY(GET_ROUNDED_ANGLE((float)atan2(diff.X, diff.Y) * 0.15915494f));
    }
    else {
        const C3DVector& diff = PTargetThing->GetPos() - GetCreature().GetPos();
        if (diff.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
            GetCreature().GetPhysicsTC()->SetFacingAngleXY(GET_ROUNDED_ANGLE((float)atan2(diff.X, diff.Y) * 0.15915494f));
    }
    GetCreature().GetPhysicsTC()->EnableGravity(false);
    if (AIAction_PerformActionAndWait(CCreatureAction_JackFloatUp(GetCreature(), battleDef->BattleFloatHeight), false) != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    float scale = GetCreature().GetTCGraphicAppearance()->GetScale();
    float step = (battleDef->BattleScale - scale) / (CMainGameComponent::GetConstantFPS() * battleDef->ScaleUpTime);
    while (scale < battleDef->BattleScale) {
        if (!PTargetThing.Get() || PTargetThing->IsDead() || IsStopping() || !PArenaCentre.Get() || PArenaCentre->IsDead())
            break;
        scale = GetCreature().GetTCGraphicAppearance()->GetScale();
        scale += step;
        GetCreature().GetTCGraphicAppearance()->SetScale(scale);
        AIAction_DoNothing();
    }
    GetCreature().GetTCGraphicAppearance()->SetScale(battleDef->BattleScale);
    boss->SetNextBossPhase(2, false);
    long startFrame = GetWorld().GetFrame();
    long elapsed;
    bool stopped = false;
    do {
        if (!PTargetThing.Get() || PTargetThing->IsDead() || IsStopping() || !PArenaCentre.Get() || PArenaCentre->IsDead()) {
            stopped = true;
            break;
        }
        AIAction_DoNothing();
        elapsed = GetWorld().GetFrame() - startFrame;
    } while ((float)elapsed < (float)CMainGameComponent::GetConstantFPS());
    if (stopped) {
        Terminate();
        return;
    }
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
        CTCPhysicsBase* physics = GetCreature().GetPhysicsTC();
        float height = battleDef->BattleFloatHeight;
        const C3DVector& arena = PArenaCentre->GetPos();
        C3DVector position(arena.X, arena.Y, height + arena.Z);
        physics->SetPosition(position);
        GetCreature().GetPhysicsTC()->EnableGravity(false);
        CThingCreatureBase& creature = GetCreature();
        CThing* target = PTargetThing.Get();
        const C3DVector& from = creature.GetPos();
        const C3DVector& to = target->GetPos();
        float dy = to.Y - from.Y;
        float dx = to.X - from.X;
        if (dx * dx + dy * dy > 1.0f) {
            AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, PTargetThing, 0x11, 0.05f);
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