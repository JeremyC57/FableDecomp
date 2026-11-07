// NScript::CQ_MinionClifftopChaseScript::CMapKeeper::Main() -- the imprisoned map keeper (the archaeologist): live only while alive, play the CS_MINIONCHASE_PIER intro once and refresh the mission timer, take scripted control, switch off opinion reactions / combat / hero pushing, open the barriers quest, keep the NYMPH_CAGE_IDLE effect on him until two guarding minions are dead, follow the minion escort until it dies or the hero leaves it, wait for the hero within 15 units, fade the cage, close the barriers, make him pushable, heal and unkillable, flag MapKeeperSaidDialog, play CS_MINIONCHASE_OUTRO, remove him and flag MissionSucceeded
// Shape (chunk 820): the escort is a VALUE local and both IsAlive tests read it through a same-type `reinterpret_cast<const CScriptThing&>`, the
//   cast-only form of the GOTCHAS I-44 lever (no view struct): VC7.1 keeps the calls virtual (a plain value local devirtualises them) and hoists `lea ecx,[esp+0x1c]` above the loop-head terminate jne (0x00DA9771) as retail does. Fidelity: a compiler lever like I-44, probably not Lionhead's spelling (the chunk 797 park bound the temporary to a reference: 1 instruction off)
// Inline-split: the in-loop `ImprisonedFX = Game->CreateEffect(...)` sits in a single-site `do { } while (0)` so VC7.1 inlines that one implicit CScriptThing operator= as retail does (GOTCHAS 1891); the first site stays the out-of-line 0x008AB980 call
// Leaf: ?Main@CMapKeeper@CQ_MinionClifftopChaseScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA91A0, 2765 bytes, thiscall, plain `ret` (two epilogue copies), no EH frame; ends at 0x00DA9C6D.
// Identity (proven): slot 1 (Main) of the CMapKeeper entity vtable 0x012D73E0 stored by the landed CMapKeeper::Alloc 0x00DA4250 (its
//   Init 0x00DA4350 / OnPredicateFail 0x00DA4300 / GetParentScript 0x00DA42F0 are landed); Ego_r.pdb / FableWin.pdb name set
//   ?Main@CMapKeeper@CQ_MinionClifftopChaseScript@NScript@@EAEXXZ; "MapKeeper" is the binding name Main 0x00DA34B0 registers.
// Callees: CIsThingAlive (vtable 0x012C32F0, inline) + CScriptThing copy 0x004ABE90 / ~ 0x004AA840, SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CBaseObject 0x0099A380 (the first handle's inline ctor), ScriptedThingBase ctor 0x007E72A0 /
//   dtor 0x007E74D0 / operator= 0x008ABD10 / IsValid 0x00CD23B9 / Release 0x00CD2770, CScriptThing::operator= 0x008AB980, CCharString
//   0x0099EBF0 / ~ 0x0099EAE0, cast map ctor 0x00CDBF70 / operator[] 0x00CD3D2E / dtor 0x00CDBFB0, movie ctor 0x006E7B60 / dtor
//   0x006E7B80, RunCutsceneMacro 0x00CBFB7D, IsDistanceBetweenThingsOver 0x00CBE3EA / Under 0x00CBE2FF (__fastcall); pDSTGame
//   0x0143E8F8 (CTimer::GetTime inline); CGameScriptInterfaceBase slots 7, 8, 70 GetHero, 77 GetNearestWithScriptName, 90 GetTimer,
//   100 CreateEffect, 108 RemoveThing, 265 ModifyThingHealth, 276 ActivateQuest, 280 DeactivateQuest, 334 UpdateQuestInfoTimer,
//   370 StartMovieSequence, 371 FixMovieSequenceCamera, 379 PauseAllNonScriptedEntities, 465 EntityAttachToScript, 517
//   EntitySetAsKillable, 592 EntitySetCombatEnabled, 617 EntitySetOpinionReactionEnabled, 655 GetActiveQuestName, 844
//   SetIsPushableByHero; CScriptThing slots 6 GetPos, 75 IsAlive, 76 IsDead.
// Layout: engine/CQ_MinionClifftopChaseScript.h (opt-in FABLE_RETAIL_CQ_MINIONCLIFFTOPCHASESCRIPT_MAPKEEPER): DoneIntro +0x1c,
//   ImprisonedFX +0x20; ParentClass MapKeeperSaidDialog +0x48, MissionSucceeded +0x4a, KilledGuardingMinions +0x50, MissionTimer
//   +0x5c, MissionTimerID +0x60.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CQ_MINIONCLIFFTOPCHASESCRIPT_MAPKEEPER
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_MinionClifftopChaseScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
#include "engine/C3DVector.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_distance.h"
#include "stlport/fable_map.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
extern CGameScriptInterfaceBase* pDSTGame;                                                                   // 0x0143E8F8
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::map();                       // 0x00CDBF70
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::~map();                      // 0x00CDBFB0

void NScript::CQ_MinionClifftopChaseScript::CMapKeeper::Main()
{
    {
        CIsThingAlive alive(Me);
        SetPredicate(alive);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (!DoneIntro) {
        if (IsActiveThreadTerminating())
            return;
        if (seh_me.IsValid())
            seh_me.Release();
        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        if (seh_hero.IsValid())
            seh_hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_hero;
        mymap["ARCH"] = seh_me;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        RunCutsceneMacro("CS_MINIONCHASE_PIER", mymap, 0, 0, false, true);
        DoneIntro = true;
        Game->UpdateQuestInfoTimer(ParentClass.MissionTimerID, (float)ParentClass.MissionTimer.GetTime());
    }
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_MEDIUM)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetOpinionReactionEnabled(Me, (EOpinionReactionType)0x22, false);
    Game->EntitySetOpinionReactionEnabled(Me, (EOpinionReactionType)0x13, false);
    Game->EntitySetCombatEnabled(Me, false);
    Game->SetIsPushableByHero(Me, false);
    Game->ActivateQuest("Q_MinionClifftopChase_Barriers");
    if (ImprisonedFX.IsDead()) {
        if (IsActiveThreadTerminating())
            return;
        ImprisonedFX = Game->CreateEffect("NYMPH_CAGE_IDLE", Me.GetPos(), "", 0.0f, false, false);
        Game->EntityAttachToScript(ImprisonedFX, Game->GetActiveQuestName());
    }
    while (ParentClass.KilledGuardingMinions < 2) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (ImprisonedFX.IsDead()) {
            if (IsActiveThreadTerminating())
                return;
            do {
                ImprisonedFX = Game->CreateEffect("NYMPH_CAGE_IDLE", Me.GetPos(), "", 0.0f, false, false);
            } while (0);
            Game->EntityAttachToScript(ImprisonedFX, Game->GetActiveQuestName());
        }
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing escort = Game->GetNearestWithScriptName(Game->GetHero(), "MinionEscort");
    bool escort_gone = false;
    do {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!reinterpret_cast<const CScriptThing&>(escort).IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            escort = Game->GetNearestWithScriptName(Game->GetHero(), "MinionEscort");
            if (!reinterpret_cast<const CScriptThing&>(escort).IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                escort_gone = true;
            }
        }
        if (IsDistanceBetweenThingsOver(Game->GetHero(), escort, 15.0f)) {
            if (IsActiveThreadTerminating())
                return;
            escort = Game->GetNearestWithScriptName(Game->GetHero(), "MinionEscort");
            if (IsDistanceBetweenThingsOver(Game->GetHero(), escort, 15.0f)) {
                if (IsActiveThreadTerminating())
                    return;
                break;
            }
        }
    } while (!escort_gone);
    if (IsActiveThreadTerminating())
        return;
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(ImprisonedFX, false, true);
    Game->CreateEffect("NYMPH_CAGE_FADE", Me.GetPos(), "", 0.0f, false, false);
    Game->DeactivateQuest("Q_MinionClifftopChase_Barriers", 0);
    Game->SetIsPushableByHero(Me, true);
    Game->ModifyThingHealth(Me, 20.0f, false);
    Game->EntitySetAsKillable(Me, false, true);
    ParentClass.MapKeeperSaidDialog = true;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    {
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        if (seh_hero.IsValid())
            seh_hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_hero;
        mymap["ARCH"] = seh_me;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_MINIONCHASE_OUTRO", mymap, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
    }
    Game->RemoveThing(Me, false, true);
    ParentClass.MissionSucceeded = true;
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}
