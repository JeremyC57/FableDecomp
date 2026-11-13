// NScript::CQ_WhiteBalverineKnotholeGladeScript::CKG_Chief::Main() -- the Knothole Glade chief: live while alive; not combo / stat counted, not pushable, unkillable, no information marker; then loop until terminated: when the hero talks to him say the line for the current BalverineState (2 / 3 / 4 / 6, or 7 while Q_WhiteBalverineWW is not active) under a paused speech movie; when the hero hits him (except a heal-life spell) before stage 7 say TEXT_QST_074_CHIEF_BEEN_ATTACKED and make him friends with everything
// Grade: DIFFER(2501v2515) -- three residues with no plain-C++ lever: (1) mixed MovieBase ctor (retail expands the talk movie ctor, vtable 0x01260EF4, and calls the COMDAT 0x006E7B60 for the hit movie; GOTCHAS chunk 823); (2) the CScriptThing copy split (retail calls 0x004ABE90 for the CIsThingAlive Thing copy but expands the by-value SetIsPushableByHero copy; declared-only copy calls both, an inline copy expands both, at any definition order); (3) retail duplicates the CWideScreenMagicPauseEntities dtor (slot 0x5EC) per return pad and expands the seh_me dtor only at the natural loop exit (calls 0x007E74D0 elsewhere); 97 lines
// Next idea: none found; tried implicit / inline / declared-only copy, copy defined before or after Main, temp predicate, loop-head do-while(0), implicit or out-of-line movie ctor, row-local inline STB dtor (a1-a11, STATUS.tsv)
// Leaf: ?Main@CKG_Chief@CQ_WhiteBalverineKnotholeGladeScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQ_WHITEBALVERINEKNOTHOLEGLADESCRIPT_CKG_CHIEF
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();                        // 0x006E7AB0
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_WhiteBalverineKnotholeGladeScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }

void NScript::CQ_WhiteBalverineKnotholeGladeScript::CKG_Chief::Main()
{
    {
        CIsThingAlive alive(Me);
        SetPredicate(alive);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->EntitySetAsToAddToStatChangesWhenHit(Me, false);
    Game->SetIsPushableByHero(Me, false);
    Game->EntitySetAsKillable(Me, false, true);
    Game->SetThingHasInformation(Me, false, false, false);
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (ParentClass.BalverineState == 2) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_074_CHIEF_BALVERINE_IN_FIRST_ATTACK", (ETextGroupSelectionMethod)0, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                }
                else if (ParentClass.BalverineState == 3) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_074_CHIEF_BALVERINE_AFTER_FIRST_ATTACK", (ETextGroupSelectionMethod)0, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                }
                else if (ParentClass.BalverineState == 4) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_074_CHIEF_BALVERINE_IN_SECOND_ATTACK", (ETextGroupSelectionMethod)0, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                }
                else if (ParentClass.BalverineState == 6) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_074_CHIEF_BALVERINE_IN_THIRD_ATTACK", (ETextGroupSelectionMethod)0, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                }
                else if (ParentClass.BalverineState == 7 && !Game->IsQuestActive("Q_WhiteBalverineWW")) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_074_CHIEF_BALVERINE_AFTER_THIRD_ATTACK", (ETextGroupSelectionMethod)0, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                }
            }
            if (seh_me.IsValid())
                seh_me.Release();
        }
        if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
             (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) &&
            ParentClass.BalverineState != 7) {
            if (IsActiveThreadTerminating())
                return;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_074_CHIEF_BEEN_ATTACKED", (ETextGroupSelectionMethod)0, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
            }
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            Me.SetFriendsWithEverythingFlag(true);
            if (seh_me.IsValid())
                seh_me.Release();
        }
        Game->NewScriptFrame();
    }
}
