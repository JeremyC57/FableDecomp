// NScript::CQ_SummoningTheShipScript::CSTS_BriarRose::Main() -- guide Briar Rose targeting and quest commentary.
// Leaf: ?Main@CSTS_BriarRose@CQ_SummoningTheShipScript@NScript@@EAEXXZ
// Grade: RELOCATION_MATCH(2139v2139) -- genuine timer helper needs stronger expansion under newest Owner conditional preference.
// Annotation: Original keyword unproven; ordinary inline attempts33-41 retained.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CISTHINGALIVE
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CTIMER
#include "engine/CQ_SummoningTheShipScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/TRetVoid.h"
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
float FABLE_FASTCALL GetDistanceBetweenThings(const CScriptThing&,const CScriptThing&);
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator==(long time) const { return GetTime()==time; }
inline CScriptThing::CScriptThing() {}
inline bool CScriptThing::IsDead() const { return !IsAlive(); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingAlive::~CIsThingAlive() {}

void NScript::CQ_SummoningTheShipScript::CSTS_BriarRose::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!ParentClass.SummonerAttacksStarted) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    long CurrentAIState=0;
    long PreviousAIState=0;
    CTimer AttackedByEnemyTimer;
    long SummonerCountComment=0;
    BrainState=2;
    Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_SUMMONING_SHIP_OBJECTIVE_03","HookCoast","HookCoast");
    FollowHero(true);
    long BriarMode=0;
    CScriptThing Enemy;
    for (;!false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.CurrentAttackWave==1 && BriarMode!=1) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Enemy=Game.GetNearestWithScriptName(Me,"SummonerAttacker");
            if (Enemy.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.GiveThingBestEnemyTarget(Me,Enemy);
            }
            BriarMode=1;
        } else if (ParentClass.CurrentAttackWave!=1 && BriarMode==1 && Enemy.IsDead()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing Summoner=Game.GetNearestWithScriptName(Me,"SummonerAttacker");
            CScriptThing Minion=Game.GetNearestWithScriptName(Me,"SummonerMinion");
            if (GetDistanceBetweenThings(Me,Summoner)>GetDistanceBetweenThings(Me,Minion)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Enemy=Minion;
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Enemy=Summoner;
            }
            if (Enemy.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.GiveThingBestEnemyTarget(Me,Enemy);
            }
        }
        if (SummonerCountComment!=ParentClass.SummonersAlive) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CCharString SummonerCountCommentString;
            switch (ParentClass.SummonersAlive) {
            case 1: SummonerCountCommentString="SUMMONER_ONE_LEFT"; break;
            case 2: SummonerCountCommentString="SUMMONER_TWO_LEFT"; break;
            case 3: SummonerCountCommentString="SUMMONER_THREE_LEFT"; break;
            }
            if (ParentClass.SummonersAlive>0 && ParentClass.SummonersAlive<=3 && ParentClass.MakeBriarRoseComment(SummonerCountCommentString)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                SummonerCountComment=ParentClass.SummonersAlive;
            }
        }
        CurrentAIState=Me.GetCurrentStateGroupType();
        if (PreviousAIState!=CurrentAIState) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (CurrentAIState==2) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.MakeBriarRoseComment("FOLLOWING");
            } else if (CurrentAIState==1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.MakeBriarRoseComment("ATTACKING");
            }
        }
        PreviousAIState=CurrentAIState;
        if (AttackedByEnemyTimer==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Me.MsgIsHitBy("SummonerAttacker") || (Me.MsgIsHitByAnySpecialAbilityFrom("SummonerAttacker") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass.MakeBriarRoseComment("SUMMONER_ATTACKED")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    AttackedByEnemyTimer=30;
                }
            }
            if (Me.MsgIsHitBy("SummonerMinion") || (Me.MsgIsHitByAnySpecialAbilityFrom("SummonerMinion") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass.MakeBriarRoseComment("MINION_ATTACKED")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    AttackedByEnemyTimer=30;
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.MakeBriarRoseComment("ATTACKED");
        }
    }
}
