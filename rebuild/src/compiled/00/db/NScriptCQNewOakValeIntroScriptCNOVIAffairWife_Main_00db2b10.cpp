// NScript::CQ_NewOakValeIntroScript::CNOVI_AffairWife::Main() -- the wronged wife: until she learns of the affair she complains when hit, talks to the hero (and, once the hero has discovered the infidelity, asks him whether her husband cheats), and now and then asks the hero where her husband is; told yes, she runs to her husband and argues with him, still reacting to hits and talk, and nags him through conversations.
// Leaf: ?Main@CNOVI_AffairWife@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_ENTITIES2
#include "engine/C3DVector.h"
#include "engine/CQ_NewOakValeIntroScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingConscious.h"
#include "engine/CTimer.h"
#include "engine/fable_script_text.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
extern "C" int __cdecl rand();
extern CGameScriptInterfaceBase* pDSTGame;
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& thing_a, const CScriptThing& thing_b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}
inline CScriptThing::CScriptThing() { }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
void NScript::CQ_NewOakValeIntroScript::CNOVI_AffairWife::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    long conversation=0;
    for (; !false; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (GoingForHusband) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing Husband=Game->GetThingWithScriptName("NOVI_AffairMan");
            Game->EntitySetAsUseMovementInActions(Me,true);
            seh_me.MoveToPosition(Husband.GetPos(),2.0f,ENTITY_MOVE_RUN,false,true);
            Game->ClearThingHasInformation(Me);
            while (!IsDistanceBetweenThingsUnder(Me,Husband,3.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!SaidRunningLine && IsDistanceFromThingToPositionOver(Me,Me.GetHomePos(),10.0f)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long conv=Game->AddNewConversation(Me,false,false);
                    Game->AddLineToConversation(conv,"TEXT_QST_048_AFFAIR_WIFE_RUNNING_TO_HUBBY",false,Me,CScriptThing());
                    SaidRunningLine=true;
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.ClearCommands();
            long WhatsThisIndex=0;
            Game->EntitySetAsUseMovementInActions(Me,false);
            for (; !false; Game->NewScriptFrame()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (IsDistanceBetweenThingsUnder(Me,Game->GetHero(),15.0f)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (rand()%2==0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.PlayAnimation("ST_ARGUING_POINT_AWAY",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.PlayAnimation("ST_ARGUING_POINT_AT",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    }
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->EntitySetFacingAngleTowardsThing(Me,Husband,false);
                        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
                            Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me);
                            ParentClass.AddBadDeed((CQ_NewOakValeIntroScript::EBadDeeds)2);
                            if (seh_me.IsValid()) seh_me.Release();
                            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            CScriptGameResourceObjectMovieBase speech_movie;
                            Game->StartMovieSequence("",speech_movie);
                            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),"TEXT_QST_048_AFFAIR_WIFE_ON_HIT",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            Game->EntitySetFacingAngleTowardsThing(Me,Husband,true);
                        }
                        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (seh_me.IsValid()) seh_me.Release();
                            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            seh_me.ClearAllActions();
                            seh_me.ClearCommands();
                            CScriptGameResourceObjectMovieBase speech_movie;
                            Game->StartMovieSequence("",speech_movie);
                            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),"TEXT_QST_048_AFFAIR_WIFE_THANKYOU_SINGLE",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            Game->EntitySetFacingAngleTowardsThing(Me,Husband,true);
                        }
                        if (!Game->IsConversationActive(conversation)) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            conversation=Game->AddNewConversation(Me,false,false);
                            Game->AddPersonToConversation(conversation,Husband);
                            WhatsThisIndex=WhatsThisIndex+10;
                            CCharString line=CCharString("TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_")+GFIntToCharString(WhatsThisIndex);
                            if (!Game->TextEntryExists(line)) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                WhatsThisIndex=10;
                                line="TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_10";
                            }
                            Game->AddLineToConversation(conversation,line,false,Me,Husband);
                            if (rand()%2==0) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                Game->AddLineToConversation(conversation,"TEXT_QST_048_AFFAIRMAN_IN_TROUBLE",false,Husband,Me);
                            }
                        }
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me);
            ParentClass.AddBadDeed((CQ_NewOakValeIntroScript::EBadDeeds)2);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game->GetHero(),"TEXT_QST_048_AFFAIR_WIFE_ON_HIT",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game->GetHero(),"TEXT_QST_048_AFFAIR_WIFE_LAYABOUT",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (ParentClass.HeroDiscoveredInfidelity) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->GiveHeroYesNoQuestion("TEXT_QST_048_AFFAIR_WIFE_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                while (answer<0) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    answer=Game->MsgIsQuestionAnsweredYesOrNo();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (answer==1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_AFFAIR_WIFE_THANKYOU",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    ParentClass.AddGoodDeed();
                    GoingForHusband=true;
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_AFFAIR_WIFE_PLEA",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
        }
        if (ParentClass.TalkIntermittentTimer.GetTime()==0 && (ForceFirstTimeSpeak || rand()%500==0) && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),5.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.TalkIntermittentTimer=3;
            ForceFirstTimeSpeak=false;
            long conv=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(conv,Game->GetHero());
            Game->AddLineToConversation(conv,"TEXT_QST_048_AFFAIR_WIFE_WHERES_HUSBAND",false,Me,Game->GetHero());
        }
    }
}