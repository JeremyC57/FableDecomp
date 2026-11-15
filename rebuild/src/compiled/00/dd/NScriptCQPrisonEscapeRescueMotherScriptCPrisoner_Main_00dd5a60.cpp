// 00dd5a60 NScript::CQ_PrisonEscapeRescueMotherScript::CPrisoner::Main() -- releases the prisoner, follows the hero and reacts to comments/hits.
// Inline-budget: native GUARDS hit exit 00DD6293 branches to shared cleanup 00DD5B0B, which calls timer destruction. Plain source and T44 diagnostic prove the budget conflict; scoped depth0 preserves this required cleanup call.
// Retail: 0x00DD5A60,2250 bytes,thiscall,complete body through plain ret.
// Flags: none
// Leaf: ?Main@CPrisoner@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTTHING_ORIGINAL_ACCESS
#define FABLE_RETAIL_CQ_PRISONESCAPERESCUEMOTHERSCRIPT_PRISONER
#include <stdlib.h>
#include "fable_string.h"
#include "engine/EHeroAbility.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_PrisonEscapeRescueMotherScript.h"
#include "engine/TRetVoid.h"
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator==(long time) const { return GetTime()==time; }
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::MsgIsUsedBy(const CCharString& user) const { if (!PImp.Data) return false; return PImp.Data->MsgIsUsedBy(user); }

void NScript::CQ_PrisonEscapeRescueMotherScript::CPrisoner::Main()
{
    TRetVoid _Fail_Val;
    CTimer TalkTimer;
    TalkTimer = rand() % 90;
    if (!ReleasedAndFollowing) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptThing CellDoor = Game.GetNearestWithScriptName(Me, "CellDoor");
        while (!ReleasedAndFollowing) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.HeroEscapedCell && TalkTimer == 0 && ParentClass.MakePrisonerComment(Me, "RELEASE")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                TalkTimer = 30;
            }
            if (!ParentClass.HeroEscapedCell && TalkTimer == 0 && ParentClass.MakePrisonerComment(Me, "IDLE")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                TalkTimer = 30;
            }
            if (CellDoor.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                while (Game.IsConversationActive(ParentClass.ConversationIndex)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_055_PRISON_PRISONER_FREE", false, Me, Game.GetHero());
                ReleasedAndFollowing = true;
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    Game.EntitySetAsAllowedToFollowHero(Me, true);
    Game.EntityFollowThing(Me, Game.GetHero(), 3.0f, true);
    Game.SetEntityAsRegionFollowing(Game.GetHero(), Me, true);
    Game.SetThingPersistent(Me, true);
    Game.EntitySetInFaction(Me, "FACTION_PRISONER");
    while (!Game.IsRegionLoaded("PrisonCourtyard")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (TalkTimer == 0 && ParentClass.MakePrisonerComment(Me, "COURTYARD")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            TalkTimer = 30;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_055_PRISON_PRISONER_ON_HIT", false, Me, Game.GetHero());
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.SetEntityAsRegionFollowing(Game.GetHero(), Me, false);
    for (;; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (TalkTimer == 0 && ParentClass.MakePrisonerComment(Me, "GUARDS")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            TalkTimer = 30;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_055_PRISON_PRISONER_ON_HIT", false, Me, Game.GetHero());
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}