// NScript::CMarkTestScriptsScript::CTestMan::Main() -- Displays elapsed time, speaks by hero age and toggles age on hits.
// Leaf: ?Main@CTestMan@CMarkTestScriptsScript@NScript@@EAEXXZ
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CISTHINGCONSCIOUS
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_DECLARED_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_EXTERNAL_DTOR
#include "engine/CMarkTestScriptsScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingConscious.h"
#include "engine/C3DVector.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
#include "engine/CCounter.h"
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
#include "engine/fable_script_text.h"
extern CGameScriptInterfaceBase* pDSTGame;
inline CCounter::CCounter():RelevantWorldFrame(pDSTGame->GetWorldFrame()) {}
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& t):Thing(t) {}

void NScript::CMarkTestScriptsScript::CTestMan::Main()
{
    SetPredicate(CIsThingConscious(Me));
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating()) return;
    Game.SetThingHasInformation(Me,false,true,false);
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if(seh_me.IsValid()) seh_me.Release();
    while(!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return;
    }
    if(IsActiveThreadTerminating()) return;
    CCounter Test;
    Test=0.0f;
    while(!IsActiveThreadTerminating()) {
      {
        CCharString Time=GFIntToCharString((long)Test.GetCounter());
        Game.ShowOnScreenMessage(C2DVector(240.0f,80.0f),Time,CRGBColour(255,255,255,255),"");
        if(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if(IsActiveThreadTerminating()) return;
            Test=0.0f;
            Game.PlaySoundOnThing(Me,"SND_TROLLRISING_01");
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if(Game.GetHeroAge()>40.0f) {
                if(IsActiveThreadTerminating()) return;
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"My, you're an old fart!",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
            } else {
                if(IsActiveThreadTerminating()) return;
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"Line 1!",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"Line 2!",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"Line 3!",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"Line 4!",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"Line 5!",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"Line 6!",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"Line 7!",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
            }
        }
        if(Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if(IsActiveThreadTerminating()) return;
            Game.PlaySoundOnThing(Me,"SND_WOMANSCREAM_01");
            Game.EntitySetAppearanceSeed(Me,12345);
            if(Game.GetHeroAge()>40.0f) {
                if(IsActiveThreadTerminating()) return;
                Game.SetHeroAge(20.0f);
            } else {
                if(IsActiveThreadTerminating()) return;
                Game.SetHeroAge(55.0f);
            }
        }
      }
        Game.NewScriptFrame();
    }
}
