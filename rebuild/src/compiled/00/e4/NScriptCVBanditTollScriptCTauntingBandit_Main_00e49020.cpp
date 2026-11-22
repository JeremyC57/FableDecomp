// NScript::CV_BanditTollScript::CTauntingBandit::Main() -- Handle flight, follow, warnings, calming, and random taunts.
// Leaf: ?Main@CTauntingBandit@CV_BanditTollScript@NScript@@EAEXXZ
// Flags: default
// Grade: RELOCATION_MATCH
// Inline-budget: Selected termination returns keep all destructors out of line. PROBES.md compares ordinary C++, budget flags and each timer-scope pattern.
// Inferred: The selected terminating edges unwind only WarningTimer, then enter the common handle cleanup.

#define FABLE_RETAIL_NSCRIPT
#include "fable_compiler.h"
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include <stdlib.h>
#include "engine/TRetVoid.h"
#include "engine/CV_BanditTollScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CTimer.h"
#include "engine/C3DVector.h"
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#include "engine/EScriptAIPriority.h"
#include "engine/EScriptEntityMoveType.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFPOINTER_ORIGINAL_DIRECT_DEREF_PASS26
#include "engine/fable_script_def_global.h"
extern CGameScriptInterfaceBase* pDSTGame;              // 0x0143E8F8
inline CScriptThing::~CScriptThing() {}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline const C3DVector& CScriptThing::GetPos() const
{
    CScriptThing* implementation = PImp.Data;
    if (!implementation)
        return NullCoord;
    return implementation->GetPos();
}
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing& thing1, const CScriptThing& thing2, float distance);   // 0x00CBE2FF
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
void FABLE_FASTCALL FadeOutAndKillEntity(const CScriptThing& thing, float seconds, bool kill, bool removeOnFade);  // 0x00CBE716
int FABLE_FASTCALL IsPlayerThreateningEntity(const CScriptThing& thing);                         // 0x00CBEED2

void NScript::CV_BanditTollScript::CTauntingBandit::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    CScriptThing BanditEscapeMarker = Game->GetThingWithScriptName("M_BanditEscapePoint");
    {
        CTimer WarningTimer;
        for (;; Game->NewScriptFrame()) {
            if (IsActiveThreadTerminating())
                break;
            if (ParentClass.BanditsFlee) {
                if (IsActiveThreadTerminating()) goto timerEnded;
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) goto timerEnded;
                }
                if (IsActiveThreadTerminating()) goto timerEnded;
                const EScriptEntityMoveType local_movetype = ENTITY_MOVE_RUN;
                float local_range = pGlobalScriptDef->TollBanditCloseToExit;
                C3DVector local_position = BanditEscapeMarker.GetPos();
                while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), local_position, local_range + 2.0f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
                        return _Fail_Val.Return();
#pragma inline_depth()
                    }
                    seh_me.MoveToPosition(local_position, local_range, local_movetype, false, true);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) goto timerEnded;
                    }
                    if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
                        return _Fail_Val.Return();
#pragma inline_depth()
                    }
                }
                if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
                    return _Fail_Val.Return();
#pragma inline_depth()
                }
                FadeOutAndKillEntity(Me, pGlobalScriptDef->TollBanditTimeToFade, true, true);
            }
            if (ParentClass.BanditsAngered && Me.IsAlive() && Me.IsAwareOfHero()) {
                if (IsActiveThreadTerminating()) goto timerEnded;
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGH)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
                        return _Fail_Val.Return();
#pragma inline_depth()
                    }
                }
                if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
                    return _Fail_Val.Return();
#pragma inline_depth()
                }
                Game->EntitySetInFaction(Me, "FACTION_BANDITS");
                seh_me.FollowThing(Game->GetHero(), 1.0f, true);
            }
            if (ParentClass.TollPaid && !ParentClass.BanditsAngered) {
                if (IsActiveThreadTerminating()) goto timerEnded;
                if (WarningTimer.GetTime() == 0) {
                    if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
                        return _Fail_Val.Return();
#pragma inline_depth()
                    }
                    if (IsPlayerThreateningEntity(Me)) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (!AngryWithHero) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            AngryWithHero = true;
                            if (seh_me.IsValid()) seh_me.Release();
                            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGH)) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                        WarningTimer = pGlobalScriptDef->TollBanditTauntTimer;
                        long Conversation = Game->AddNewConversation(Me, false, false);
                        Game->AddPersonToConversation(Conversation, Game->GetHero());
                        switch (AngerCount) {
                        case 0:
                            Game->AddLineToConversation(Conversation, "TEXT_QST_011_BANDIT_WARNING_10", false, Me, Game->GetHero());
                            break;
                        case 1:
                            Game->AddLineToConversation(Conversation, "TEXT_QST_011_BANDIT_WARNING_20", false, Me, Game->GetHero());
                            break;
                        case 2:
                            Game->AddLineToConversation(Conversation, "TEXT_QST_011_BANDIT_WARNING_30", false, Me, Game->GetHero());
                            break;
                        case 3:
                            Game->AddLineToConversation(Conversation, "TEXT_QST_011_BANDIT_ANGERED_10", false, Me, Game->GetHero());
                            ParentClass.BanditsAngered = true;
                            break;
                        }
                        ++AngerCount;
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (AngryWithHero) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            long Conversation = Game->AddNewConversation(Me, false, false);
                            Game->AddPersonToConversation(Conversation, Game->GetHero());
                            Game->AddLineToConversation(Conversation, "TEXT_QST_011_BANDIT_NO_LONGER_THREATENING_10", false, Me, Game->GetHero());
                            AngryWithHero = false;
                            if (seh_me.IsValid()) seh_me.Release();
                        }
                        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)pGlobalScriptDef->TollBanditPayDistance)) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (rand() % 300 == 0) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                long Conversation = Game->AddNewConversation(Me, false, false);
                                Game->AddPersonToConversation(Conversation, Game->GetHero());
                                Game->AddLineToConversation(Conversation, "TEXT_QST_011_BANDIT_RANDOM_TAUNT", false, Me, Game->GetHero());
                            }
                        }
                    }
                }
            }
        }
        return _Fail_Val.Return();
    }
timerEnded:
#pragma inline_depth(0)
    return _Fail_Val.Return();
#pragma inline_depth()
}