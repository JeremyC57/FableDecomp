// NScript::CQ_HeroSoulsArenaScript::CHOHGuard::Main() -- comment on the summoner fight and the hero's actions.
// Leaf: ?Main@CHOHGuard@CQ_HeroSoulsArenaScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/C3DVector.h"
#include "engine/CQ_HeroSoulsArenaScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
#include "engine/CIsThingAlive.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
extern CGameScriptInterfaceBase* pDSTGame;
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator==(long time) const { return GetTime()==time; }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingAlive::~CIsThingAlive() {}
inline bool CScriptThing::IsAlive() const { return !PImp.IsNull() && PImp->IsAlive(); }
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&,const CScriptThing&,float);
bool FABLE_FASTCALL IsDistanceBetweenThingsOver(const CScriptThing&,const CScriptThing&,float);
void NScript::CQ_HeroSoulsArenaScript::CHOHGuard::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    CScriptThing SummonerA=Game->GetThingWithScriptName("Summoner1");
    CScriptThing SummonerB=Game->GetThingWithScriptName("Summoner2");
    CScriptThing Fred=Game->GetNearestWithScriptName(Me,"HOHGuard");
    while (SummonerA.IsAlive() || SummonerB.IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.GuardChatTimer==0 && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),30.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(HLStyleConversationIndex,Game->GetHero());
            if (IsDistanceBetweenThingsOver(Me,Game->GetHero(),15.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Fred.IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B04_FIGHTINGGUARD_COMMENT_10",false,Me,Game->GetHero());
                }
            } else if (!ParentClass.PlayerHitSummoner1 && !ParentClass.PlayerHitSummoner2) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B04_FIGHTINGGUARD_NOTHELPED_10",false,Me,Game->GetHero());
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B04_FIGHTINGGUARD_ATTACKEDENEMY_10",false,Me,Game->GetHero());
            }
            ParentClass.GuardChatTimer=30;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(HLStyleConversationIndex,Game->GetHero());
            Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B04_FIGHTINGGUARD_ONTALKED_10",false,Me,Game->GetHero());
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(HLStyleConversationIndex,Game->GetHero());
            Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B04_FIGHTINGGUARD_ONHIT_10",false,Me,Game->GetHero());
            ParentClass.PlayerAttacksGuard=true;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    for (; !false; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.GuardChatTimer==0 && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),15.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(HLStyleConversationIndex,Game->GetHero());
            if (!ParentClass.PlayerHitSummoner1 && !ParentClass.PlayerHitSummoner2) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B04_FIGHTINGGUARD_ENEMYDEADPLAYERNOHELP_COMMENT_10",false,Me,Game->GetHero());
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B04_FIGHTINGGUARD_ENEMYDEADPLAYERHELPED_COMMENT_10",false,Me,Game->GetHero());
            }
            ParentClass.GuardChatTimer=30;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (!ParentClass.PlayerHitSummoner1 && !ParentClass.PlayerHitSummoner2) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B04_FIGHTINGGUARD_ENEMYDEADPLAYERNOHELP_ONTALKED_10",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B04_FIGHTINGGUARD_ENEMYDEADPLAYERHELPED_ONTALKED_10",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(HLStyleConversationIndex,Game->GetHero());
            Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B04_FIGHTINGGUARD_ENEMYDEAD_ONHIT_10",false,Me,Game->GetHero());
            ParentClass.PlayerAttacksGuard=true;
        }
        if (ParentClass.PlayerAttacksGuard) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->GiveThingBestEnemyTarget(Me,Game->GetHero());
            while (!false) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}