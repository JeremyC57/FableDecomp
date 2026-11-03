// NScript::CQ_WhiteBalverineWWScript::CKG_Chief::Main() -- Chief waits for the hunt result, speaks, and completes the quest scene.
// Leaf: ?Main@CKG_Chief@CQ_WhiteBalverineWWScript@NScript@@EAEXXZ
// Grade: DIFFER(2358v2539) -- cast assignments stay out of line; resource and cleanup frame residue.
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
#define FABLE_RETAIL_STLPORT_MAP
#include "engine/CQ_WhiteBalverineWWScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingConscious.h"
#include "engine/C3DVector.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& t):Thing(t) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();

void NScript::CQ_WhiteBalverineWWScript::CKG_Chief::Main()
{
    SetPredicate(CIsThingConscious(Me));
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating()) return;
    Game.EntitySetAsKillable(Me,false,true);
    Game.EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    Game.EntitySetAsToAddToStatChangesWhenHit(Me,false);
    Me.SetFriendsWithEverythingFlag(true);
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if(seh_me.IsValid()) seh_me.Release();
    while(!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return;
    }
    if(IsActiveThreadTerminating()) return;
    Game.EntityAttachToScript(Me,Game.GetActiveQuestName());
    while(!ParentClass.MissionSucceeded) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return;
        if(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if(IsActiveThreadTerminating()) return;
            if(!ParentClass.WhiteBalverineAlive) {
                if(IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["HERO"]=seh_hero;
                mymap["CHIEF"]=seh_me;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                Game.FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_WBK_CHIEF4",mymap,0,0,false,true);
                Game.FixMovieSequenceCamera(false);
                ParentClass.MissionSucceeded=true;
            } else {
                if(IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if(!InitialChatted) {
                    if(IsActiveThreadTerminating()) return;
                    InitialChatted=false;
                    if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_074_CHIEF_BALVERINE_GONE_FIRST",GROUP_SELECT_NONE,false,true,false);
                        while(seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if(IsActiveThreadTerminating()) return;
                        }
                        if(IsActiveThreadTerminating()) return;
                    }
                } else {
                    if(IsActiveThreadTerminating()) return;
                    if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_074_CHIEF_BALVERINE_GONE_SECOND",GROUP_SELECT_NONE,false,true,false);
                        while(seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if(IsActiveThreadTerminating()) return;
                        }
                        if(IsActiveThreadTerminating()) return;
                    }
                }
            }
        }
        if(Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
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
        }
    }
    if(IsActiveThreadTerminating()) return;
    Game.ClearThingHasInformation(Me);
}
