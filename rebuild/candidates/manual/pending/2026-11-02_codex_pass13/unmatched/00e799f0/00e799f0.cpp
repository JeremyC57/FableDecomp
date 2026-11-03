// NScript::CV_ExposeMayorScript::CEM_Rhodri::Main() -- guide the hero to the haunted house and react to conversations and attacks.
// Leaf: ?Main@CEM_Rhodri@CV_ExposeMayorScript@NScript@@EAEXXZ
// Grade: DIFFER(2580v2580) -- predicate destructor expansion and initial parent resource address differ.
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_RELEASE
#define FABLE_RETAIL_CTIMER
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CISTHINGCONSCIOUS
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_DECLARED_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE
#include "engine/CV_ExposeMayorScript.h"
#include "engine/CIsThingConscious.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
inline NScript::CIsThingConscious::~CIsThingConscious() {}
inline CScriptThing::~CScriptThing() {}
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CTimer.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_text.h"
extern CGameScriptInterfaceBase* pDSTGame;
CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
CTimer::~CTimer(){pDSTGame->DeregisterTimer(TimerIndex);}
inline CTimer& CTimer::operator=(long n){pDSTGame->SetTimer(TimerIndex,n);return *this;}
inline long CTimer::GetTime() const{return pDSTGame->GetTimer(TimerIndex);}
inline bool CTimer::operator==(long n) const{return GetTime()==n;}
void NScript::CV_ExposeMayorScript::CEM_Rhodri::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    {
        CScriptGameResourceObjectScriptedThingBase &seh_Rhodri=ParentClass.seh_Rhodri;
        if (seh_Rhodri.IsValid()) seh_Rhodri.Release();
    }
    while (!Game.StartScriptingEntity(Me,ParentClass.seh_Rhodri,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CTimer TalkTimer;
    CTimer FaceHeroTimer;
    while (ParentClass.StageNumber!=STAGE_PATH_COMPLETE) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (FaceHeroTimer==0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),10.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
            FaceHeroTimer=4;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.StageNumber==STAGE_TWO) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ExplainedPlight=true;
                ParentClass.PlayCutscene("CS_MAYOREXPOSE_RHODRI",true,"NULL");
                ParentClass.StageNumber=STAGE_THREE;
                Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_EXPOSE_MAYOR_OBJECTIVE_02","HauntedHouse","");
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (ParentClass.StageNumber==STAGE_FIVE) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(ParentClass.seh_Rhodri.GetScriptThing())>0.0f) {
                        ParentClass.seh_Rhodri.Speak(Game.GetHero(),"TEXT_QST_B18_RHODRI_LETTER",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                        while (ParentClass.seh_Rhodri.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(ParentClass.seh_Rhodri.GetScriptThing())>0.0f) {
                        ParentClass.seh_Rhodri.Speak(Game.GetHero(),CCharString("TEXT_QST_B18_RHODRI_CHATTER")+(PMasterData->HeroMarriedLadyGrey?"_MARRIED":""),GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                        while (ParentClass.seh_Rhodri.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(ParentClass.seh_Rhodri.GetScriptThing())>0.0f) {
                ParentClass.seh_Rhodri.Speak(Game.GetHero(),CCharString("TEXT_QST_B18_RHODRI_ATTACKED")+(PMasterData->HeroMarriedLadyGrey?"_MARRIED":""),GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                while (ParentClass.seh_Rhodri.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        if (!ExplainedPlight && TalkTimer==0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),15.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            TalkTimer=7;
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B18_RHODRI_ATTENTION",false,Me,Game.GetHero());
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
