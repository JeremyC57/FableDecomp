// NScript::CQ_NewOakValeIntroScript::CNOVI_BarrelThug::Main() -- the warehouse barrel man: explains the barrel job once, follows the hero, answers him by the barrel state, tempts or praises him on the watch timer, and turns on the hero when hit.
// Leaf: ?Main@CNOVI_BarrelThug@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
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
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
extern CGameScriptInterfaceBase* pDSTGame;
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}
void NScript::CQ_NewOakValeIntroScript::CNOVI_BarrelThug::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    for (; !false; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!DoneIntro) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (!ParentClass.BarrelManLeftHeroInCharge) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntityTeleportToThing(Me,Game->GetThingWithScriptName("M_WHouse_ManStart"),false);
            Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
            Game->Pause(3.0f);
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game->GetHero(),"TEXT_QST_048_BARRELTHUG_EXPLAIN",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            DoneIntro=true;
            seh_me.FollowThing(Game->GetHero(),1.0f,true);
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
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
            if (ParentClass.BarrelManSpokenToHeroOnReturn) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass.BarrelBrokenPersistent) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_BARRELTHUG_OUTRO",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_BARRELTHUG_WHY_NOT_SMASH",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass.BarrelBrokenPersistent) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_BARRELTHUG_SCRMSG_WELLDONE",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
        }
        if (!ParentClass.BarrelManSpokenToHeroOnReturn && ParentClass.WatchTimer.GetTime()>0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!ParentClass.BarrelBrokenPersistent) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(conversation,Game->GetHero());
                if (ParentClass.WatchTimer.GetTime()<10 && LastTimeSpoken>10) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->AddLineToConversation(conversation,"TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_10",false,Me,Game->GetHero());
                    LastTimeSpoken=ParentClass.WatchTimer.GetTime();
                } else if (ParentClass.WatchTimer.GetTime()<20 && LastTimeSpoken>20) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->AddLineToConversation(conversation,"TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_20",false,Me,Game->GetHero());
                    LastTimeSpoken=ParentClass.WatchTimer.GetTime();
                } else if (ParentClass.WatchTimer.GetTime()<25 && LastTimeSpoken>25) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->AddLineToConversation(conversation,"TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_30",false,Me,Game->GetHero());
                    LastTimeSpoken=ParentClass.WatchTimer.GetTime();
                } else if (ParentClass.WatchTimer.GetTime()<30 && LastTimeSpoken>30) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->AddLineToConversation(conversation,"TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_40",false,Me,Game->GetHero());
                    LastTimeSpoken=ParentClass.WatchTimer.GetTime();
                } else if (ParentClass.WatchTimer.GetTime()<34 && LastTimeSpoken>34) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->AddLineToConversation(conversation,"TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_50",false,Me,Game->GetHero());
                    LastTimeSpoken=ParentClass.WatchTimer.GetTime();
                } else if (ParentClass.WatchTimer.GetTime()<38 && LastTimeSpoken>38) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->AddLineToConversation(conversation,"TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_60",false,Me,Game->GetHero());
                    LastTimeSpoken=ParentClass.WatchTimer.GetTime();
                } else if (ParentClass.WatchTimer.GetTime()<45 && LastTimeSpoken>45) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->AddLineToConversation(conversation,"TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_70",false,Me,Game->GetHero());
                    LastTimeSpoken=ParentClass.WatchTimer.GetTime();
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(conversation,Game->GetHero());
                if (ParentClass.WatchTimer.GetTime()<10 && LastTimeSpoken>10) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->AddLineToConversation(conversation,"TEXT_QST_048_BARRELTHUG_SCRMSG_WELLDONE_10",false,Me,Game->GetHero());
                    LastTimeSpoken=ParentClass.WatchTimer.GetTime();
                } else if (ParentClass.WatchTimer.GetTime()<25 && LastTimeSpoken>25) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->AddLineToConversation(conversation,"TEXT_QST_048_BARRELTHUG_SCRMSG_WELLDONE_20",false,Me,Game->GetHero());
                    LastTimeSpoken=ParentClass.WatchTimer.GetTime();
                } else if (ParentClass.WatchTimer.GetTime()<35 && LastTimeSpoken>35) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->AddLineToConversation(conversation,"TEXT_QST_048_BARRELTHUG_SCRMSG_WELLDONE_30",false,Me,Game->GetHero());
                    LastTimeSpoken=ParentClass.WatchTimer.GetTime();
                } else if (ParentClass.WatchTimer.GetTime()<45 && LastTimeSpoken>45) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->AddLineToConversation(conversation,"TEXT_QST_048_BARRELTHUG_SCRMSG_WELLDONE_40",false,Me,Game->GetHero());
                    LastTimeSpoken=ParentClass.WatchTimer.GetTime();
                }
            }
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.Release();
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me);
            ParentClass.AddBadDeed((CQ_NewOakValeIntroScript::EBadDeeds)2);
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
                seh_me.Speak(Game->GetHero(),"TEXT_QST_048_BARRELTHUG_WHY_HIT",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}