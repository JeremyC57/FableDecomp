// NScript::CQ_BanditCampBossBattleScript::CCROWDBANDITS::Main() -- bandit-camp boss battle, a crowd bandit round the arena: untargetable spectator until the king fight starts, then cheers, jeers and boos the hero by distance and the king's health until the fight ends, then joins the fight against the hero when attacked or an angry bandit is needed
// Inline-budget: (1) scoped `#pragma inline_depth(0)` round the row-local CIsThingAlive ctor / dtor (with FABLE_RETAIL_CPREDICATEBASE_NOVTABLE and FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY, the predicate idiom of 0x00E27E90 / 0x00E25F70) -- retail expands the predicate ctor and dtor but CALLS their nested CScriptThing copy 0x004ABE90 (0x00D0B409) and ~CScriptThing 0x004AA840 (0x00D0B42B), while it expands the by-value SetIsPushableByHero copy (0x00D0B461..0x00D0B483); (2) one in-body `#pragma inline_depth(0)` region from the post-loop terminate test of the arena loop to the end of the second loop's cutscene wait -- retail CALLS ~CTimer 0x00CD4470 at those exits (they jump to the shared tail 0x00D0C28B) and expands it only at the arena loop's own exits (0x00D0BD91); our c2 scores the post-loop exit f=0.072 (sc 39) and the second cutscene-wait retry exit f=0.035 (sc 69) against T=74 and expands both, while the arena exits retail expands score 37, so no /d2inlT / /d2inlS value splits them (T30..60 give 3770..3818, S5/S15 3826/3879); (3) FABLE_FORCEINLINE on the row-local CTimer::operator=(long) and GetTime() (the 0x00D0A830 CBanditKing text of this TU has the forced operator=) -- retail expands every use; plain inline calls one GetTime (f=0.0088, sc 238) and operator= sites.
// Ablation (each device removed in turn): no predicate pragma 3834v3802, no NOVTABLE 3818, no IMPLICIT_COPY 3786, no tail pragma 3826, plain operator= 3796, plain GetTime DIFFER(3802v3802); the draft's OUTOFLINE_DTOR opt-in was unneeded (dropped).
// Leaf: ?Main@CCROWDBANDITS@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D0B3E0, 3802 bytes, thiscall, plain `ret`, no EH frame.
// Identity: slot 1 (Main) of the CCROWDBANDITS entity vtable 0x012C95F0 (slot 0 scalar deleting dtor 0x00D0B3C0, 3 GetParentScript 0x00D0B370,
//   5 OnPredicateFail 0x00D0B380; Alloc 0x00D0B2D0 bound "CROWDBANDITS" by the landed script Main 0x00D038E0); Ego_r.pdb
//   ?Main@CCROWDBANDITS@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ. Layout: engine/CQ_BanditCampBossBattleScript.h (new opt-in
//   FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_CROWDBANDITS: Ego_r 28 bytes, ParentClass +0x14, PMasterData +0x18).
// Source shape: Ego_d.pdb line table (lines 2035..2372; 2294..2356 hold no code) and locals (King, village, KingHealthLast (int), seh_me,
//   ActionTimer, _Fail_Val, SetPausedForCutscene; Conversation in the taunt block), in the generated text of the TU's landed CBanditKing::Main
//   0x00D0A830 (TRetVoid returns, the arena loop `for (;!false;Game->NewScriptFrame())` with its terminate test first, so its bottom exit is
//   the pre-loop exit edge, 0x00D0B60E / 0x00D0BD8B). The second loop keeps the pre-loop test and a bottom `frame; test` (retail 0x00D0BE26 /
//   0x00D0C200), the last loop is `while (true) { frame; test }` (0x00D0C278, no leading test). The arena loop's `if (BanditKingFightEnded)
//   break;` sits at the end of the distance-over && timer block: retail skips it on the attack path and when that test fails
//   (jumps to 0x00D0BD7A from 0x00D0B786 / 0x00D0B7A6 / 0x00D0B7BD); the drafts tested it on every non-attack frame.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_CROWDBANDITS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/ECutsceneBehaviour.h"
#include "engine/EHeroAbility.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_animation_defaults.h"
#include <stdlib.h>
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
long __fastcall GFCharStringToInt(const CCharString& text);      // 0x0099E7F0
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
#ifdef _MSC_VER
#pragma inline_depth(0)   // Inline-budget (1)
#endif
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
#ifdef _MSC_VER
#pragma inline_depth()
#endif

void NScript::CQ_BanditCampBossBattleScript::CCROWDBANDITS::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->EntitySetTargetable(Me, false);
    Game->EntitySetAsDamageable(Me, false);
    Game->SetIsPushableByHero(Me, false);
    Game->EntitySetAbleToBeEngagedInCombat(Me, false);
    while (!ParentClass.BanditKingFightStarted) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->EntitySetAsUseMovementInActions(Me, false);
    CScriptThing village = Game->GetNearestWithDefName(Me, "VILLAGE_BANDIT_CAMP_BOSS");
    Game->EntityAttachToVillage(Me, village);
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    seh_me.ClearCommands();
    int KingHealthLast = ParentClass.KingHealth;
    CScriptThing King = Game->GetThingWithScriptName("BanditKing");
    CTimer ActionTimer;
    bool SetPausedForCutscene = false;
    ActionTimer = 0;
    for (;!false;Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.BanditsNeededForCutscene) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetAsUseMovementInActions(Me, true);
            if (seh_me.IsValid()) seh_me.Release();
            while (ParentClass.BanditsNeededForCutscene) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetAsUseMovementInActions(Me, false);
        }
        if (!SetPausedForCutscene && ParentClass.ItsAllOver) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetCutsceneBehaviour(Me, CUTSCENE_BEHAVIOUR_PAUSED);
            SetPausedForCutscene = true;
        }
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 3.5f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetAttackThingImmediately(Me, Game->GetHero(), false, true);
        } else {
            if (IsDistanceBetweenThingsOver(Me, Game->GetHero(), 6.5f) && ActionTimer.GetTime() <= 0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!seh_me.IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (ParentClass.KingHealth < KingHealthLast) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        KingHealthLast = ParentClass.KingHealth;
                        Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), true);
                        if (rand() % 2 == 0) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            switch (rand() % 3) {
                            case 0: seh_me.PlayAnimation("SPECIAL_BOAST", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                            case 1: seh_me.PlayAnimation("ST_THREATEN", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                            case 2: seh_me.PlayAnimation("SCRIPT_SHOUT", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                            }
                            if (rand() % 4 == 0) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                switch (rand() % 2) {
                                case 0: Game->PlaySoundOnThing(Me, "SND_CROWDBOASTBOOS_01"); break;
                                case 1: Game->PlaySoundOnThing(Me, "SND_CROWDBOASTBOOS_02"); break;
                                }
                            }
                        } else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            ActionTimer = 2;
                        }
                    } else if (IsDistanceBetweenThingsOver(Game->GetHero(), King, 8.0f)) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), true);
                        if (rand() % 2 == 0) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            switch (rand() % 2) {
                            case 0: seh_me.PlayAnimation("SPECIAL_BOAST", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                            case 1: seh_me.PlayAnimation("ST_WAVE_SPECIAL_01", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                            }
                            if (rand() % 8 == 0) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                long Conversation = Game->AddNewConversation(Me, false, false);
                                Game->AddPersonToConversation(Conversation, Game->GetHero());
                                Game->AddLineToConversation(Conversation, "TEXT_QST_009_BOSSFIGHT_TAUNTING", false, Me, Game->GetHero());
                            }
                            if (rand() % 4 == 0) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                switch (rand() % 2) {
                                case 0: Game->PlaySoundOnThing(Me, "SND_CROWDBOASTBOOS_01"); break;
                                case 1: Game->PlaySoundOnThing(Me, "SND_CROWDBOASTBOOS_02"); break;
                                }
                            }
                        } else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            ActionTimer = 2;
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), true);
                        if (rand() % 2 == 0) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            switch (rand() % 2) {
                            case 0: seh_me.PlayAnimation("COCKY", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                            case 1: seh_me.PlayAnimation("ST_UNPERTURBED_GESTURE", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                            }
                            switch (rand() % 2) {
                            case 0: Game->PlaySoundOnThing(Me, "SND_CROWDLARGEBOASTREACT_01"); break;
                            case 1: Game->PlaySoundOnThing(Me, "SND_CROWDLARGEBOASTREACT_02"); break;
                            }
                        } else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            ActionTimer = 2;
                        }
                    }
                }
                if (ParentClass.BanditKingFightEnded)
                    break;
            }
        }
    }
#ifdef _MSC_VER
#pragma inline_depth(0)   // Inline-budget (2): ~CTimer called at these exits
#endif
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (GFCharStringToInt(Me.GetDataString()) == 1) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->RemoveThing(Me, false, true);
    }
    Game->EntitySetTargetable(Me, true);
    Game->EntitySetAsDamageable(Me, true);
    Game->EntitySetAsUseMovementInActions(Me, true);
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    for (;;) {
        if (ParentClass.BanditsNeededForCutscene) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetAsUseMovementInActions(Me, true);
            if (seh_me.IsValid()) seh_me.Release();
            while (ParentClass.BanditsNeededForCutscene) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetAsUseMovementInActions(Me, false);
        }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        if (!ParentClass.TwinBladeKilled) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ActionTimer.GetTime() <= 0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (rand() % 2 == 0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    switch (rand() % 3) {
                    case 0: seh_me.PlayAnimation("SPECIAL_BOAST", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                    case 1: seh_me.PlayAnimation("ST_THREATEN", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                    case 2: seh_me.PlayAnimation("SCRIPT_SHOUT", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                    }
                    if (rand() % 4 == 0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        switch (rand() % 2) {
                        case 0: Game->PlaySoundOnThing(Me, "SND_CROWDBOASTBOOS_01"); break;
                        case 1: Game->PlaySoundOnThing(Me, "SND_CROWDBOASTBOOS_02"); break;
                        }
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ActionTimer = 2;
                }
            }
        }
        if (ParentClass.AngryBanditNeeded > 0 || Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))
            break;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    Game->EntitySetAbleToBeEngagedInCombat(Me, true);
    if (seh_me.IsValid()) seh_me.Release();
    if (ParentClass.AngryBanditNeeded != 0) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        --ParentClass.AngryBanditNeeded;
    }
    while (true) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
}