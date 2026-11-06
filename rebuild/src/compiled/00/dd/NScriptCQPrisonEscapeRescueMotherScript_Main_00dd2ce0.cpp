// NScript::CQ_PrisonEscapeRescueMotherScript::Main() -- the prison escape quest main: switch teleporting off, set the courtyard objective, bind the fifteen entity scripts (PrisonGuard .. PrisonKeyDoor), post the scripted entities, add logbook story entry 220 and spawn WatchHeroEscapeStatus / SplittingUpCutscene / PrisonLifeIntro / ProcessRaces
// Leaf: ?Main@CQ_PrisonEscapeRescueMotherScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD2CE0, 2265 bytes, thiscall, plain `ret`, no EH frame, ends in its `ret` at 0x00DD35B8.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012DB86C (engine/CQ_PrisonEscapeRescueMotherScript.h); Ego_r.pdb /
//   FableWin.pdb name set ?Main@CQ_PrisonEscapeRescueMotherScript@NScript@@UAEXXZ; every bound Alloc is a landed
//   CQ_PrisonEscapeRescueMotherScript::C*::Alloc row (CPrisonGuard 0x00DD43D0, CPrisonerAccomplice 0x00DD49A0, CPrisoner 0x00DD5940,
//   CPrisonEscapeGuard 0x00DD6330, CMotherTorture 0x00DD68A0, CGuardMotherEscape 0x00DD83C0, CInventoryChest 0x00DD8790,
//   CTortureDoorPassage 0x00DD88D0, CTortureDoorCourtyard 0x00DD8AE0, CCellDoorHero 0x00DD8D20, CCellDoor 0x00DD8F10,
//   CPrisonKeyDoor 0x00DD90C0 for the four key doors) and the four threads are this class's WatchHeroEscapeStatus 0x00DD35D0
//   (landed), SplittingUpCutscene 0x00DD9520, PrisonLifeIntro 0x00DD3B80 (landed, chunk 785), ProcessRaces 0x00DD9900, named by
//   their thread-name literals.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230 (binding vtable 0x012DBAFC), CScriptBase::PostAddScriptedEntities 0x00CB8930,
//   AddLogbookStoryEntry(long) 0x00CBE87F (__fastcall, Ego_r ?AddLogbookStoryEntry@@YIXJ@Z), CSpawnedFuncBase 0x00CDD450
//   (CSpawnedFunc<CQ_PrisonEscapeRescueMotherScript> vtable 0x012DB954), CScriptBase::AddSpawnedFunction 0x00CB7E50;
//   CGameScriptInterfaceBase slots 64 PostAddScriptedEntities (+0x100), 296 SetQuestCardObjective (+0x4a0), 565
//   SetTeleportingAsActive (+0x8d4), 655 GetActiveQuestName (+0xa3c, by value).
// Shape: the landed quest-Main model (CQ_TraderEscortScript::Main 0x00E00DB0): one flag word (ebx, memory home zeroed at entry)
//   guards each name temporary that exists only when its operator new succeeded; binding flags 0 / 1 / 0 as retail stores them.
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
#include "engine/CQ_PrisonEscapeRescueMotherScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"

void FABLE_FASTCALL AddLogbookStoryEntry(long entry);   // 0x00CBE87F

namespace NScript {
#define FABLE_PRISON_ENTITY(Name) \
    class CQ_PrisonEscapeRescueMotherScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_PrisonEscapeRescueMotherScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_PRISON_ENTITY(CPrisonGuard)            // Alloc 0x00DD43D0
FABLE_PRISON_ENTITY(CPrisonerAccomplice)     // Alloc 0x00DD49A0
FABLE_PRISON_ENTITY(CPrisoner)               // Alloc 0x00DD5940
FABLE_PRISON_ENTITY(CPrisonEscapeGuard)      // Alloc 0x00DD6330
FABLE_PRISON_ENTITY(CMotherTorture)          // Alloc 0x00DD68A0
FABLE_PRISON_ENTITY(CGuardMotherEscape)      // Alloc 0x00DD83C0
FABLE_PRISON_ENTITY(CInventoryChest)         // Alloc 0x00DD8790
FABLE_PRISON_ENTITY(CTortureDoorPassage)     // Alloc 0x00DD88D0
FABLE_PRISON_ENTITY(CTortureDoorCourtyard)   // Alloc 0x00DD8AE0
FABLE_PRISON_ENTITY(CCellDoorHero)           // Alloc 0x00DD8D20
FABLE_PRISON_ENTITY(CCellDoor)               // Alloc 0x00DD8F10
FABLE_PRISON_ENTITY(CPrisonKeyDoor)          // Alloc 0x00DD90C0
#undef FABLE_PRISON_ENTITY
}

void NScript::CQ_PrisonEscapeRescueMotherScript::Main()
{
    Game->SetTeleportingAsActive(false);
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_02", "PrisonCourtyard", "");
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonEscapeRescueMotherScript>("PrisonGuard", this, &CPrisonGuard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonEscapeRescueMotherScript>("PrisonerAccomplice", this, &CPrisonerAccomplice::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonEscapeRescueMotherScript>("Prisoner", this, &CPrisoner::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonEscapeRescueMotherScript>("PrisonEscapeGuard", this, &CPrisonEscapeGuard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonEscapeRescueMotherScript>("MotherTorture", this, &CMotherTorture::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonEscapeRescueMotherScript>("GuardMotherEscape", this, &CGuardMotherEscape::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonEscapeRescueMotherScript>("InventoryChest", this, &CInventoryChest::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonEscapeRescueMotherScript>("TortureDoorPassage", this, &CTortureDoorPassage::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonEscapeRescueMotherScript>("TortureDoorCourtyard", this, &CTortureDoorCourtyard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonEscapeRescueMotherScript>("CellDoorHero", this, &CCellDoorHero::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonEscapeRescueMotherScript>("CellDoor", this, &CCellDoor::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonEscapeRescueMotherScript>("StoreRoomDoor", this, &CPrisonKeyDoor::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonEscapeRescueMotherScript>("TortureChamberDoor", this, &CPrisonKeyDoor::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonEscapeRescueMotherScript>("BarracksDoor", this, &CPrisonKeyDoor::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonEscapeRescueMotherScript>("PrisonKeyDoor", this, &CPrisonKeyDoor::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddLogbookStoryEntry(220);
    AddSpawnedFunction(new CSpawnedFunc<CQ_PrisonEscapeRescueMotherScript>("WatchHeroEscapeStatus", 0, this, &CQ_PrisonEscapeRescueMotherScript::WatchHeroEscapeStatus), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_PrisonEscapeRescueMotherScript>("SplittingUpCutscene", 0, this, &CQ_PrisonEscapeRescueMotherScript::SplittingUpCutscene), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_PrisonEscapeRescueMotherScript>("PrisonLifeIntro", 0, this, &CQ_PrisonEscapeRescueMotherScript::PrisonLifeIntro), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_PrisonEscapeRescueMotherScript>("ProcessRaces", 0, this, &CQ_PrisonEscapeRescueMotherScript::ProcessRaces), CCharString(""));
}