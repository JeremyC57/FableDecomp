// 00e4aa00 NScript::CV_BanditTollScript::CBanditLeader::Main
// Inline-budget: retail expands handle cleanup at outer-loop termination0x00E4AE2C; other exits call0x007E74D0.
// Grade: RELOCATION_MATCH1356/1356; behavior and complete relocation checks recorded separately.
// (NF344, 2026-11-24) /d2inlT200 dropped: on the current headers the object is identical without it (every function COMDAT, bytes and relocations; NF340 /d2inl audit).
// Leaf: ?Main@CBanditLeader@CV_BanditTollScript@NScript@@EAEXXZ
#include "fable_compiler.h"
#include "engine/TRetVoid.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CPredicateBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_BanditTollScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_animation_defaults.h"
// Retail expands these at this row's sites: defined inline after the headers.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator==(long time) const { return GetTime() == time; }
using NScript::CIsThingAlive;

void NScript::CV_BanditTollScript::CBanditLeader::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (ParentClass.BanditsAngered && Me.IsAlive() && Game->IsThingAwareOfOtherThingInAnyWay(Game->GetHero(), Me)) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, (EScriptAIPriority)3)) {
                Game->NewScriptFrame();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            Game->EntitySetInFaction(Me, "FACTION_BANDITS");
            seh_me.FollowThing(Game->GetHero(), 1.0f, true);
            for (;;) {
                Game->NewScriptFrame();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
        }
        if (TalkFrequency == 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 10.0f)) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, (EScriptAIPriority)4)) {
                Game->NewScriptFrame();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            long HLStyleConversationIndex;
            switch (TalkedToHero) {
            case 0: {
                HLStyleConversationIndex = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_011_BANDIT_LEADER_GO_AWAY", false, Me, Game->GetHero());
                TalkFrequency = (15);
                break;
            }
            case 1: {
                HLStyleConversationIndex = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_011_BANDIT_LEADER_GO_AWAY_AGAIN", false, Me, Game->GetHero());
                TalkFrequency = (12);
                break;
            }
            case 2: {
                HLStyleConversationIndex = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_011_BANDIT_LEADER_GO_AWAY_AGAIN_AGAIN", false, Me, Game->GetHero());
                TalkFrequency = (10);
                break;
            }
            default: {
                HLStyleConversationIndex = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
                Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_011_BANDIT_LEADER_GO_AWAY_SHORT", false, Me, Game->GetHero());
                TalkFrequency = (8);
                break;
            }
            }
            if (TalkedToHero == 0 || rand() % 5 == 0) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                seh_me.PlayAnimation("ST_OPINION_DISAPPROVAL_BOOING", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                TalkFrequency = (8);
            }
            seh_me.Release();
            TalkedToHero++;
        }
    }
}