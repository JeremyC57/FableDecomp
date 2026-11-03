// NScript::CQ_WhiteBalverineKnotholeGladeScript::CKG_Chief::Main() -- Chief reports the current attack state and responds to hostile hits.
// Leaf: ?Main@CKG_Chief@CQ_WhiteBalverineKnotholeGladeScript@NScript@@EAEXXZ
// Grade: DIFFER(2557v2515) -- mixed ScriptThing copy and Movie construction expansion remain unresolved.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CISTHINGALIVE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_EXTERNAL_DTOR
#include "engine/CQ_WhiteBalverineKnotholeGladeScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/C3DVector.h"
#include "fable_string.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& t):Thing(t) {}
inline NScript::CIsThingAlive::~CIsThingAlive() {}

void NScript::CQ_WhiteBalverineKnotholeGladeScript::CKG_Chief::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game.EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    Game.EntitySetAsToAddToStatChangesWhenHit(Me,false);
    Game.SetIsPushableByHero(Me,false);
    Game.EntitySetAsKillable(Me,false,true);
    Game.SetThingHasInformation(Me,false,false,false);
    while(!IsActiveThreadTerminating()) {
        if(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if(IsActiveThreadTerminating()) return;
            if(seh_me.IsValid()) seh_me.Release();
            while(!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if(IsActiveThreadTerminating()) return;
            }
            if(IsActiveThreadTerminating()) return;
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if(ParentClass.BalverineState==MBS_FIRST_BALVERINE_ATTACK) {
                    if(IsActiveThreadTerminating()) return;
                    if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_074_CHIEF_BALVERINE_IN_FIRST_ATTACK",GROUP_SELECT_NONE,false,true,false);
                        while(seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if(IsActiveThreadTerminating()) return;
                        }
                        if(IsActiveThreadTerminating()) return;
                    }
                }
                else if(ParentClass.BalverineState==MBS_AFTER_FIRST_BALVERINE_ATTACK) {
                    if(IsActiveThreadTerminating()) return;
                    if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_074_CHIEF_BALVERINE_AFTER_FIRST_ATTACK",GROUP_SELECT_NONE,false,true,false);
                        while(seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if(IsActiveThreadTerminating()) return;
                        }
                        if(IsActiveThreadTerminating()) return;
                    }
                }
                else if(ParentClass.BalverineState==MBS_SECOND_BALVERINE_ATTACK) {
                    if(IsActiveThreadTerminating()) return;
                    if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_074_CHIEF_BALVERINE_IN_SECOND_ATTACK",GROUP_SELECT_NONE,false,true,false);
                        while(seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if(IsActiveThreadTerminating()) return;
                        }
                        if(IsActiveThreadTerminating()) return;
                    }
                }
                else if(ParentClass.BalverineState==MBS_THIRD_BALVERINE_ATTACK) {
                    if(IsActiveThreadTerminating()) return;
                    if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_074_CHIEF_BALVERINE_IN_THIRD_ATTACK",GROUP_SELECT_NONE,false,true,false);
                        while(seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if(IsActiveThreadTerminating()) return;
                        }
                        if(IsActiveThreadTerminating()) return;
                    }
                }
                else if(ParentClass.BalverineState==MBS_AFTER_THIRD_BALVERINE_ATTACK && !Game.IsQuestActive("Q_WhiteBalverineWW")) {
                    if(IsActiveThreadTerminating()) return;
                    if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_074_CHIEF_BALVERINE_AFTER_THIRD_ATTACK",GROUP_SELECT_NONE,false,true,false);
                        while(seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if(IsActiveThreadTerminating()) return;
                        }
                        if(IsActiveThreadTerminating()) return;
                    }
                }
            }
            if(seh_me.IsValid()) seh_me.Release();
        }
        if((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) && ParentClass.BalverineState!=MBS_AFTER_THIRD_BALVERINE_ATTACK) {
            if(IsActiveThreadTerminating()) return;
            Game.NewScriptFrame();
            if(IsActiveThreadTerminating()) return;
            if(seh_me.IsValid()) seh_me.Release();
            while(!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if(IsActiveThreadTerminating()) return;
            }
            if(IsActiveThreadTerminating()) return;
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_074_CHIEF_BEEN_ATTACKED",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
            }
            Game.NewScriptFrame();
            if(IsActiveThreadTerminating()) return;
            Me.SetFriendsWithEverythingFlag(true);
            if(seh_me.IsValid()) seh_me.Release();
        }
        Game.NewScriptFrame();
    }
}
