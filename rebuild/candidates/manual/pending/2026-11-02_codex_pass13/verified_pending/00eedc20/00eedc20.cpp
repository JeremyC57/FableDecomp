// NScript::CPersonalScript_alextestScript::CBurningPerson::Main() -- cycle test health bars, creature visibility and dialogue.
// Leaf: ?Main@CBurningPerson@CPersonalScript_alextestScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CISTHINGALIVE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_DECLARED_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_EXTERNAL_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_EXTERNAL_DTOR
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CPersonalScript_alextestScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
#include "engine/fable_script_animation_defaults.h"
#define FABLE_RETAIL_CTIMER
#define FABLE_RETAIL_CRGBCOLOUR_THREE_CHANNEL_CTOR
#include "engine/CTimer.h"
#include "engine/CRGBColour.h"
#include "stlport/fable_containers.h"
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer(){ TimerIndex=pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer(){ pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time){ pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
template<> std::vector<CScriptThing>::~vector();
void NScript::CPersonalScript_alextestScript::CBurningPerson::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    long TimerIndex=-1;
    long CounterIndex=-1;
    long TickIndex=-1;
    CScriptThing light_marker;
    CTimer TestTimer;
    TestTimer=0;
    bool Toggle=false;
    CScriptThing Barber2=Game.GetThingWithScriptName("Barber2");
    Game.EntitySetAsAllowedToFollowHero(Me,true);
    Game.EntitySetAsAllowedToFollowHero(Barber2,true);
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptGameResourceObjectScriptedThingBase seh_me;
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (SpeakCounter==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            SpeakCounter=1;
            Game.AddQuestInfoBarHealth(Me,CRGBColour(0,0,255),"HUD_ICON_MONEY_BAG",1.0f);
            Game.AddQuestInfoTimer(TestTimer,"HUD_ICON_MONEY_BAG",1.0f);
            Game.AddQuestInfoTimer(TestTimer,"HUD_ICON_MONEY_BAG",1.0f);
            Game.AddQuestInfoBarHealth(Barber2,CRGBColour(255,0,0),"HUD_ICON_MONEY_BAG",1.0f);
            Game.AddQuestInfoBarHealth(Game.GetHero(),CRGBColour(0,255,0),"HUD_ICON_MONEY_BAG",1.0f);
            Game.DisplayQuestInfo(true);
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_CS_B13_MAGICMAN_WAKEUP_DRUNK_10",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            std::vector<CScriptThing> Followers;
            Game.GetAllCreaturesExcludingHero(Followers);
            for (long i=0;i<Followers.size();++i) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetAlpha(Followers[i],0.0f,true);
                Game.EntitySetAsHiddenOnMiniMap(Followers[i],true);
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        } else if (SpeakCounter==1) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            SpeakCounter=2;
            Game.GiveHeroYesNoQuestion("Sure?","Yep","Oh Yes","Who cares?",true);
            long answer;
            while (!((answer=Game.MsgIsQuestionAnsweredYesOrNo())>=0)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            std::vector<CScriptThing> Followers;
            Game.GetAllCreaturesExcludingHero(Followers);
            for (long i=0;i<Followers.size();++i) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetAlpha(Followers[i],1.0f,true);
                Game.EntitySetAsHiddenOnMiniMap(Followers[i],false);
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        } else if (SpeakCounter==2) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            SpeakCounter=0;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"Gottle of geer.",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        while (seh_me.IsPerformingScriptTask()) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.CameraDefault();
        Game.PauseAllNonScriptedEntities(false);
        if (SpeakCounter==1) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        } else if (SpeakCounter==2) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        } else if (SpeakCounter==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
    }
}
