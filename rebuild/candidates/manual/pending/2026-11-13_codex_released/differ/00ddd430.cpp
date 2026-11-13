// NScript::CQ_PrisonRaceScript::CPrisonRaceStartingGuard::Main() -- the guard who starts the prison race: take scripted control of Me, stop it looking at anything and sheathe its weapons; take the hero, cast HERO / GUARD and play CS_PRISON_RACE_INTRO_SHORT (a repeat race) or CS_PRISON_RACE_INTRO under a paused movie with a fixed camera, then CS_PRISON_RACE_INTRO_END, and mark the race started; with the race-start music override, each frame: when the hero hits Me (not with a heal-life spell) release Me, make the hero its best enemy for 20 frames (renewed by every further hit), clear the target, retake control and walk back home
// Grade: DIFFER(2112v2044) -- per-exit ~CTimer split: with the inline ~CTimer the timer index stays in ebp as in retail, but VC7.1 expands DeregisterTimer at the three in-loop exits (timer wait, StartScripting retry, IsPerformingScriptTask wait) where retail calls 0x00CD4470 at one shared pad; the cast-block zero register (ebx v xor ebp) and the expanded STB operator= spill slots also differ (102 lines)
// Next idea: an out-of-line ~CTimer gives retail's single pad but spills TimerIndex and aligns the timer-loop head (e2, md108); a spelling that keeps the inline dtor cold at the three loop exits
// Leaf: ?Main@CPrisonRaceStartingGuard@CQ_PrisonRaceScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DDD430, 2044 bytes, thiscall, plain `ret` (five epilogue copies), frame 0x5c.
// Identity (proven): Ego_r.pdb + FableWin.pdb ?Main@CPrisonRaceStartingGuard@CQ_PrisonRaceScript@NScript@@EAEXXZ; the single .rdata
//   reference is slot 1 of the entity vtable 0x012DCA48 (slot 0 scalar dtor 0x00DDC5C0, slot 2 Init 0x00DDC5B0, slot 3 GetParentScript
//   0x00DDC5A0, the class's landed labels).
// Callees: CBaseObject 0x0099A380 / ~ 0x0099A430 / operator= 0x0099A3B0, ScriptedThingBase IsValid 0x00CD23B9 / Release 0x00CD2770 /
//   dtor 0x007E74D0 / GetScriptThing 0x007E7490 / MoveToPosition 0x007E72F0 / IsPerformingScriptTask 0x007E7450, IsActiveThreadTerminating
//   0x00F35B30, malloc 0x00BFEA0E (STLport map node allocator), std::map<CCharString, STB>::operator[] 0x00CD3D2E / ~map 0x00CDBFB0, RunCutsceneMacro 0x00CBFB7D,
//   the MovieBase PImp dtor CCountedPointer<MovieBase>::~ 0x006E7AB0, operator delete 0x00BFE9BC, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   ~CScriptThing 0x004AA840, ~CTimer 0x00CD4470 (ctor / operator= / GetTime expanded over pDSTGame 0x0143E8F8),
//   IsDistanceFromThingToPositionOver 0x00CBE45C; CScriptThing slots 7 GetHomePos, 21 MsgIsHitBy, 41 / 42; CGameScriptInterfaceBase
//   slots 7, 8, 70 GetHero, 370 StartMovieSequence, 371 FixMovieSequenceCamera, 379 PauseAllNonScriptedEntities, 451 / 452, 500, 506, 696.
// Layout: engine/CQ_PrisonRaceScript.h (opt-in FABLE_RETAIL_CQ_PRISONRACESCRIPT_STARTINGGUARD): ParentClass +0x14 (RaceStarted +0x49),
//   PMasterData +0x18 (CQ_SunnyvaleMasterData::PrisonRaceNumber +0xe8).
// Inline-split: one `do { } while (0);` round the two cast-map assignments heats that single block so VC7.1 expands both implicit
//   CScriptGameResourceObjectScriptedThingBase::operator= copies (CBaseObject::operator= 0x0099A3B0 + the inline CCountedPointer share)
//   as retail does (the landed CKG_Chief::Main 0x00E19D80 lever, chunk 833); without it both are called (e1, 137 lines).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CQ_PRISONRACESCRIPT_STARTINGGUARD
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();                        // 0x006E7AB0
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_PrisonRaceScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/C3DVector.h"
#include "engine/fable_script_cutscene.h"
#include "stlport/fable_map.h"
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::~map();                      // 0x00CDBFB0
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing&, const C3DVector&, float);            // 0x00CBE45C
extern CGameScriptInterfaceBase* pDSTGame;                                                                   // 0x0143E8F8
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

void NScript::CQ_PrisonRaceScript::CPrisonRaceStartingGuard::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntityForceToLookAtNothing(Me);
    Game->EntitySheatheWeapons(Me, false);
    {
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
        do {
            cast["HERO"] = seh_hero;
            cast["GUARD"] = seh_me;
        } while (0);
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        if (PMasterData->PrisonRaceNumber >= 1) {
            if (IsActiveThreadTerminating())
                return;
            RunCutsceneMacro("CS_PRISON_RACE_INTRO_SHORT", cast, 0, 0, false, true);
        }
        else {
            if (IsActiveThreadTerminating())
                return;
            RunCutsceneMacro("CS_PRISON_RACE_INTRO", cast, 0, 0, false, true);
        }
        Game->FixMovieSequenceCamera(false);
        RunCutsceneMacro("CS_PRISON_RACE_INTRO_END", cast, 0, 0, false, false);
        ParentClass.RaceStarted = true;
    }
    Game->OverrideMusic((EMusicSetType)0x22, false, false);
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
            (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            if (seh_me.IsValid())
                seh_me.Release();
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
            CTimer timer;
            timer = 20;
            while (timer.GetTime() > 0) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
                    (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                    if (IsActiveThreadTerminating())
                        return;
                    timer = 20;
                }
            }
            Game->ClearThingBestEnemyTarget(Me);
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            C3DVector home = Me.GetHomePos();
            while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), home, 2.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                seh_me.MoveToPosition(home, 0.0f, (EScriptEntityMoveType)1, false, true);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->NewScriptFrame();
    }
}
