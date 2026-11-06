// NScript::CQ_AwakeningTheOracleScript::Main() -- the awakening-the-oracle quest main: bind the eleven entity scripts (OracleTablet .. PretendGuard), post them, switch the Northern Wastes 3 generators off and set the first objective; unless the opening cut-scene has played, wait for Northern Wastes 3 (autosaving once), point at Snowspire, wait for it, play CS_SNOWSPIRE_INTRO and fade in; then re-enable the generators, set the second objective, spawn WatchForAliveTablets, switch Northern Wastes 2 off, play the necropolis intro (with the quest start screen) once, wait for all four tablets, complete the quest and deactivate it
// Leaf: ?Main@CQ_AwakeningTheOracleScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CFB080, 2699 bytes, thiscall, plain `ret`, no EH frame, ends in its `ret` at 0x00CFBB0A.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012C78B0 (the landed ctor 0x00CFAD30 stores it); Ego_r.pdb name set
//   ?Main@CQ_AwakeningTheOracleScript@NScript@@UAEXXZ. Every bound Alloc is a landed CQ_AwakeningTheOracleScript::C*::Alloc row (binding-name
//   literal = class name less its `C`; PretendGuard is the one flag-1 binding); the thread is this class's WatchForAliveTablets 0x00CFF110
//   (Ego_r.pdb QAEXXZ method named by its thread-name literal); PlayCutscene(CCharString, bool) 0x00CFF9A0 is the landed row.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::PostAddScriptedEntities 0x00CB8930, CScriptBase::IsActiveThreadTerminating
//   0x00CB7940, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CQ_AwakeningTheOracleScript> vtable 0x012C78EC), CScriptBase::AddSpawnedFunction
//   0x00CB7E50; CGameScriptInterfaceBase slots 7 NewScriptFrame, 12 IsRegionLoaded, 64 PostAddScriptedEntities, 281 DeactivateQuestLater,
//   288 SetQuestAsCompleted, 296 SetQuestCardObjective, 309 KickOffQuestStartScreen, 374 FadeScreenIn, 558 SetCreatureGeneratorsEnabled,
//   655 GetActiveQuestName (by value), 705 AutoSave.
// Layout (engine/CQ_AwakeningTheOracleScript.h): PlayedOpeningCS +0x49, PlayedNecroCS +0x4b, TabletsCollected +0x4c, AutosavedInNW3 +0x6c.
// Shape: the landed quest-Main binding model (CQ_PrisonEscapeRescueMotherScript::Main 0x00DD2CE0) with WAIT_UNTIL loops (loop, then a
//   trailing terminate check); ebx holds 1 for the first binding's flag byte, ebp the zero.
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
#include "engine/CQ_AwakeningTheOracleScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"

namespace NScript {
#define FABLE_ORACLE_ENTITY(Name) \
    class CQ_AwakeningTheOracleScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_AwakeningTheOracleScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_ORACLE_ENTITY(COracleTablet)                // Alloc 0x00CFBB20
FABLE_ORACLE_ENTITY(CFinalTabletSarcophagus)      // Alloc 0x00CFBC00
FABLE_ORACLE_ENTITY(CNecropolisGhost)             // Alloc 0x00CFBCE0
FABLE_ORACLE_ENTITY(CNecropolisGate)              // Alloc 0x00CFC700
FABLE_ORACLE_ENTITY(CLostBayCutsceneTrigger)      // Alloc 0x00CFC7D0
FABLE_ORACLE_ENTITY(CChunkCollectionIndicator)    // Alloc 0x00CFC8A0
FABLE_ORACLE_ENTITY(CNecropolisEnemy)             // Alloc 0x00CFCA60
FABLE_ORACLE_ENTITY(CScytheBeforeGettingTablets)  // Alloc 0x00CFCDC0
FABLE_ORACLE_ENTITY(CATO_MagicBarrier)            // Alloc 0x00CFD5D0
FABLE_ORACLE_ENTITY(CFinalSummoner)               // Alloc 0x00CFDCA0
FABLE_ORACLE_ENTITY(CPretendGuard)                // Alloc 0x00CFCBD0
#undef FABLE_ORACLE_ENTITY
}

void NScript::CQ_AwakeningTheOracleScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AwakeningTheOracleScript>("OracleTablet", this, &COracleTablet::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AwakeningTheOracleScript>("FinalTabletSarcophagus", this, &CFinalTabletSarcophagus::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AwakeningTheOracleScript>("NecropolisGhost", this, &CNecropolisGhost::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AwakeningTheOracleScript>("NecropolisGate", this, &CNecropolisGate::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AwakeningTheOracleScript>("LostBayCutsceneTrigger", this, &CLostBayCutsceneTrigger::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AwakeningTheOracleScript>("ChunkCollectionIndicator", this, &CChunkCollectionIndicator::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AwakeningTheOracleScript>("NecropolisEnemy", this, &CNecropolisEnemy::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AwakeningTheOracleScript>("ScytheBeforeGettingTablets", this, &CScytheBeforeGettingTablets::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AwakeningTheOracleScript>("ATO_MagicBarrier", this, &CATO_MagicBarrier::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AwakeningTheOracleScript>("FinalSummoner", this, &CFinalSummoner::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AwakeningTheOracleScript>("PretendGuard", this, &CPretendGuard::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->SetCreatureGeneratorsEnabled("NorthernWastes3", false);
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_AWAKEN_ORACLE_OBJECTIVE_01", "SnowspireVillage", "LostBay");
    if (!PlayedOpeningCS) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsRegionLoaded("NorthernWastes3")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (!AutosavedInNW3) {
            if (IsActiveThreadTerminating())
                return;
            AutosavedInNW3 = true;
            Game->AutoSave();
        }
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_AWAKEN_ORACLE_OBJECTIVE_01", "SnowspireVillage", "NorthernWastes3");
        while (!Game->IsRegionLoaded("SnowspireVillage")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        PlayCutscene("CS_SNOWSPIRE_INTRO", true);
        PlayedOpeningCS = true;
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_AWAKEN_ORACLE_OBJECTIVE_02", "NorthernWastes2", "NorthernWastes3");
        Game->FadeScreenIn();
    }
    Game->SetCreatureGeneratorsEnabled("NorthernWastes3", true);
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_AWAKEN_ORACLE_OBJECTIVE_02", "NorthernWastes2", "NorthernWastes3");
    AddSpawnedFunction(new CSpawnedFunc<CQ_AwakeningTheOracleScript>("WatchForAliveTablets", 0, this, &CQ_AwakeningTheOracleScript::WatchForAliveTablets), CCharString(""));
    Game->SetCreatureGeneratorsEnabled("NorthernWastes2", false);
    if (!PlayedNecroCS) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsRegionLoaded("NorthernWastes2")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        PlayCutscene("CS_ORACLE_NECROPOLIS_INTRO", true);
        PlayedNecroCS = true;
        Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
        Game->FadeScreenIn();
    }
    while (TabletsCollected != 4) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    Game->FadeScreenIn();
}