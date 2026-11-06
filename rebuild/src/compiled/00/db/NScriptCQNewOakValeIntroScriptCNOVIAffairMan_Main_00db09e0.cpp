// NScript::CQ_NewOakValeIntroScript::CNOVI_AffairMan::Main() -- the unfaithful husband: hit by the hero he complains; talked to near his wife he answers by the deal he made, otherwise asks the hero to keep quiet (gold and a bad deed) or shoos him; idle, he cowers by his wife, walks home, or whispers dirty words to his lover and kisses or hugs her.
// Leaf: ?Main@CNOVI_AffairMan@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_ENTITIES
#include "engine/C3DVector.h"
#include "engine/CQ_NewOakValeIntroScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingConscious.h"
#include "engine/fable_script_text.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
extern "C" int __cdecl rand();
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& thing_a, const CScriptThing& thing_b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}
void NScript::CQ_NewOakValeIntroScript::CNOVI_AffairMan::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing Woman=Game->GetThingWithScriptName("NOVI_AffairWoman");
    CScriptThing Wife=Game->GetThingWithScriptName("NOVI_AffairWife");
    long conversation=0;
    for (; !false; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
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
                seh_me.Speak(Game->GetHero(),"TEXT_QST_048_AFFAIRMAN_ON_HIT",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            ParentClass.AddBadDeed((CQ_NewOakValeIntroScript::EBadDeeds)2);
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
            seh_me.ClearAllActions();
            seh_me.ClearCommands();
            if (IsDistanceBetweenThingsUnder(Me,Wife,5.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (HeroAgreedToKeepQuiet) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_AFFAIRMAN_HAD_A_DEAL",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                else if (HeroSaidHeWouldReportMan) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_AFFAIRMAN_SOME_NERVE",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_AFFAIRMAN_HOW_FIND_OUT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                Game->ClearThingHasInformation(Me);
            }
            else if (!EncounterOver) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetFacingAngleTowardsThing(Woman,Game->GetHero(),false);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_048_AFFAIRMAN_INTRO",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                ParentClass.HeroDiscoveredInfidelity=true;
                Game->GiveHeroYesNoQuestion("TEXT_QST_048_AFFAIRMAN_QUESTION_WILL_YOU_TELL","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
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
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_AFFAIRMAN_GOOD_LAD",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    ParentClass.AddBadDeed((CQ_NewOakValeIntroScript::EBadDeeds)4);
                    Game->GiveHeroGold(1);
                    HeroAgreedToKeepQuiet=true;
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_AFFAIRMAN_DONT_TELL",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    HeroSaidHeWouldReportMan=true;
                }
                Game->ClearThingHasInformation(Me);
                EncounterOver=true;
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_048_AFFAIRMAN_SHOO",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            if (Woman.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetFacingAngleTowardsThing(Woman,Me,false);
            }
            else if (Wife.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetFacingAngleTowardsThing(Wife,Me,false);
            }
        }
        else if (!seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (IsDistanceBetweenThingsUnder(Me,Wife,5.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetFacingAngleTowardsThing(Me,Wife,false);
                if (rand()%50==0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.PlayAnimation("ST_OPINION_FEAR_IDLE_COWERING",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                }
            }
            else if (IsDistanceFromThingToPositionOver(Me,Me.GetHomePos(),0.1f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                C3DVector HomePos=Me.GetHomePos();
                while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),HomePos,2.0f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(HomePos,0.0f,ENTITY_MOVE_WALK,false,true);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            else if (!Game->IsConversationActive(conversation) && (!SaidFirstRangedComment || rand()%100==0) && IsDistanceBetweenThingsUnder(Me,Woman,2.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (IsDistanceBetweenThingsUnder(Me,Game->GetHero(),5.0f)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    conversation=Game->AddNewConversation(Me,false,false);
                    Game->AddPersonToConversation(conversation,Woman);
                    BadgerIndex=BadgerIndex+10;
                    if (BadgerIndex>50) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        BadgerIndex=10;
                    }
                    CCharString ManLine=CCharString("TEXT_QST_048_AFFAIRMAN_DIRTY_WORDS_NEAR_")+GFIntToCharString(BadgerIndex);
                    CCharString WomanLine=CCharString("TEXT_QST_048_AFFAIRWOMAN_DIRTY_WORDS_NEAR_")+GFIntToCharString(BadgerIndex);
                    if (BadgerIndex==10) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->AddLineToConversation(conversation,WomanLine,false,Woman,Me);
                        Game->AddLineToConversation(conversation,ManLine,false,Me,Woman);
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->AddLineToConversation(conversation,ManLine,false,Me,Woman);
                        Game->AddLineToConversation(conversation,WomanLine,false,Woman,Me);
                    }
                    ParentClass.HeroDiscoveredInfidelity=true;
                }
                else if (IsDistanceBetweenThingsUnder(Me,Game->GetHero(),13.0f)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    SaidFirstRangedComment=true;
                    conversation=Game->AddNewConversation(Me,false,false);
                    Game->AddPersonToConversation(conversation,Woman);
                    BadgerIndex=BadgerIndex+10;
                    if (BadgerIndex>50) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        BadgerIndex=10;
                    }
                    CCharString ManLine=CCharString("TEXT_QST_048_AFFAIRMAN_SCRMSG_DIRTY_WORDS_")+GFIntToCharString(BadgerIndex);
                    CCharString WomanLine=CCharString("TEXT_QST_048_AFFAIRWOMAN_DIRTY_WORDS_")+GFIntToCharString(BadgerIndex);
                    Game->AddLineToConversation(conversation,ManLine,false,Me,Woman);
                    Game->AddLineToConversation(conversation,WomanLine,false,Woman,Me);
                }
                if (!ParentClass.TalkingToWoman) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->Pause(0.4f);
                    Game->EntitySetFacingAngleTowardsThing(Me,Woman,true);
                    Game->EntitySetFacingAngleTowardsThing(Woman,Me,true);
                    if (rand()%2==0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        ParentClass.ReceiveKiss=true;
                        seh_me.PlayAnimation("GIVE_KISS",false,true,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        ParentClass.ReceiveHug=true;
                        seh_me.PlayAnimation("GIVE_HUG",false,true,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    }
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}