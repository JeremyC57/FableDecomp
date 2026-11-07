// NScript::CQ_HangingTreeEvilScript::CHTE_FreedomFighter::Main() -- Hanging Tree (evil) freedom fighter: waits for the intro cutscene, takes scripting control, then while the quest is running heads for the bandit exit point (yelling, following the hero once close) and, when the enemies are gone, attacks the nearest assault guard
// Shape (chunk 832): the assault guard is a VALUE local whose three IsAlive tests go through a same-type
//   `reinterpret_cast<const CScriptThing&>` (the landed CMapKeeper::Main 0x00DA91A0 form, owner-accepted 2026-11-07): VC7.1 keeps
//   the calls virtual (a plain value local devirtualises them, a reference bound to the temporary loads the vptr before `this`)
//   and computes `this` first, so the inner follow-wait loop's terminate `jne` gets retail's hoisted `lea ecx,[esp+2Ch]` (0x00D6AC5A,
//   target 0x00D6AD54 past the shared exit pad's lea). Chunk 781's reference form was the one-instruction residue.
// Leaf: ?Main@CHTE_FreedomFighter@CQ_HangingTreeEvilScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D6A560, 2058 bytes, thiscall, plain `ret`; frame 0x48, ebx pushed at entry (CIsThingAlive vtable 0x012C32F0 in ebx, then the zero register), ebp pushed lazily at the HTE_State != 6 loop head.
// Identity (proven): slot 1 (Main) of the entity vtable 0x012D2170 that CHTE_FreedomFighter::Alloc 0x00D6A3D0 stores (slots ~ 0x00D6A540, Main
//   0x00D6A560, Init 0x00D6A480, GetParentScript 0x00D6A470, OnPredicateFail 0x00D6A4F0: the landed siblings); Ego_r.pdb + FableWin.pdb
//   ?Main@CHTE_FreedomFighter@CQ_HangingTreeEvilScript@NScript@@EAEXXZ. The ledger's `NScript::CHTE_FreedomFighter::Main` misses the
//   CQ_HangingTreeEvilScript scope.
// Callees: CIsThingAlive (inline ctor/dtor: CScriptThing copy 0x004ABE90, ~CScriptThing 0x004AA840), SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30; scripted-thing handle (vtable 0x0127094C, CBaseObject ctor 0x0099A380, IsValid 0x00CD23B9,
//   Release 0x00CD2770, MoveToPosition 0x007E72F0, FollowThing 0x007E7320, ClearCommands 0x007E7360, IsPerformingScriptTask
//   0x007E7450, dtor 0x007E74D0); CCharString 0x0099EBF0 / 0x0099EAE0; IsDistanceBetweenThingsUnder 0x00CBE2FF / Over 0x00CBE3EA
//   (__fastcall); interface slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 72 GetThingWithScriptName, 77
//   GetNearestWithScriptName, 235 EntitySetAsAllowedToFollowHero, 364 AddNewConversation, 366 AddLineToConversation, 451
//   GiveThingBestEnemyTarget, 480 SetEntityAsRegionFollowing; through pDSTGame 0x0143E8F8 slot 90 GetTimer (CTimer::GetTime inline);
//   CScriptThing slots 6 GetPos (inline, NullCoord 0x0143E8E0), 21 MsgIsHitBy, 75 IsAlive (virtual through the guard reference).
// Data: "HTE_BanditExitPoint" 0x012D2228, "TEXT_QST_029_BANDIT_CHARGE" 0x012D220C, "..._HURRY_MILD" 0x012D21EC, "..._HURRY_URGENT"
//   0x012D21C8, "TEXT_QST_029_BANDITS_FOLLOWING" 0x012D21A8, "SCRIPT_NAME_HERO" 0x0125D1C8, "HTE_AssaultGuard" 0x012D2064.
// Layout: engine/CQ_HangingTreeEvilScript.h (chunk 744; this chunk completes the nested CHTE_FreedomFighter: ParentClass +0x14,
//   PMasterData +0x18, HeroHitMe +0x1c, Ego_r.pdb); parent flags IntroCutscenePlayed +0x48, SwitchingRegions +0x51, AllEnemiesDead
//   +0x53, BanditYelledRecently +0x55, ShouldFollowHero +0x58, HTE_State +0x60, MissionTimer +0xa4.
// Shape: the guard is a reference bound to the GetNearestWithScriptName temporary (retail calls IsAlive through the vptr: a
//   by-value local is devirtualized); the trailing terminate check of the guard block sits in both arms of its if/else (one call,
//   the else arm jumping to it with ecx preloaded); GetPos / GetTime are __forceinline (retail expands both at both sites).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CQ_HangingTreeEvilScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST = 0, SCRIPT_AI_PRIORITY_LOW = 1, SCRIPT_AI_PRIORITY_MEDIUM = 2,
                         SCRIPT_AI_PRIORITY_HIGH = 3, SCRIPT_AI_PRIORITY_HIGHEST = 4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
__forceinline const C3DVector& CScriptThing::GetPos() const { return !PImp.Data ? NullCoord : PImp.Data->GetPos(); }
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool FABLE_FASTCALL IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA

void NScript::CQ_HangingTreeEvilScript::CHTE_FreedomFighter::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass->IntroCutscenePlayed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase fighter;
    if (fighter.IsValid())
        fighter.Release();
    while (!Game->StartScriptingEntity(Me, fighter, SCRIPT_AI_PRIORITY_HIGH)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (ParentClass->SwitchingRegions) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->ShouldFollowHero = false;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (ParentClass->HTE_State != 6) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        while (ParentClass->AllEnemiesDead) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            CScriptThing exitPoint = Game->GetThingWithScriptName("HTE_BanditExitPoint");
            if (!ParentClass->BanditYelledRecently) {
                if (IsActiveThreadTerminating())
                    return;
                long conversation = Game->AddNewConversation(Me, false, false);
                if (ParentClass->MissionTimer.GetTime() >= 90)
                    Game->AddLineToConversation(conversation, "TEXT_QST_029_BANDIT_CHARGE", false, Me, Me);
                else if (ParentClass->MissionTimer.GetTime() >= 30)
                    Game->AddLineToConversation(conversation, "TEXT_QST_029_BANDIT_HURRY_MILD", false, Me, Me);
                else
                    Game->AddLineToConversation(conversation, "TEXT_QST_029_BANDIT_HURRY_URGENT", false, Me, Me);
                ParentClass->BanditYelledRecently = true;
            }
            if (ParentClass->ShouldFollowHero) {
                if (IsActiveThreadTerminating())
                    return;
                fighter.ClearCommands();
                Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
                Game->EntitySetAsAllowedToFollowHero(Me, true);
                fighter.FollowThing(Game->GetHero(), 1.0f, true);
                if (!ParentClass->BanditYelledRecently) {
                    if (IsActiveThreadTerminating())
                        return;
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddLineToConversation(conversation, "TEXT_QST_029_BANDITS_FOLLOWING", false, Me, Game->GetHero());
                    ParentClass->BanditYelledRecently = true;
                }
                while (!ParentClass->SwitchingRegions) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                fighter.ClearCommands();
            } else if (ParentClass->HTE_State != 6) {
                if (IsActiveThreadTerminating())
                    return;
                fighter.ClearCommands();
                fighter.MoveToPosition(exitPoint.GetPos(), 2.0f, (EScriptEntityMoveType)1, false, true);
                while (!ParentClass->SwitchingRegions && ParentClass->AllEnemiesDead && !ParentClass->ShouldFollowHero
                       && IsDistanceBetweenThingsOver(Me, exitPoint, 8.0f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                    if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
                        if (IsActiveThreadTerminating())
                            return;
                        ++HeroHitMe;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                    if (HeroHitMe >= 3) {
                        if (IsActiveThreadTerminating())
                            return;
                        if (fighter.IsValid())
                            fighter.Release();
                        Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
                        return;
                    }
                    if (!fighter.IsPerformingScriptTask()) {
                        if (IsActiveThreadTerminating())
                            return;
                        fighter.MoveToPosition(exitPoint.GetPos(), 2.0f, (EScriptEntityMoveType)1, false, true);
                    }
                }
                if (IsActiveThreadTerminating())
                    return;
                if (IsDistanceBetweenThingsUnder(Me, exitPoint, 8.0f)
                    || IsDistanceBetweenThingsUnder(Game->GetHero(), exitPoint, 13.0f)) {
                    if (IsActiveThreadTerminating())
                        return;
                    ParentClass->ShouldFollowHero = true;
                    ParentClass->BanditYelledRecently = false;
                }
            }
        }
        if (IsActiveThreadTerminating())
            return;
        fighter.ClearCommands();
        CScriptThing guard = Game->GetNearestWithScriptName(Me, "HTE_AssaultGuard");
        if (reinterpret_cast<const CScriptThing&>(guard).IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            Game->GiveThingBestEnemyTarget(Me, guard);
            while (reinterpret_cast<const CScriptThing&>(guard).IsAlive()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                if (IsDistanceBetweenThingsOver(Me, guard, 5.0f)) {
                    if (IsActiveThreadTerminating())
                        return;
                    fighter.FollowThing(guard, 1.0f, true);
                    while (reinterpret_cast<const CScriptThing&>(guard).IsAlive() && !IsDistanceBetweenThingsUnder(Me, guard, 5.0f)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                    fighter.ClearCommands();
                }
            }
            if (IsActiveThreadTerminating())
                return;
        } else {
            if (IsActiveThreadTerminating())
                return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    fighter.FollowThing(Game->GetHero(), 1.0f, true);
}