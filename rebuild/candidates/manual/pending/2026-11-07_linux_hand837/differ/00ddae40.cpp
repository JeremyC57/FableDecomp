// NScript::CQ_PrisonRaceScript::Main() -- the prison race quest main: bind the six race entity scripts, post them, clear the race-won flag, teleport the hero to the "PrisonRaceStartHSP" marker (its position and heading), wait for the prison courtyard region, then spawn the WatchForHeroWinOrLose thread
// Grade: DIFFER(1180v1180) -- x87 store scheduling (2 lines): after the expanded StartPos.GetAngleXY() virtual call ours stores `fstp [esp+0x18]` right after `call [edx+0x28]`, retail after the following GetPos inline's `mov ecx,[esp+0x20]; cmp ecx,ebp`; everything else exact
// Next idea: 17 grades on the GetAngleXY / GetPos inline spellings (plain, ternary, reversed, named float, __forceinline either or both, the chunk 783 forceinline-GetPos model) and argument locals all keep the fstp beside the call; try a GetAngleXY that returns through a by-value float member of a local struct, or compiling the TU-mate WatchForHeroWinOrLose 0x00DDB2F0 body above Main (x87 scheduling can follow TU state)
// Leaf: ?Main@CQ_PrisonRaceScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DDAE40, 1180 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012DC88C (the landed RegisterMain 0x00DDADA0 spawns it); Ego_r.pdb
//   name set ?Main@CQ_PrisonRaceScript@NScript@@UAEXXZ. Each bound Alloc is a landed CQ_PrisonRaceScript::C*::Alloc row (binding-name
//   literal = class name less its `C`); the thread is WatchForHeroWinOrLose 0x00DDB2F0 (Ego_r.pdb
//   ?WatchForHeroWinOrLose@CQ_PrisonRaceScript@NScript@@QAEXXZ, named by its thread-name literal).
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::PostAddScriptedEntities 0x00CB8930,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CQ_PrisonRaceScript> vtable
//   0x012DC8A0), CScriptBase::AddSpawnedFunction 0x00CB7E50, ~CScriptThing 0x004AA840 (natural exit; expanded at the wait-loop
//   return: vtable 0x01238C8C, PImp release, ~CBaseClass 0x0099A2E0); CGameScriptInterfaceBase slots 7 NewScriptFrame,
//   12 IsRegionLoaded, 64 PostAddScriptedEntities, 70 GetHero, 72 GetThingWithScriptName, 473 EntityTeleportToPosition;
//   CScriptThing slots 6 GetPos and 10 GetAngleXY expanded (NullCoord 0x0143E8E0).
// Data: CQ_SunnyvaleMasterData::PrisonRaceWonByHero +0xec cleared.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CQ_PRISONRACESCRIPT_MAIN
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_PrisonRaceScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"

namespace NScript {
#define FABLE_PR_ENTITY(Name) \
    class CQ_PrisonRaceScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_PrisonRaceScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_PR_ENTITY(CPrisonRaceWinningPostGuard)    // Alloc 0x00DDBF90
FABLE_PR_ENTITY(CPrisonRaceWarden)              // Alloc 0x00DDC2D0
FABLE_PR_ENTITY(CPrisonRacer)                   // Alloc 0x00DDC3E0
FABLE_PR_ENTITY(CPrisonRaceStartingGuard)       // Alloc 0x00DDC500
FABLE_PR_ENTITY(CWaypointSpeedCheck)            // Alloc 0x00DDC5E0
#undef FABLE_PR_ENTITY
}

inline CScriptThing::~CScriptThing() { }
inline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
inline float CScriptThing::GetAngleXY() const { if (!PImp.Data) return 0.0f; return PImp.Data->GetAngleXY(); }

void NScript::CQ_PrisonRaceScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonRaceScript>("PrisonRaceGuard", this, &CPrisonRaceGuard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonRaceScript>("PrisonRaceWinningPostGuard", this, &CPrisonRaceWinningPostGuard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonRaceScript>("PrisonRaceWarden", this, &CPrisonRaceWarden::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonRaceScript>("PrisonRacer", this, &CPrisonRacer::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonRaceScript>("PrisonRaceStartingGuard", this, &CPrisonRaceStartingGuard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonRaceScript>("WaypointSpeedCheck", this, &CWaypointSpeedCheck::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    MasterScriptData->PrisonRaceWonByHero = false;
    CScriptThing StartPos = Game->GetThingWithScriptName("PrisonRaceStartHSP");
    Game->EntityTeleportToPosition(Game->GetHero(), StartPos.GetPos(), StartPos.GetAngleXY(), false, false);
    while (!Game->IsRegionLoaded("PrisonCourtyard")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddSpawnedFunction(new CSpawnedFunc<CQ_PrisonRaceScript>("WatchForHeroWinOrLose", 0, this, &CQ_PrisonRaceScript::WatchForHeroWinOrLose), CCharString(""));
}
