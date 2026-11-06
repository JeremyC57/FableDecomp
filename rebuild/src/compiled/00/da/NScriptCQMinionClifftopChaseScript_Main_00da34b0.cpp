// NScript::CQ_MinionClifftopChaseScript::Main() -- the minion clifftop chase quest main: bind the twenty entity scripts (MapKeeper .. InvisibleCage), post the scripted entities, set the jail objective, wait for BowerstoneJail to load, override the music with the danger set and spawn ProcessMission / WatchForTermination
// Leaf: ?Main@CQ_MinionClifftopChaseScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA34B0, 2681 bytes, thiscall, plain `ret`, no EH frame, ends in its `ret` at 0x00DA3F28.
// Identity (proven): slot 2 (Main) of the quest-script vtable; Ego_r.pdb name set ?Main@CQ_MinionClifftopChaseScript@NScript@@UAEXXZ.
//   Every bound Alloc is a landed CQ_MinionClifftopChaseScript::C*::Alloc row (Ego_r.pdb ?Alloc@<Class>@CQ_MinionClifftopChaseScript@NScript@@SI...,
//   binding-name literal = class name less its `C`); the threads are this class's ProcessMission 0x00DA8510 and WatchForTermination
//   0x00DA3F40 (Ego_r.pdb QAEXXZ methods, named by their thread-name literals).
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::PostAddScriptedEntities 0x00CB8930, CScriptBase::IsActiveThreadTerminating
//   0x00CB7940, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CQ_MinionClifftopChaseScript> vtable 0x012D716C), CScriptBase::AddSpawnedFunction
//   0x00CB7E50; CGameScriptInterfaceBase slots 7 NewScriptFrame, 12 IsRegionLoaded, 64 PostAddScriptedEntities, 296 SetQuestCardObjective,
//   655 GetActiveQuestName (by value), 696 OverrideMusic.
// Shape: the landed quest-Main binding model (CQ_PrisonEscapeRescueMotherScript::Main 0x00DD2CE0) plus the WAIT_UNTIL region loop of
//   CQ_HeroSoulsNostroScript::Main 0x00D87CB0; the flag word (ebx) keeps the loop's negated bool in a frame byte.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_MinionClifftopChaseScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/EMusicSetType.h"

namespace NScript {
#define FABLE_MINION_ENTITY(Name) \
    class CQ_MinionClifftopChaseScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_MinionClifftopChaseScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_MINION_ENTITY(CMapKeeper)               // Alloc 0x00DA4250
FABLE_MINION_ENTITY(CMinionEscort)            // Alloc 0x00DA4490
FABLE_MINION_ENTITY(CMinionEscortEnd)         // Alloc 0x00DA4690
FABLE_MINION_ENTITY(CMinionWarningGuard)      // Alloc 0x00DA4920
FABLE_MINION_ENTITY(CBalverineFightingGuard)  // Alloc 0x00DA4B80
FABLE_MINION_ENTITY(CMagicBarrier)            // Alloc 0x00DA4D40
FABLE_MINION_ENTITY(CMagicBarrierGraveyard)   // Alloc 0x00DA53A0
FABLE_MINION_ENTITY(CMinionSpawner)           // Alloc 0x00DA58B0
FABLE_MINION_ENTITY(CMinionAttackingGuard)    // Alloc 0x00DA4A30
FABLE_MINION_ENTITY(CMinionFieldGenerator)    // Alloc 0x00DA5B40
FABLE_MINION_ENTITY(CCorneredGuard)           // Alloc 0x00DA5C90
FABLE_MINION_ENTITY(CMinionTrollSummoner)     // Alloc 0x00DA5DA0
FABLE_MINION_ENTITY(CRockTroll)               // Alloc 0x00DA5E70
FABLE_MINION_ENTITY(CMinionSpawnAdv)          // Alloc 0x00DA60A0
FABLE_MINION_ENTITY(CM_Minion)                // Alloc 0x00DA6170
FABLE_MINION_ENTITY(CM_Guard)                 // Alloc 0x00DA6320
FABLE_MINION_ENTITY(CGuardToLeadTheWay)       // Alloc 0x00DA6560
FABLE_MINION_ENTITY(CM_NotThisWay)            // Alloc 0x00DA6670
FABLE_MINION_ENTITY(CGuardNotThisWay)         // Alloc 0x00DA6810
FABLE_MINION_ENTITY(CInvisibleCage)           // Alloc 0x00DA57A0
#undef FABLE_MINION_ENTITY
}

void NScript::CQ_MinionClifftopChaseScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("MapKeeper", this, &CMapKeeper::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("MinionEscort", this, &CMinionEscort::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("MinionEscortEnd", this, &CMinionEscortEnd::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("MinionWarningGuard", this, &CMinionWarningGuard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("BalverineFightingGuard", this, &CBalverineFightingGuard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("MagicBarrier", this, &CMagicBarrier::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("MagicBarrierGraveyard", this, &CMagicBarrierGraveyard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("MinionSpawner", this, &CMinionSpawner::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("MinionAttackingGuard", this, &CMinionAttackingGuard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("MinionFieldGenerator", this, &CMinionFieldGenerator::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("CorneredGuard", this, &CCorneredGuard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("MinionTrollSummoner", this, &CMinionTrollSummoner::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("RockTroll", this, &CRockTroll::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("MinionSpawnAdv", this, &CMinionSpawnAdv::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("M_Minion", this, &CM_Minion::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("M_Guard", this, &CM_Guard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("GuardToLeadTheWay", this, &CGuardToLeadTheWay::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("M_NotThisWay", this, &CM_NotThisWay::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("GuardNotThisWay", this, &CGuardNotThisWay::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_MinionClifftopChaseScript>("InvisibleCage", this, &CInvisibleCage::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_MINION_CLIFFTOP_CHASE_OBJECTIVE_01", "BowerstoneJail", "BowerstoneSlums");
    while (!Game->IsRegionLoaded("BowerstoneJail")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->OverrideMusic(MUSIC_SET_DANGER, false, false);
    AddSpawnedFunction(new CSpawnedFunc<CQ_MinionClifftopChaseScript>("ProcessMission", 0, this, &CQ_MinionClifftopChaseScript::ProcessMission), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_MinionClifftopChaseScript>("WatchForTermination", 0, this, &CQ_MinionClifftopChaseScript::WatchForTermination), CCharString(""));
}