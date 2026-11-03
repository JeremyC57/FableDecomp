// NScript::CQ_BreakSiegeScript::CGuardToHelpDefend::Main() -- recruit the town guards and respond to hero commands.
// Leaf: ?Main@CGuardToHelpDefend@CQ_BreakSiegeScript@NScript@@EAEXXZ
// Flags: /Ob1
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_COMPARE_NONINLINE_BODY
#define FABLE_RETAIL_CCHARSTRING_PARSER_HELPERS
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CISTHINGCONSCIOUS
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_DECLARED_DTOR
#include <string.h>
#include "engine/CQ_BreakSiegeScript.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CIsThingConscious.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/fable_script_animation_defaults.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
CCharString FABLE_FASTCALL operator+(const char*,const CCharString&);
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}
void NScript::CQ_BreakSiegeScript::CGuardToHelpDefend::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.SetThingHasInformation(Me,false,true,false);
    Game.EntitySetAsAllowedToFollowHero(Me,true);
    CCharString expression;
    CScriptThing ThePrisoner=Game.GetThingWithScriptName("ImprisonedBanditLeader");
    Game.EntitySetThingAsAllyOfThing(Me,ThePrisoner);
    Game.EntitySetThingAsAllyOfThing(ThePrisoner,Me);
    while (!ParentClass.BanditSiegeStart) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            switch (TimesSpokenTo) {
            case 0: {
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_"+Me.GetDataString()+"_TALK_ONCE",false,Me,Game.GetHero());
                break;
            }
            case 1: {
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_"+Me.GetDataString()+"_TALK_TWICE",false,Me,Game.GetHero());
                break;
            }
            case 2: {
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_"+Me.GetDataString()+"_TALK_THRICE",false,Me,Game.GetHero());
                break;
            }
            default: {
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_"+Me.GetDataString()+"_TALK_WILL_FOLLOW",false,Me,Game.GetHero());
                break;
            }
            }
            ++TimesSpokenTo;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_"+Me.GetDataString()+"_ON_HIT",false,Me,Game.GetHero());
        }
        if (Me.MsgExpressionPerformedTo(expression)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (expression=="EXPRESSION_FOLLOW") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                seh_me.PlayAnimation("SALUTE_STAND_TO_ATTENTION_INTO",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_"+Me.GetDataString()+"_FOLLOW",false,Me,Game.GetHero());
                PMasterData->BreakSiegeNoAssistance=false;
                seh_me.Release();
            } else if (expression=="EXPRESSION_WAIT") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.ClearThingHasInformation(Me);
    while (!ParentClass.EndCutsceneDone) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.EntitySetAsAllowedToFollowHero(Me,false);
    Game.EntityStopFollowing(Me);
    seh_me.Release();
    while (!false) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
