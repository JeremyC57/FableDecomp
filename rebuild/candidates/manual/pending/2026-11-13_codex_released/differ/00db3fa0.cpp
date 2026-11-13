// NScript::CQ_NewOakValeIntroScript::CNOVI_BookTrader::Main() -- New Oakvale book trader: walk back home and face Theresa, react to being hit by the hero (ally, bad deed, complaint), on being talked to give the intro and offer chocolates for 3 gold (quest objective on purchase), and now and then shout a roll-up line at a nearby hero
// Grade: DIFFER(4106v4042) -- md=53: the predicate temporary's ~CIsThingConscious expands with its nested ~CScriptThing (dec/test/delete) where retail expands only the outer vtable store and calls 0x004AA840, and the 4 frame slots that shift with it; every other site (walk-home temp expanded, GetHealth temps called, movie/pause, yes/no question) matches
// Next idea: GOTCHAS chunk 799: the one-level expansion needs inline_depth(0) (forbidden); a lane-level answer for one-level ~CIsThing expansion would land this row and its CIsThingAlive siblings
// Leaf: ?Main@CNOVI_BookTrader@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_BOOKTRADER
#include "engine/C3DVector.h"
#include "engine/CQ_NewOakValeIntroScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingConscious.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_animation_defaults.h"
#include <stdlib.h>
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
extern CGameScriptInterfaceBase* pDSTGame;
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline CScriptThing::~CScriptThing() { }
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
__forceinline NScript::CIsThingConscious::~CIsThingConscious() {}

#define BT_T if (IsActiveThreadTerminating()) return _Fail_Val.Return()
#define BT_START(priority) \
    if (seh_me.IsValid()) seh_me.Release(); \
    while (!Game->StartScriptingEntity(Me,seh_me,priority)) { \
        Game->NewScriptFrame(); \
        BT_T; \
    } \
    BT_T
#define BT_SPEAK(text) \
    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) { \
        seh_me.Speak(Game->GetHero(),text,GROUP_SELECT_NONE,false,true,false); \
        while (seh_me.IsPerformingScriptTask()) { \
            Game->NewScriptFrame(); \
            BT_T; \
        } \
        BT_T; \
    }

void NScript::CQ_NewOakValeIntroScript::CNOVI_BookTrader::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    BT_T;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    for (; !false; Game->NewScriptFrame()) {
        BT_T;
        BT_START(SCRIPT_AI_PRIORITY_HIGH);
        if (IsDistanceFromThingToPositionOver(Me,Me.GetHomePos(),0.1f)) {
            BT_T;
            C3DVector home=Me.GetHomePos();
            while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),home,2.0f)) {
                Game->NewScriptFrame();
                BT_T;
                seh_me.MoveToPosition(home,0.0f,(EScriptEntityMoveType)0,false,true);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    BT_T;
                }
                BT_T;
            }
            BT_T;
            Game->EntitySetFacingAngleTowardsThing(Me,Game->GetThingWithScriptName("NOVI_Theresa"),false);
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            BT_T;
            Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me);
            ParentClass.AddBadDeed((CQ_NewOakValeIntroScript::EBadDeeds)2);
            BT_START(SCRIPT_AI_PRIORITY_HIGHEST);
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            BT_SPEAK("TEXT_QST_048_TRADER_ON_HIT");
            Game->EntitySetFacingAngleTowardsThing(Me,Game->GetThingWithScriptName("NOVI_Theresa"),true);
        } else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            BT_T;
            BT_START(SCRIPT_AI_PRIORITY_HIGHEST);
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (ParentClass.GivenSweets) {
                BT_T;
                BT_SPEAK("TEXT_QST_048_TRADER_INTRO_10");
            } else {
                BT_T;
                if (!DoneIntro) {
                    BT_T;
                    BT_SPEAK("TEXT_QST_048_TRADER_INTRO");
                    DoneIntro=true;
                } else {
                    BT_T;
                    BT_SPEAK("TEXT_QST_048_TRADER_STILL_GOT");
                }
                Game->GiveHeroYesNoQuestion("TEXT_QST_048_TRADER_BUY_SWEETS","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                while (answer<0) {
                    Game->NewScriptFrame();
                    BT_T;
                    answer=Game->MsgIsQuestionAnsweredYesOrNo();
                }
                BT_T;
                if (answer==1) {
                    BT_T;
                    if (Game->GetHeroGold()>=3) {
                        BT_T;
                        BT_SPEAK("TEXT_QST_048_TRADER_GIVES_SWEETS");
                        Game->GiveHeroObject("OBJECT_CHOCOLATE_BOX_UNGIVEABLE",-1,false);
                        Game->GiveHeroGold(-3);
                        Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_04","","");
                        ParentClass.GivenSweets=true;
                        Game->ClearThingHasInformation(Me);
                    } else {
                        BT_T;
                        BT_SPEAK("TEXT_QST_048_TRADER_NOT_ENOUGH_CASH");
                    }
                } else {
                    BT_T;
                    BT_SPEAK("TEXT_QST_048_TRADER_BUY_LATER");
                }
            }
            Game->EntitySetFacingAngleTowardsThing(Me,Game->GetThingWithScriptName("NOVI_Theresa"),false);
        }
        if (ParentClass.TalkIntermittentTimer.GetTime()==0 && rand()%200==0 && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),20.0f)) {
            BT_T;
            ParentClass.TalkIntermittentTimer=3;
            long conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(conversation,Game->GetHero());
            BT_START(SCRIPT_AI_PRIORITY_HIGHEST);
            seh_me.PlayAnimation("ST_OPINION_NEUTRAL_SHOUTING_WITH_HANDS_CUPPED",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            Game->AddLineToConversation(conversation,"TEXT_QST_048_TRADER_ROLL_UP",false,Me,Game->GetHero());
        }
    }
    BT_T;
}
