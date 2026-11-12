// NScript::CQ_BanditCampScript::CGate1GuardOuter::Main() -- the outer guard at the bandit camp's first gate: until the gate is open he calls the hero over every ten seconds and, when talked to, checks the hero's bandit disguise piece by piece (shirt, trousers, hat, boots, gloves), then lets a fully disguised hero in (notes whether the bandits sleep), opens Gate1, sets the quest objective and grants experience; once the gate is open he only gives an aside; hit by the hero (or once the outer guards were attacked) he complains, flags the attack and fights the hero for good.
// Grade: DIFFER(5134v5178) -- Four native expanded Movie cleanup sites remain called, with corresponding cleanup layout and stack slots. 35 strict / 6 loose hunks. Scalar inline budgets trade Movie expansion against unwanted Timer expansion.
// Inline-budget: Forced genuine Movie/Pause ctors and Timer accessors preserve native expansions; implicit Movie cleanup still has a called-versus-expanded residue.
// Leaf: ?Main@CGate1GuardOuter@CQ_BanditCampScript@NScript@@EAEXXZ
// Retail: 0x00D01630, 5178 bytes, thiscall.
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTRESOURCE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CBASEOBJECT_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTTHING_ORIGINAL_ACCESS
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#include "engine/C3DVector.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_BanditCampScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
#include "engine/CTimer.h"
#include "engine/fable_script_def_global.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
#include "engine/EHeroAbility.h"
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() { TimerIndex=pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingAlive::~CIsThingAlive() {}
void NScript::CQ_BanditCampScript::CGate1GuardOuter::Main()
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
    CScriptThing Gate1GuardInner=Game.GetThingWithScriptName("Gate1GuardInner");
    CTimer MoanTimer;
    MoanTimer=0;
    bool TalkedTo=false;
    bool FailedOnce=false;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!ParentClass.Gate1Open) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (MoanTimer.GetTime()<=0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                if (!TalkedTo) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    TalkedTo=true;
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_009_BANDIT1_CALL_OVER_FIRST",false,Me,Game.GetHero());
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_009_BANDIT1_CALL_OVER_SECOND",false,Me,Game.GetHero());
                }
                MoanTimer=10;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (!Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_SHIRT_BANDITCAMP") && !Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_TROUSERS_BANDITCAMP") && !Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_HAT_BANDITCAMP") && !Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_BOOTS_BANDITCAMP") && !Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_GLOVES_BANDITCAMP")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_NO_DISGUISE",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    FailedOnce=true;
                }
                else if (!Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_SHIRT_BANDITCAMP")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_NO_SHIRT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    FailedOnce=true;
                }
                else if (!Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_TROUSERS_BANDITCAMP")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_NO_TROUSERS",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    FailedOnce=true;
                }
                else if (!Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_HAT_BANDITCAMP")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_NO_HAT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    FailedOnce=true;
                }
                else if (!Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_BOOTS_BANDITCAMP")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_NO_BOOTS",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    FailedOnce=true;
                }
                else if (!Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_GLOVES_BANDITCAMP")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_NO_GLOVES",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    FailedOnce=true;
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (FailedOnce==true) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_TRIED_AND_IN",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_STRAIGHT_IN",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    CScriptThing BanditCamp=Game.GetNearestWithDefName(Game.GetHero(),"VILLAGE_BANDIT_CAMP_MAIN");
                    if (Game.IsSleepingTime(BanditCamp)) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_BANDITS_SLEEPING",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_GATE_OPEN",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    Game.OpenDoor(Game.GetThingWithScriptName("Gate1"));
                    Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_05","BanditCampBoss","BanditCampEntrance");
                    ParentClass.Gate1Open=true;
                    Game.GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_MEDIUM);
                    Game.ClearThingHasInformation(Me);
                }
            }
        }
        else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_ASIDE",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game.GetHealth(Me)>0.0f) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_009_BANDIT1_ATTACKED_NEW",false,Me,Game.GetHero());
            }
            ParentClass.AttackedOuterGateGuards=true;
            Game.GiveThingBestEnemyTarget(Me,Game.GetHero());
            Game.ClearThingHasInformation(Me);
            if (seh_me.IsValid()) seh_me.Release();
            for (;;) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        if (ParentClass.AttackedOuterGateGuards) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.GiveThingBestEnemyTarget(Me,Game.GetHero());
            if (seh_me.IsValid()) seh_me.Release();
            for (;;) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
