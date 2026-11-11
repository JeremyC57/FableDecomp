// NScript::CQ_RansomVictimScript::CVictim::Main() -- the ransom victim: under parent scripted control, mark him, spawn the parent's CheckVictimHit watch on him and, until he is rescued, answer the hero's talk with the help line; then (the thanks cut-scene when the ransommer died) follow the hero with a health bar and, frame by frame: note leaving the cave, answer talks (cave thanks / waiting-for-way-cleared bandits or creatures / random nagging), run to the hero when hit by others, ally with the hero on a hero hit, say the on-hit lines near the hero, wait while enemies are near and resume following when they are gone, and stop under parent control for the thanks cut-scene
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Inline-budget: predicate/spawn nested depth0; actual timer constructor/destructor remain calls under guarded auto_inline(off); no extra flags.
// Leaf: ?Main@CVictim@CQ_RansomVictimScript@NScript@@EAEXXZ
// Retail: 0x00DE6C20, 5511 bytes, thiscall.
// Identity (proven, two sources): the Ego_r.pdb / FableWin.pdb name sets hold ?Main@CVictim@CQ_RansomVictimScript@NScript@@EAEXXZ
//   (private virtual); the address has one .rdata reference, slot 1 of the entity vtable 0x012DD1AC that the landed
//   CVictim::Alloc 0x00DE1A50 stores.
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSPAWNEDFUNC1_ORIGINAL_ACCESS
#define FABLE_RETAIL_CQ_RANSOMVICTIMSCRIPT_VICTIM_ORIGINAL_DECLS
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CQ_RANSOMVICTIMSCRIPT_CHECKVICTIMHIT
#include "engine/CQ_RansomVictimScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/CRGBColour.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/CSpawnedFunc1.h"
#include "engine/fable_script_text.h"
#include "engine/fable_script_distance.h"
#include <stdlib.h>
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
#include "engine/EScriptEntityMoveType.h"
#include "engine/EHeroAbility.h"
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
inline CScriptThing::~CScriptThing() {}
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline NScript::CIsThingAlive::~CIsThingAlive() {}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
template <> FABLE_FORCEINLINE CSpawnedFunc1<NScript::CQ_RansomVictimScript,CScriptThing>::CSpawnedFunc1(
    const CCharString& name, unsigned long id, NScript::CQ_RansomVictimScript* pclass,
    void (NScript::CQ_RansomVictimScript::*func)(CScriptThing), CScriptThing par1)
    : CSpawnedFuncBase(name,id),Func(func),PClass(pclass),Par1(par1) {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) {}
CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

void NScript::CQ_RansomVictimScript::CVictim::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Victim.IsValid()) parent.seh_Victim.Release(); }
    while (!Game.StartScriptingEntity(Me, ParentClass.seh_Victim, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.MiniMapAddMarker(Me, "HUD_ORB_GREEN_SMALL");
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQ_RansomVictimScript, CScriptThing>(CCharString("ParentClass.") + CCharString("CheckVictimHit"), 0, &ParentClass, &CQ_RansomVictimScript::CheckVictimHit, Me), CCharString(""));
    while (!ParentClass.RescuedVictim) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(ParentClass.seh_Victim.GetScriptThing()) > 0.0f) {
                ParentClass.seh_Victim.Speak(Game.GetHero(), "TEXT_QST_B06_VICTIM_HELP_10", GROUP_SELECT_NONE, false, true, false);
                while (ParentClass.seh_Victim.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (ParentClass.RansommerDied) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
        ParentClass.PlayCutscene("CS_RANSOM_THANKS", "NULL");
        GotOutOfCave = true;
    }
    Game.EntitySetAsAllowedToFollowHero(Me, true);
    Game.SetEntityAsRegionFollowing(Game.GetHero(), Me, true);
    Game.DisplayQuestInfo(true);
    Game.EntitySetOpinionReactionsEnabled(Me, false);
    Game.AddQuestInfoBarHealth(Me, CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_GRANDSON", 1.0f);
    Game.EntityFollowThing(Me, Game.GetHero(), 1.0f, true);
    Game.SetIsPushableByHero(Me, true);
    { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Victim.IsValid()) parent.seh_Victim.Release(); }
    CTimer BoyResponseTimer;
    long NextLine = 10;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!GotOutOfCave && Game.IsRegionLoaded("Witchwood2")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            GotOutOfCave = true;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game.IsRegionLoaded("WitchwoodCavern") && !GotOutOfCave) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Victim.IsValid()) parent.seh_Victim.Release(); }
                while (!Game.StartScriptingEntity(Me, ParentClass.seh_Victim, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(ParentClass.seh_Victim.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Victim.Speak(Game.GetHero(), "TEXT_QST_B06_VICTIM_THANKS", GROUP_SELECT_NONE, false, true, false);
                    while (ParentClass.seh_Victim.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Victim.IsValid()) parent.seh_Victim.Release(); }
                GotOutOfCave = true;
            } else if (WaitingForThingsToDie) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass.RansommerDied) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(ParentClass.seh_Victim.GetScriptThing()) > 0.0f) {
                        ParentClass.seh_Victim.Speak(Game.GetHero(), "TEXT_QST_B06_VICTIM_WAITING_FOR_WAY_CLEARED_BANDITS_10", GROUP_SELECT_NONE, false, true, false);
                        while (ParentClass.seh_Victim.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(ParentClass.seh_Victim.GetScriptThing()) > 0.0f) {
                        ParentClass.seh_Victim.Speak(Game.GetHero(), "TEXT_QST_B06_VICTIM_WAITING_FOR_WAY_CLEARED_CREATURES_10", GROUP_SELECT_NONE, false, true, false);
                        while (ParentClass.seh_Victim.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_B06_VICTIM_NAGGING_" + GFIntToCharString(rand() % 4), false, Me, Game.GetHero());
            }
        }
        if ((Me.MsgIsHitBy("") || (Me.MsgIsHitByAnySpecialAbilityFrom("") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))
            && !(Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.seh_Victim.MoveToThing(Game.GetHero(), 3.0f, ENTITY_MOVE_RUN, false, true, false, true);
            ToldHeroOfThingsAttackingHim = false;
        }
        if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))
            && BoyResponseTimer.GetTime() == 0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_B06_VICTIM_ON_HERO_HIT", false, Me, Game.GetHero());
            BoyResponseTimer = 5;
            Game.EntitySetThingAsAllyOfThing(Me, Game.GetHero());
            Game.EntitySetThingAsAllyOfThing(Game.GetHero(), Me);
        }
        if (!ToldHeroOfThingsAttackingHim && IsDistanceBetweenThingsUnder(Me, Game.GetHero(), 14.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (BoyResponseTimer.GetTime() == 0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!Game.TextEntryExists("TEXT_QST_B06_VICTIM_ON_HIT_" + GFIntToCharString(NextLine))) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    NextLine = 10;
                }
                long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_B06_VICTIM_ON_HIT_" + GFIntToCharString(NextLine), false, Me, Game.GetHero());
                BoyResponseTimer = 5;
                NextLine += 10;
            }
            if (WaitingForThingsToDie) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntityStopFollowing(Me);
            }
            ToldHeroOfThingsAttackingHim = true;
        }
        if (!WaitingForThingsToDie && IsDistanceBetweenThingsUnder(Me, Game.GetNearestWithScriptName(Me, ParentClass.ActiveEnemyName), 15.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            WaitingForThingsToDie = true;
            Game.EntityStopFollowing(Me);
            Game.EntitySetAsAllowedToFollowHero(Me, false);
            Game.SetEntityAsRegionFollowing(Game.GetHero(), Me, false);
            Game.EntitySetAsScared(Me, true);
            { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Victim.IsValid()) parent.seh_Victim.Release(); }
            while (!Game.StartScriptingEntity(Me, ParentClass.seh_Victim, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (ParentClass.ThanksCutsceneStarting) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntityStopFollowing(Me);
            { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Victim.IsValid()) parent.seh_Victim.Release(); }
            while (!Game.StartScriptingEntity(Me, ParentClass.seh_Victim, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            while (!IsActiveThreadTerminating()) {
                Game.NewScriptFrame();
            }
            return _Fail_Val.Return();
        }
        if (WaitingForThingsToDie && (Game.GetNearestWithScriptName(Me, ParentClass.ActiveEnemyName).IsDead() || IsDistanceBetweenThingsOver(Me, Game.GetNearestWithScriptName(Me, ParentClass.ActiveEnemyName), 15.0f))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            WaitingForThingsToDie = false;
            Game.EntitySetAsAllowedToFollowHero(Me, true);
            Game.SetEntityAsRegionFollowing(Game.GetHero(), Me, true);
            Game.EntitySetOpinionReactionsEnabled(Me, false);
            Game.EntityFollowThing(Me, Game.GetHero(), 1.0f, true);
            Game.EntitySetAsScared(Me, false);
            { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Victim.IsValid()) parent.seh_Victim.Release(); }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}