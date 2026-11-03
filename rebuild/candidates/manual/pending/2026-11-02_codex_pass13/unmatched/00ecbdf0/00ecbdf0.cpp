// NScript::CV_SickChildScript::CTalkingTrader1::Main() -- manage trader dialogue and reactions to attacks.
// Leaf: ?Main@CTalkingTrader1@CV_SickChildScript@NScript@@EAEXXZ
// Original Ego_r TPI10a26; native 012f216c slot1.
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_EXTERNAL_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_EXTERNAL_DTOR
#define FABLE_RETAIL_CTIMER
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
#include "engine/fable_script_distance.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}


extern CGameScriptInterfaceBase* pDSTGame;
CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
CTimer::~CTimer(){pDSTGame->DeregisterTimer(TimerIndex);}
inline CTimer& CTimer::operator=(long n){pDSTGame->SetTimer(TimerIndex,n);return *this;}
inline long CTimer::GetTime() const{return pDSTGame->GetTimer(TimerIndex);}
inline bool CTimer::operator==(long n) const{return GetTime()==n;}

void NScript::CV_SickChildScript::CTalkingTrader1::Main()
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
    CScriptThing TalkToThisGuy=Game.GetThingWithScriptName("TalkingTrader2");
    CTimer SaySomethingTimer;
    long ThingsSaid=0;
    long LineNumber=10;
    if(ParentClass.GotFishingSpotMushroom) {
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.RemoveThing(Me,false,true);
    }
    while(!IsActiveThreadTerminating()) {
        if(SaySomethingTimer==0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),20.0f) && IsDistanceBetweenThingsUnder(Me,TalkToThisGuy,10.0f)) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,TalkToThisGuy);
            switch(ThingsSaid) {
                case 0:
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_TRADERA_CHAT1_10",false,Me,TalkToThisGuy);
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_TRADERB_CHAT1_20",false,TalkToThisGuy,Me);
                    break;
                case 1:
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_TRADERA_CHAT2_10",false,Me,TalkToThisGuy);
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_TRADERB_CHAT2_20",false,TalkToThisGuy,Me);
                    break;
                case 2:
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_TRADERA_CHAT3_10",false,Me,TalkToThisGuy);
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_TRADERB_CHAT3_20",false,TalkToThisGuy,Me);
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_TRADERA_CHAT3_30",false,TalkToThisGuy,Me);
                    break;
                case 3:
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_TRADERA_CHAT4_10",false,Me,TalkToThisGuy);
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_TRADERB_CHAT4_20",false,TalkToThisGuy,Me);
                    break;
            }
            if(ThingsSaid<3) {
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                ++ThingsSaid;
            } else {
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                ThingsSaid=0;
            }
            SaySomethingTimer=16;
        }
        if(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            CCharString TalkString;
            switch(TimesTalkedTo) {
                case 0: TalkString="TEXT_QST_B10_TRADERA_INTRO"; break;
                case 1: TalkString="TEXT_QST_B10_TRADERA_REPEATA"; break;
                case 2: TalkString="TEXT_QST_B10_TRADERA_REPEATB_10"; TimesTalkedTo=-1; break;
            }
            ++TimesTalkedTo;
            SaySomethingTimer=SaySomethingTimer.GetTime()+5;
            if(TalkToThisGuy.IsAlive()) {
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),TalkString,GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            } else {
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_TRADERA_OTHERTRADERDEAD_10",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) && Game.GetHealth(Me)>0.0f) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            if(TimesHit>2) {
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetAsKillable(Me,true,true);
                Game.EntitySetAsToAddToComboMultiplierWhenHit(Me,true);
            }
            CCharString FullText;
            FullText="TEXT_QST_B10_TRADERA_ONHIT_"+GFIntToCharString(LineNumber);
            if(!Game.TextEntryExists(FullText)) {
                LineNumber=10;
                FullText="TEXT_QST_B10_TRADERA_ONHIT_"+GFIntToCharString(LineNumber);
            }
            LineNumber+=10;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),FullText,GROUP_SELECT_NONE,false,true,false);
                while(seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            ++TimesHit;
        }
        Game.NewScriptFrame();
    }
}
