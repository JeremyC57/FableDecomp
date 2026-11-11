// NScript::CV_BanditTollScript::CBanditLeader::Main() -- bandit-toll leader: alive predicate on Me, then every frame until terminated: once the bandits are angered and Me (alive) is aware of the hero, join FACTION_BANDITS and follow the hero for good; otherwise, whenever the talk timer has run out and the hero is within 10 units, face the hero, tell him to go away (four escalating lines, re-arming the timer with 15/12/10/8), sometimes boo, and count the talk
// Grade: DIFFER(1356v1356) -- one register: the inline handle release before operator delete reloads Info into eax (`mov eax,[esp+30h] ; push eax`) where retail uses ecx (0x00E4AE44); everything else matches (shared headers, /d2inlT200 + scoped inline_depth(0) put the handle dtor inline only at the loop exit as retail). c2prio --rotation: the temp gets eax by plain rotation (pointer at eax); retail needs the pointer one step on. Moving the angered block to the loop end (negated condition + continue) is byte-identical.
// Inline-budget: the scripted-thing handle's implicit destructor is expanded only at the loop exit (0x00E4AE2C, laid right after the outer loop) and called out of line (0x007E74D0) at every return: /d2inlT200 lets the scope-end site under the threshold, and scoped `#pragma inline_depth(0)` regions keep the in-loop returns out of line (as Q9's 0x00E1CAE0).
// Flags: /d2inlT200
// Leaf: ?Main@CBanditLeader@CV_BanditTollScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E4AA00, 1356 bytes (1360 with padding), plain `ret`; entity vtable slot 1 (Ego_r.pdb
//   ?Main@CBanditLeader@CV_BanditTollScript@NScript@@EAEXXZ).
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840 (the predicate's), CIsThingAlive vtable 0x012C32F0 / ~CIsThingAlive
//   0x00E241B0, CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, CBaseObject() 0x0099A380,
//   the scripted-thing handle (vtable 0x0127094C; dtor 0x007E74D0) IsValid 0x00CD23B9 / Release 0x00CD2770 / FollowThing
//   0x007E7320 / PlayAnimation 0x007E73D0 / IsPerformingScriptTask 0x007E7450, IsDistanceBetweenThingsUnder 0x00CBE2FF, rand
//   0x00BFEB16; Game slots 7, 8, 70, 364/365/366, 475 EntitySetFacingAngleTowardsThing, 601 EntitySetInFaction, 636
//   IsThingAwareOfOtherThingInAnyWay; the timer through pDSTGame 0x0143E8F8 slots 89 SetTimer / 90 GetTimer; CScriptThing slot 75.
// Literals: TEXT_QST_011_BANDIT_LEADER_GO_AWAY 0x012E4FE0 / _AGAIN 0x012E5004 / _AGAIN_AGAIN 0x012E5030 / _SHORT 0x012E5060,
//   "ST_OPINION_DISAPPROVAL_BOOING" 0x01299A04, "FACTION_BANDITS" 0x0125FE68; NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations 0x01375748.
// Layout: engine/CV_BanditTollScript.h (Ego_r.pdb CBanditLeader 36 bytes): ParentClass +0x14 (BanditsAngered +0x48), PMasterData
//   +0x18, TalkedToHero +0x1c, CTimer TalkFrequency +0x20.
#include "fable_compiler.h"
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
using NScript::CIsThingAlive;

void NScript::CV_BanditTollScript::CBanditLeader::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    while (!IsActiveThreadTerminating()) {
        if (ParentClass.BanditsAngered && Me.IsAlive() && Game->IsThingAwareOfOtherThingInAnyWay(Game->GetHero(), Me)) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            if (scripted.IsValid())
                scripted.Release();
            while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)3)) {
                Game->NewScriptFrame();
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
            }
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            Game->EntitySetInFaction(Me, "FACTION_BANDITS");
            scripted.FollowThing(Game->GetHero(), 1.0f, true);
            for (;;) {
                Game->NewScriptFrame();
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
            }
        }
        if (TalkFrequency.GetTime() == 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 10.0f)) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            if (scripted.IsValid())
                scripted.Release();
            while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
                Game->NewScriptFrame();
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
            }
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            while (scripted.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
            }
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            switch (TalkedToHero) {
            case 0: {
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_011_BANDIT_LEADER_GO_AWAY", false, Me, Game->GetHero());
                TalkFrequency = (15);
                break;
            }
            case 1: {
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_011_BANDIT_LEADER_GO_AWAY_AGAIN", false, Me, Game->GetHero());
                TalkFrequency = (12);
                break;
            }
            case 2: {
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_011_BANDIT_LEADER_GO_AWAY_AGAIN_AGAIN", false, Me, Game->GetHero());
                TalkFrequency = (10);
                break;
            }
            default: {
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_011_BANDIT_LEADER_GO_AWAY_SHORT", false, Me, Game->GetHero());
                TalkFrequency = (8);
                break;
            }
            }
            if (TalkedToHero == 0 || rand() % 5 == 0) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                scripted.PlayAnimation("ST_OPINION_DISAPPROVAL_BOOING", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
                while (scripted.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
#pragma inline_depth(0)
                    if (IsActiveThreadTerminating())
                        return;
#pragma inline_depth()
                }
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                TalkFrequency = (8);
            }
            scripted.Release();
            TalkedToHero++;
        }
        Game->NewScriptFrame();
    }
}
