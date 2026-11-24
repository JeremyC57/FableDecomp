// NScript::CQ_PrisonRaceScript::CPrisonRaceGuard::Main
// Leaf: ?Main@CPrisonRaceGuard@CQ_PrisonRaceScript@NScript@@EAEXXZ
// Flags:
#define FABLE_RETAIL_CQ_PRISONRACESCRIPT_GUARD_PASS27
#define FABLE_RETAIL_CSCRIPTTHING_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTRESOURCE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_SCRIPT_MOVE_ENUM
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CSCRIPTTHING_NULLCOORD
#include "engine/CQ_PrisonRaceScript.h"
#include "engine/TRetVoid.h"
#include "engine/C3DVector.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/EHeroAbility.h"
#include "engine/CTimer.h"
#include "engine/fable_script_distance.h"
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#include "engine/EScriptAIPriority.h"
#include "engine/fable_script_animation_defaults.h"
extern CGameScriptInterfaceBase* pDSTGame;                   // 0x0143E8F8
inline CScriptThing::~CScriptThing() { }
inline float CScriptThing::GetAngleXY() const
{
    if (PImp.IsNull())
        return 0.0f;
    return (PImp->GetAngleXY());
}
FABLE_FORCEINLINE const C3DVector& CScriptThing::GetPos() const
{
    if (PImp.IsNull())
        return NullCoord;
    return PImp->GetPos();
}
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline bool CTimer::operator==(long time) const { return GetTime() == time; }
inline bool CTimer::operator>(long time) const { return GetTime() > time; }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

void NScript::CQ_PrisonRaceScript::CPrisonRaceGuard::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
    }
    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
    Game.Pause(1.0f);
    CScriptThing HomePosition = Game.GetNearestWithScriptName(Me, "GuardPosition");
    Game.EntitySheatheWeapons(Me, false);
    Game.EntityTeleportToThing(Me, HomePosition, false);
    Game.EntitySetFacingAngle(Me, HomePosition.GetAngleXY(), true);
    Game.EntitySetFacingAngle(Me, HomePosition.GetAngleXY(), true);
    CTimer WaveTimer;
    for (; true; Game.NewScriptFrame()) {
        do { if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; } } while (0);
        if (IsDistanceBetweenThingsUnder(Me, Game.GetHero(), (float)DistanceToPlayAnim) && WaveTimer == 0) {
            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_055_RACE_TAUNT", false, Me, Game.GetHero());
            seh_me.PlayAnimation(Me.GetDataString(), false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
            while (seh_me.IsPerformingScriptTask()) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            }
            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            WaveTimer = 10;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            if (seh_me.IsValid())
                seh_me.Release();
            Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
            CTimer attack_timer;
            attack_timer = 20;
            while (attack_timer > 0) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                    || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                        && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    attack_timer = 20;
                }
            }
            Game.ClearThingBestEnemyTarget(Me);
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            }
            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            {
                float local_range = 0.0f;
                C3DVector local_position = HomePosition.GetPos();
                EScriptEntityMoveType local_movetype = ENTITY_MOVE_RUN;
                while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), local_position, local_range + 2.0)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    seh_me.MoveToPosition(local_position, local_range, local_movetype, false, true);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    }
                    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                }
                if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            }
            Game.EntitySetFacingAngle(Me, HomePosition.GetAngleXY(), false);
            Game.Pause(2.0f);
        }
    }
}