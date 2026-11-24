// NScript::CQ_NewOakValeIntroScript::CNOVI_BookTrader::Main() -- New Oakvale book trader: walk back home and face Theresa, react to being hit by the hero (ally, bad deed, complaint), on being talked to give the intro and offer chocolates for 3 gold (quest objective on purchase), and now and then shout a roll-up line at a nearby hero
// Grade: RELOCATION_MATCH (B24B3, 2026-11-24)
// Inline-budget: `#pragma inline_depth(0)` round the FABLE_FORCEINLINE ~CIsThingConscious definition -- retail expands the predicate
//   temporary's destructor in Main (vtable store 0x012C2FE8 at 0x00DB3FD8) but CALLS the nested member ~CScriptThing 0x004AA840
//   (0x00DB3FDC), while ~CScriptThing is expanded at the other sites; c2 inherits the nested call from the body compiled under depth 0.
// Inline-budget: FABLE_FORCEINLINE CTimer::GetTime / operator= (the landed TIMER-family spelling, 89 / 86 rows; retail expands both)
//   and ~CIsThingConscious (retail expands it at the predicate site).
// Pick-any: CCountedPointer<CScriptGameResourceObjectMovieBase>'s dtor is the visible header template (HEAD row 0x00D44CB0 spelling), so the
//   emitted ??1CScriptGameResourceObjectMovieBase copy equals retail 0x006E7B80 (the declared-only specialization made it call 0x006E7AB0).
// Leaf: ?Main@CNOVI_BookTrader@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_ENTITIES3
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
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/EHeroAbility.h"
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
extern CGameScriptInterfaceBase* pDSTGame;
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline CScriptThing::~CScriptThing() { }
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
#pragma inline_depth(0)
FABLE_FORCEINLINE NScript::CIsThingConscious::~CIsThingConscious() {}
#pragma inline_depth()

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
