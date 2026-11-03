// NScript::CQ_RansomVictimScript::CVictimsBrotherHelper::Main() -- handle the brother's protection, conversation, and revenge.
// Leaf: ?Main@CVictimsBrotherHelper@CQ_RansomVictimScript@NScript@@EAEXXZ
// Grade: RELOCATION_MATCH(2528v2528) -- original generated for-frame and authored pause constructor expansion.
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include "engine/CQ_RansomVictimScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
inline NScript::CIsThingAlive::~CIsThingAlive() {}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/fable_script_text.h"
extern CGameScriptInterfaceBase* pDSTGame;
CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
CTimer::~CTimer(){pDSTGame->DeregisterTimer(TimerIndex);}
inline CTimer& CTimer::operator=(long n){pDSTGame->SetTimer(TimerIndex,n);return *this;}
inline long CTimer::GetTime() const{return pDSTGame->GetTimer(TimerIndex);}
inline bool CTimer::operator==(long n) const{return GetTime()==n;}
void NScript::CQ_RansomVictimScript::CVictimsBrotherHelper::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CTimer TurningTimer;
    Game->EntitySetInFaction(Me,"FACTION_HERO");
    for (; !false; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (TurningTimer==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
            TurningTimer=1;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.RescuedVictim && Game->GetThingWithScriptName("Victim").IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.ThanksCutsceneStarting=true;
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                ParentClass.PlayCutscene("CS_RANSOM_OUTRO_EVIL","NULL");
                PMasterData->RansomVictimHaveVictimKilled=true;
                ParentClass.MissionSucceeded=true;
                Game->SetCategoryActivity("ChiefsSonDied",true);
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B06_"+Me.GetDataString()+"_CHAT",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (!ParentClass.SomeoneDealingWithBeingHit && (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.SomeoneDealingWithBeingHit=true;
            ++ParentClass.HitEvilBrotherAndProtection;
            if (ParentClass.HitEvilBrotherAndProtection==1 && Game->GetHealth(Me)>0.0f) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                {
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(ParentClass.seh_Brother.GetScriptThing())>0.0f) {
                        ParentClass.seh_Brother.Speak(Game->GetHero(),"TEXT_QST_B06_BROTHER_ON_HIT_HELPERS",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                        while (ParentClass.seh_Brother.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                ParentClass.SomeoneDealingWithBeingHit=false;
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing Brother=Game->GetThingWithScriptName("VictimsBrother");
                long HLStyleConversationIndex=Game->AddNewConversation(Brother,false,false);
                Game->AddPersonToConversation(HLStyleConversationIndex,Game->GetHero());
                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B06_BROTHER_ON_RUN_AWAY_10",false,Brother,Game->GetHero());
                Game->EntitySetInFaction(Me,"FACTION_BANDITS");
                Game->GiveThingBestEnemyTarget(Me,Game->GetHero());
                if (seh_me.IsValid()) seh_me.Release();
                for (;;) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (ParentClass.HitEvilBrotherAndProtection>1) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetInFaction(Me,"FACTION_BANDITS");
            Game->GiveThingBestEnemyTarget(Me,Game->GetHero());
            if (seh_me.IsValid()) seh_me.Release();
            for (;;) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}