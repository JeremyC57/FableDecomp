// NScript::CQ_GuildTrainingScript::CWillDummy::Main() -- animate the will dummy and award lightning hits with training feedback.
// Leaf: ?Main@CWillDummy@CQ_GuildTrainingScript@NScript@@EAEXXZ
// Grade: RELOCATION_MATCH(2168v2168) -- original generated loop and proven timer helper expansion.
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/CQ_GuildTrainingScript.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/TRetVoid.h"
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
extern CGameScriptInterfaceBase* pDSTGame;
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
FABLE_FORCEINLINE CTimer& CTimer::operator=(long n) { pDSTGame->SetTimer(TimerIndex,n); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator<=(long n) const { return GetTime()<=n; }

void NScript::CQ_GuildTrainingScript::CWillDummy::Main()
{
    float initial_angle=Me.GetAngleXY();
    TRetVoid _Fail_Val;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    ParentClass.WillHelpTimer=15;
    while (PMasterData->WillTrainingStarted != true) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    for (;!false;Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!(Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_LIGHTNING_SPELL,"SCRIPT_NAME_HERO") || Me.MsgIsHitBy("SCRIPT_NAME_HERO"))) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_LIGHTNING_SPELL,"SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntityPlayObjectAnimation(Me,"GET_HIT_SPIN",false);
            Game->EntitySetFacingAngle(Me,initial_angle+0.25,true);
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetFacingAngle(Me,initial_angle+0.5,true);
            ++PMasterData->WillScore;
            if (ParentClass.WillHelpTimer.GetTime()<=0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetThingWithScriptName("WillApprentice").IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long Conversation=Game->AddNewConversation(Game->GetThingWithScriptName("WillApprentice"),false,false);
                    Game->AddPersonToConversation(Conversation,Game->GetHero());
                    Game->AddLineToConversation(Conversation,"TEXT_QST_028_APPRENTICE_WILL_GOOD_HIT",false,Game->GetThingWithScriptName("WillApprentice"),Game->GetHero());
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long Conversation=Game->AddNewConversation(Game->GetThingWithScriptName("TheRealGuildmaster"),false,false);
                    Game->AddPersonToConversation(Conversation,Game->GetHero());
                    Game->AddLineToConversation(Conversation,"TEXT_QST_028_GUILDMASTER_WILL_GOOD_HIT",false,Game->GetThingWithScriptName("TheRealGuildmaster"),Game->GetHero());
                }
                ParentClass.WillHelpTimer=7;
            }
            Game->EntitySetTargetable(Me,false);
            Game->Pause(pGlobalScriptDef->GUI_WillDummySpinTimer);
            Game->EntitySetFacingAngle(Me,initial_angle+0.25,true);
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetFacingAngle(Me,initial_angle,true);
            Game->EntitySetTargetable(Me,true);
        } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntityPlayObjectAnimation(Me,"GET_HIT",false);
            if (ParentClass.WillHelpTimer.GetTime()<=0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetThingWithScriptName("WillApprentice").IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long Conversation=Game->AddNewConversation(Game->GetThingWithScriptName("WillApprentice"),false,false);
                    Game->AddPersonToConversation(Conversation,Game->GetHero());
                    Game->AddLineToConversation(Conversation,"TEXT_QST_028_APPRENTICE_WILL_TROUBLE",false,Game->GetThingWithScriptName("WillApprentice"),Game->GetHero());
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long Conversation=Game->AddNewConversation(Game->GetThingWithScriptName("TheRealGuildmaster"),false,false);
                    Game->AddPersonToConversation(Conversation,Game->GetHero());
                    Game->AddLineToConversation(Conversation,"TEXT_QST_028_GUILDMASTER_WILL_TROUBLE",false,Game->GetThingWithScriptName("TheRealGuildmaster"),Game->GetHero());
                }
                ParentClass.WillHelpTimer=7;
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}