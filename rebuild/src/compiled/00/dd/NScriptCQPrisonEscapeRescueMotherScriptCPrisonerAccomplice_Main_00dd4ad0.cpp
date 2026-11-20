// NScript::CQ_PrisonEscapeRescueMotherScript::CPrisonerAccomplice::Main() -- the hero's cell-mate in the prison: once the prison cutscene has finished, until released, whenever the hero talks to him play a paused speech scene (key stolen: KEY; race won: HINT2; more than one race run: HINT3; else HINT4_01 + HINT4_02, each line only while he is alive), and when the hero uses his cell door give the KEY line, free him (persistent, no information, morality reward); then follow the hero in the prisoner faction, commenting COURTYARD every 30 s and complaining when the hero hits him until the courtyard region is loaded, then stop region-following and comment GUARDS / complain until the thread ends
// Inline-budget: (1) scoped `#pragma inline_depth(0)` round the row-local CIsThingAlive ctor / dtor (with FABLE_RETAIL_CPREDICATEBASE_NOVTABLE; the predicate idiom of 0x00E25F70 / 0x00D0B3E0) -- retail expands the predicate ctor and dtor (vptr 0x012C32F0 in ebx) but CALLS their nested CScriptThing copy 0x004ABE90 (0x00DD4AF3) and ~CScriptThing 0x004AA840 (0x00DD4B0C), while it expands ~CScriptThing at the cutscene-wait exit 0x00DD5385; without the pragma c2 scores the nested ~CScriptThing 63 and the exit 65 (T=74, both expand), so no flag splits them (T30..60 give 3317..3365, S5 / S15 3531 / 3620); (2) FABLE_FORCEINLINE on the row-local CTimer::operator=(long) and operator==(long) -- retail expands SetTimer / GetTimer at all four sites (0x00DD4D08, 0x00DD54A2, 0x00DD565F, 0x00DD56D5); plain inline scores them 169 / 89 and calls them, and no T80..T170 / S5..S15 matches; (3) the out-of-line declaration of CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer (0x006E7AB0, the landed idiom of 139 rows) -- retail expands the movie dtor at the talk block's end and calls that release from it (0x00DD527F), while it calls the movie dtor COMDAT 0x006E7B80 at the exits; with the release inline the end site costs 60 and scores 199 (f=0.058; it stays above T even with every terminate test removed), with it out of line the end site expands and the exits stay calls.
// Ablation (each device removed in turn): no predicate pragma 3531v3467 (ctor-only pragma 3531, inline_depth(1) matches), no NOVTABLE 3483, plain operator= 3455, plain operator== 3473, movie release inline 3450; the draft's row-local ~CScriptThing definition is the twin's text (removing it gives 3414); IMPLICIT_COPY and the WIDESCREEN FORCEINLINE_CTOR opt-in are unneeded (dropped).
// Leaf: ?Main@CPrisonerAccomplice@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD4AD0, 3467 bytes, thiscall, plain `ret` (four epilogues), no EH frame.
// Identity: slot 1 (Main) of the CPrisonerAccomplice entity vtable 0x012DBEF0 (Alloc 0x00DD49A0 stores it; the script Main 0x00DD2CE0
//   binds "PrisonerAccomplice"); Ego_r.pdb / FableWin.pdb ?Main@CPrisonerAccomplice@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ.
//   Layout: engine/CQ_PrisonEscapeRescueMotherScript.h (new opt-in FABLE_RETAIL_CQ_PRISONESCAPERESCUEMOTHERSCRIPT_PRISONERACCOMPLICE:
//   Ego_r 32 bytes, ParentClass +0x14, PMasterData +0x18, ReleasedAndFollowing +0x1c).
// Source shape: Ego_d.pdb line table (lines 760..880) and locals (CellDoor, seh_me, TalkTimer, _Fail_Val; speech_movie and
//   PauseMyEntitiesPlease in the talk block; HLStyleConversationIndex in the door block and both hit blocks), in the generated text of
//   the TU's landed twin CPrisoner::Main 0x00DD5A60 (TRetVoid returns, Game as the original reference member, `TalkTimer == 0`,
//   SetEntityAsRegionFollowing(hero, Me, ...), the final `for (;; Game.NewScriptFrame())`) and the speech scene of the landed
//   0x00E25F70 (speech_movie, CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game), the health-gated Speak and its wait).
//   The drafts' `game` local and pointer ParentClass were not the original text.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTTHING_ORIGINAL_ACCESS
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CQ_PRISONESCAPERESCUEMOTHERSCRIPT_PRISONERACCOMPLICE
#include "fable_string.h"
#include "engine/EHeroAbility.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0 (out of line in retail)
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_PrisonEscapeRescueMotherScript.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
#include "engine/fable_script_def_global.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
extern CGameScriptInterfaceBase* pDSTGame;                       // 0x0143E8F8
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
FABLE_FORCEINLINE bool CTimer::operator==(long time) const { return GetTime()==time; }
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::MsgIsUsedBy(const CCharString& user) const { if (!PImp.Data) return false; return PImp.Data->MsgIsUsedBy(user); }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
#ifdef _MSC_VER
#pragma inline_depth()
#endif

void NScript::CQ_PrisonEscapeRescueMotherScript::CPrisonerAccomplice::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    CTimer TalkTimer;
    ParentClass.PrisonCutsceneFinished = false;
    while (!ParentClass.PrisonCutsceneFinished) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing CellDoor = Game.GetNearestWithScriptName(Me, "CellDoor");
    while (!ReleasedAndFollowing) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!ParentClass.PrisonCutsceneFinished) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (PMasterData->PrisonKeyStolenByHero) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_055_PRISON_PRISONER_KEY", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            } else if (PMasterData->PrisonRaceWonByHero) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_055_PRISON_PRISONER_HINT2", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            } else if (PMasterData->PrisonRaceNumber > 1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_055_PRISON_PRISONER_HINT3", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_055_PRISON_PRISONER_HINT4_01", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_055_PRISON_PRISONER_HINT4_02", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (CellDoor.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_055_PRISON_PRISONER_KEY", false, Me, Game.GetHero());
            Game.SetThingPersistent(Me, true);
            Game.ClearThingHasInformation(Me);
            Game.GiveHeroMorality(pGlobalScriptDef->PrisonFreePrisonerMorality);
            ReleasedAndFollowing = true;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (seh_me.IsValid()) seh_me.Release();
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
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_055_PRISON_PRISONER_ON_HIT", false, Me, Game.GetHero());
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}