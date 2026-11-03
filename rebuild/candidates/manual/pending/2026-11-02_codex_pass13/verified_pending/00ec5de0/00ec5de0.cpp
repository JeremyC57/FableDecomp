// NScript::CV_SickChildScript::CSickChild::Main() -- manage the sick child in bed, recovery and dialogue.
// Leaf: ?Main@CSickChild@CV_SickChildScript@NScript@@EAEXXZ
// Original Ego_r TPI10a85; native012f18dc slot1 and original named bed binding.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_EXTERNAL_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_EXTERNAL_DTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING_EXTERNAL_CONVERSION
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFPOINTER
#define FABLE_RETAIL_CTIMER
#define FABLE_RETAIL_VECTOR_ANGLE
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
#include "engine/CV_SickChildScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_text.h"
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}

#include "engine/CTimer.h"
#include "engine/C3DVector.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_animation_defaults.h"
#include "engine/fable_script_distance.h"
#include <math.h>
#pragma intrinsic(atan2,fmod)
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() { TimerIndex=pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator==(long time) const { return GetTime()==time; }
inline float __fastcall GFArcTan(double opposite,double adjacent)
{ return (float)atan2(opposite,adjacent)/(2.0f*3.14159265358979323846f); }
// Fidelity: proven original header math; stronger expansion follows the owner header-helper rule, not a recovered keyword.
__forceinline float __fastcall GET_ROUNDED_ANGLE(float angle)
{
    if(angle<0.0f || angle>=1.0f) {
        angle=(float)fmod(angle,1.0f);
        if(angle<0.0f) angle+=1.0f;
    }
    return angle;
}
__forceinline float C3DVector::GetAngleXY() const { return GET_ROUNDED_ANGLE(GFArcTan(X,Y)); }

void NScript::CV_SickChildScript::CSickChild::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if(seh_me.IsValid()) seh_me.Release();
    while(!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    CTimer GarbledSpeak;
    C3DVector Position,Direction;
    Game.GetSleepingPositionAndOrientationFromBed(Me,Game.GetThingWithScriptName("SickChildBed"),Position,Direction);
    Game.EntityTeleportToPosition(Me,Position,Direction.GetAngleXY(),false,false);
    seh_me.PlayLoopingAnimation(pGlobalScriptDef->SickChildSleepingAnim,-1,false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
    Game.SetBedAvailability(Game.GetThingWithScriptName("SickChildBed"),false);
    Game.SetThingAsUsable(Game.GetThingWithScriptName("SickChildBed"),false);
    Game.EntitySetTargetable(Me,false);
    Game.EntitySetAsRespondToHit(Me,false);
    Game.EntitySetAsKillable(Me,false,false);
    Game.EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    long LineNumber=10;
    while(!ParentClass.FinishedQuest) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        if((GarbledSpeak==0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),14.0f)) || Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            CCharString Text;
            CCharString FullText;
            FullText="TEXT_QST_B10_BOY_JABBERS_"+GFIntToCharString(LineNumber);
            if(!Game.TextEntryExists(FullText)) {
                LineNumber=10;
                FullText="TEXT_QST_B10_BOY_JABBERS_"+GFIntToCharString(LineNumber);
            }
            LineNumber+=10;
            Game.AddLineToConversation(HLStyleConversationIndex,FullText,false,Me,Game.GetHero());
            GarbledSpeak=20;
        }
    }
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.SetBedAvailability(Game.GetThingWithScriptName("SickChildBed"),true);
    Game.EntityTeleportToThing(Me,Game.GetThingWithScriptName("MK_SC_BOY"),false);
    seh_me.ClearAllActionsIncludingLoopingAnimations();
    Game.EntitySetAsRespondToHit(Me,true);
    Game.EntitySetTargetable(Me,true);
    for(;true;Game.NewScriptFrame()) {
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        if(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_BOY_CURED_10",GROUP_SELECT_NONE,false,true,false);
                while(seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        if(Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MOTHER_HIT",GROUP_SELECT_NONE,false,true,false);
                while(seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            Game.EntitySetThingAsAllyOfThing(Me,Game.GetHero());
            Game.EntitySetThingAsAllyOfThing(Game.GetHero(),Me);
        }
    }
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
}
