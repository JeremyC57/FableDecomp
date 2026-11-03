// NScript::COracleMinigameTestScript::COracleGamesmaster::Main() -- offer and stage the Oracle minigame.
// Leaf: ?Main@COracleGamesmaster@COracleMinigameTestScript@NScript@@EAEXXZ
// Grade: RELOCATION_MATCH -- original timer helper uses documented stronger expansion after ordinary forms fail.
// Inline evidence: original body/types and native expansion proven; original keyword unproven; Owner preference applied to this genuine header helper.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CISTHINGALIVE
#define FABLE_RETAIL_CTIMER
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_DECLARED_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_EXTERNAL_DTOR
#include "engine/COracleMinigameTestScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/CRGBColour.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&,const CScriptThing&,float);
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator==(long time) const { return GetTime()==time; }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}

void NScript::COracleMinigameTestScript::COracleGamesmaster::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.SetThingHasInformation(Me,false,false,false);
    CTimer ToutingTimer;
    ToutingTimer=10;
    for (;!false;Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (!SaidIntro) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B03_GAMEMASTER_INTRO",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                SaidIntro=true;
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B03_GAMEMASTER_RETURN",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            Game.GiveHeroYesNoQuestion("TEXT_QST_B03_MINIGAME_OFFER_GAME","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
            long YNQAnswer;
            while (!((YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo())>=0)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (YNQAnswer==1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.SetPreferredQuickAccessItem("OBJECT_EXPRESSION_Y_DUMMY",0,1000000);
                Game.SetPreferredQuickAccessItem("OBJECT_EXPRESSION_A_DUMMY",1,1000000);
                Game.SetPreferredQuickAccessItem("OBJECT_EXPRESSION_M_DUMMY",2,1000000);
                Game.SetPreferredQuickAccessItem("OBJECT_EXPRESSION_C_DUMMY",3,1000000);
                Game.FadeScreenOut(0.5f,0.5f,CRGBColour(0,0,0,255));
                Game.Pause(0.5f);
                Game.SetTeleportingAsActive(false);
                Game.EntityTeleportToThing(Game.GetHero(),Game.GetThingWithScriptName("HeroOracleMarker"),false);
                Game.EntityTeleportToThing(Me,Game.GetThingWithScriptName("ScytheOracleMarker"),false);
                Game.CameraUseCameraPoint(Game.GetThingWithScriptName("Cam_ForSymbols"),0,0,-1.0f,0,-1);
                Game.FadeScreenIn();
                Game.Pause(0.5f);
                Game.HeroPlayOracleMinigame();
                while (Game.IsHeroPlayingOracleMinigame()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.FadeScreenOut(0.5f,0.5f,CRGBColour(0,0,0,255));
                Game.Pause(0.5f);
                Game.SetPreferredQuickAccessItem("OBJECT_EXPRESSION_Y_DUMMY",0,-1);
                Game.SetPreferredQuickAccessItem("OBJECT_EXPRESSION_A_DUMMY",1,-1);
                Game.SetPreferredQuickAccessItem("OBJECT_EXPRESSION_M_DUMMY",2,-1);
                Game.SetPreferredQuickAccessItem("OBJECT_EXPRESSION_C_DUMMY",3,-1);
                Game.FadeScreenIn();
                Game.Pause(0.5f);
            }
            Game.SetTeleportingAsActive(true);
        }
        if (ToutingTimer==0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),20.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B03_GAMEMASTER_TOUTING",false,Me,Game.GetHero());
            ToutingTimer=25;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
