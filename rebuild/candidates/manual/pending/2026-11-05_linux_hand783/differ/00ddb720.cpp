// NScript::CQ_PrisonRaceScript::CPrisonRaceGuard::Main() -- prison race guard: take Me under scripted control, pause 1 s, teleport Me to GuardPosition (weapons sheathed) and face its angle; then each frame: when the hero is within DistanceToPlayAnim and the taunt timer is clear, add the RACE_TAUNT conversation line, play Me's data-string animation and rearm the timer for 10 s; when the hero hits him, fight the hero until 20 s pass without another hit, then stop fighting, re-take control, walk back to GuardPosition, face its angle and pause 2 s
// Grade: DIFFER(2150v2158) -- VC7.1 inline split of ~CTimer per exit site: retail calls 0x00CD4470 at the checks after the taunt speak loop, after the post-fight StartScriptingEntity wait (and inside it) and after the walk-back loop, where this build expands the DeregisterTimer body; every other instruction matches.
// Next idea: find what makes those after-loop returns cold in retail (INLINE_DEPTH_REVISIT.md frequency model: the code after the fight loop keeps its frequency here); marker-call measurement per site.
// Leaf: ?Main@CPrisonRaceGuard@CQ_PrisonRaceScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DDB720, 2158 bytes, thiscall, plain ret; entity-script vtable 0x012DC97C slot 1 (Main).
// Identity (proven): Ego_r.pdb ?Main@CPrisonRaceGuard@CQ_PrisonRaceScript@NScript@@EAEXXZ (Ego_r 0x00E13F40 after its Alloc / Init; retail
//   donor order Alloc 0x00DDB4A0, GetParentScript 0x00DDB540, Init 0x00DDB570, Main 0x00DDB720); the landed Alloc stores the vtable whose
//   slot 1 is this; DistanceToPlayAnim +0x1c is the Ego_r CPrisonRaceGuard member the landed Init writes; literals GuardPosition /
//   TEXT_QST_055_RACE_TAUNT. The ledger's under-qualified NScript::CPrisonRaceGuard::Main is this.
// Callees: CBaseObject() 0x0099A380 / ~ 0x0099A430, scripted-thing handle IsValid / Release / GetScriptThing / IsPerformingScriptTask /
//   MoveToPosition 0x007E72F0 / PlayAnimation 0x007E73D0 / dtor 0x007E74D0, ~CScriptThing 0x004AA840 / ~CBaseClass 0x0099A2E0 /
//   GetAngleXY 0x004AAA40 (out of line at the last site only), operator delete 0x00BFE9BC, IsDistanceBetweenThingsUnder 0x00CBE2FF,
//   IsDistanceFromThingToPositionOver 0x00CBE45C, ~CTimer 0x00CD4470, CCharString 0x0099EBF0 / 0x0099EAE0; CGameScriptInterfaceBase
//   slots 7, 8, 70, 77 GetNearestWithScriptName, 364/365/366, 376 Pause, 451 GiveThingBestEnemyTarget, 452 ClearThingBestEnemyTarget,
//   472 EntityTeleportToThing, 474 EntitySetFacingAngle, 506 EntitySheatheWeapons; pDSTGame timer slots 87-90; CScriptThing slots
//   3 GetDataString, 6 GetPos, 10 GetAngleXY, 21, 41, 42; NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations 0x01375748;
//   CScriptThing::NullCoord 0x0143E8E0.
// Key spellings: the main loop as for(;;) { do { if (T) return; } while (0); ... } (INLINE_DEPTH_REVISIT lever 4: expands the
//   loop-exit dtors), GetPos __forceinline with the !PImp.Data ? NullCoord : ... arm order, GetAngleXY plain inline (expanded twice,
//   called once, as retail), ~CScriptThing plain inline (the frame then matches).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CSCRIPTTHING_NULLCOORD
#include "engine/CQ_PrisonRaceScript.h"
#include "engine/C3DVector.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/EHeroAbility.h"
#include "engine/CTimer.h"
#include "engine/fable_script_distance.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST = 0, SCRIPT_AI_PRIORITY_LOW = 1, SCRIPT_AI_PRIORITY_MEDIUM = 2,
                         SCRIPT_AI_PRIORITY_HIGH = 3, SCRIPT_AI_PRIORITY_HIGHEST = 4 };
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing&, const C3DVector&, float);   // 0x00CBE45C
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
extern CGameScriptInterfaceBase* pDSTGame;                   // 0x0143E8F8
inline CScriptThing::~CScriptThing() { }
inline float CScriptThing::GetAngleXY() const { return !PImp.Data ? 0.0f : PImp.Data->GetAngleXY(); }
__forceinline const C3DVector& CScriptThing::GetPos() const { return !PImp.Data ? NullCoord : PImp.Data->GetPos(); }
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

void NScript::CQ_PrisonRaceScript::CPrisonRaceGuard::Main()
{
    CScriptGameResourceObjectScriptedThingBase guard;
    if (guard.IsValid())
        guard.Release();
    while (!Game->StartScriptingEntity(Me, guard, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->Pause(1.0f);
    CScriptThing guardPosition = Game->GetNearestWithScriptName(Me, "GuardPosition");
    Game->EntitySheatheWeapons(Me, false);
    Game->EntityTeleportToThing(Me, guardPosition, false);
    Game->EntitySetFacingAngle(Me, guardPosition.GetAngleXY(), true);
    Game->EntitySetFacingAngle(Me, guardPosition.GetAngleXY(), true);
    CTimer tauntTimer;
    for (;;) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)DistanceToPlayAnim) && tauntTimer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_055_RACE_TAUNT", false, Me, Game->GetHero());
            guard.PlayAnimation(Me.GetDataString(), false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
            while (guard.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            tauntTimer = 10;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            if (guard.IsValid())
                guard.Release();
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
            CTimer fightTimer;
            fightTimer = 20;
            while (fightTimer.GetTime() > 0) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                    || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                        && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                    if (IsActiveThreadTerminating())
                        return;
                    fightTimer = 20;
                }
            }
            Game->ClearThingBestEnemyTarget(Me);
            if (guard.IsValid())
                guard.Release();
            while (!Game->StartScriptingEntity(Me, guard, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            C3DVector position = guardPosition.GetPos();
            while (IsDistanceFromThingToPositionOver(guard.GetScriptThing(), position, 2.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                guard.MoveToPosition(position, 0.0f, (EScriptEntityMoveType)1, false, true);
                while (guard.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetFacingAngle(Me, guardPosition.GetAngleXY(), false);
            Game->Pause(2.0f);
        }
        Game->NewScriptFrame();
    }
}
