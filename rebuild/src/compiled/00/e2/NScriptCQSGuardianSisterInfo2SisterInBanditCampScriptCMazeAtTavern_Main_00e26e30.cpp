// NScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript::CMazeAtTavern::Main() -- Maze repeats timed greetings, grants the Bandit Camp quest and teleports away.
// Leaf: ?Main@CMazeAtTavern@CQS_GuardianSisterInfo2_SisterInBanditCampScript@NScript@@EAEXXZ
// Grade: RELOCATION_MATCH(2477v2477) -- original frame loop and scoped success return restore cleanup.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_MAP
#include "engine/CQS_GuardianSisterInfo2_SisterInBanditCampScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/C3DVector.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_fade.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_animation_defaults.h"
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& t):Thing(t) {}
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();

extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline bool CTimer::operator==(long time) const { return pDSTGame->GetTimer(TimerIndex)==time; }

void NScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript::CMazeAtTavern::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if(seh_me.IsValid()) seh_me.Release();
    while(!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
        Game->NewScriptFrame();
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->SetThingHasInformation(Me,false,true,false);
    CTimer WavingFrequency;
    for(;!false;Game->NewScriptFrame()) {
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        if(WavingFrequency==0 && IsDistanceBetweenThingsUnder(Game->GetHero(),Me,30.0f)) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(HLStyleConversationIndex,Game->GetHero());
            Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_027_MAZE_CALL_HERO_OVER_10",false,Me,Game->GetHero());
            seh_me.PlayAnimation("ST_HELLO",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            WavingFrequency=5;
        }
        if(!ParentClass.GuardianSpokeToHero) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
        }
        if(Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me);
            if(seh_me.IsValid()) seh_me.Release();
            while(!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if(Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game->GetHero(),"TEXT_QST_076_MAZE_ON_HIT_10",GROUP_SELECT_NONE,false,true,false);
                while(seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            ++HitCount;
        }
        if(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            if(seh_me.IsValid()) seh_me.Release();
            while(!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            if(TalkCounter==0) {
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->ForceShipsVisible();
                {
                    CScriptGameResourceObjectScriptedThingBase seh_hero;
                    Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                    mymap["MAZE"]=seh_me;
                    mymap["HERO"]=seh_hero;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    RunCutsceneMacro("CS_GUARDIAN_SISTER_2",mymap,0,0,false,true);
                }
                ParentClass.GuardianSpokeToHero=true;
                Game->AddQuestCard("OBJECT_QUEST_CARD_BANDIT_CAMP","Q_BanditCamp",false,false);
            }
            ++TalkCounter;
            if(HitCount<1) {
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                if(seh_me.IsValid()) seh_me.Release();
                while(!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
                    Game->NewScriptFrame();
                    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            Game->ClearThingHasInformation(Me);
        }
        if(ParentClass.GuardianSpokeToHero) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    seh_me.ClearCommands();
    seh_me.PlayAnimation("ST_TELEPORT_OUT",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
    Game->CreateEffect("MAZE_TELEPORT_OUT_01",Me.GetPos(),"",0.0f,false,false);
    FadeOutAndKillEntity(Me,1.0f,true,true);
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(),false,false,false);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(),0);
            return _Fail_Val.Return();
        }
    }
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();

}