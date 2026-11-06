// NScript::CQ_RansomVictimScript::Main() -- the ransom victim quest main: bind the ten entity scripts (VictimsFather .. NPCComment), post the scripted entities, spawn WatchForTermination / WatchForNewRegion / DisappearFollowers, wait for Knothole Glade, set up the gossip, open the KG gate, then walk the three objectives (activating the Witchwood sub-quests and disabling the Witchwood creature generators) and hand the active enemy name to RansomRevenge
// Leaf: ?Main@CQ_RansomVictimScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DE0850, 2606 bytes, thiscall, plain `ret`, no EH frame, ends in its `ret` at 0x00DE127D.
// Identity (proven): slot 2 (Main) of the quest-script vtable; Ego_r.pdb name set ?Main@CQ_RansomVictimScript@NScript@@UAEXXZ. Every bound
//   Alloc is a landed CQ_RansomVictimScript::C*::Alloc row (binding-name literal = class name less its `C`; NPCComment is the one flag-0
//   binding). The threads are this class's WatchForTermination 0x00DE14A0 (landed), WatchForNewRegion 0x00DE1770 and DisappearFollowers
//   0x00DE4710 (Ego_r.pdb QAEXXZ methods named by their thread-name literals); GossipSetup 0x00DE1280 is the landed row.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0 / operator=(const char*)
//   0x0099EFE0, ~CScriptThing 0x004AA840, CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::PostAddScriptedEntities 0x00CB8930,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CQ_RansomVictimScript> vtable 0x012DCF1C),
//   CScriptBase::AddSpawnedFunction 0x00CB7E50; CGameScriptInterfaceBase slots 7 NewScriptFrame, 12 IsRegionLoaded, 64 PostAddScriptedEntities,
//   72 GetThingWithScriptName (by value), 276 ActivateQuest, 280 DeactivateQuest, 296 SetQuestCardObjective, 425 OpenDoor,
//   558 SetCreatureGeneratorsEnabled, 655 GetActiveQuestName (by value).
// Layout (engine/CQ_RansomVictimScript.h, retail offsets): QuestStartScreened +0x48, RescuedVictim +0x4b, RansommerDied +0x4c,
//   ActiveEnemyName +0x64.
// Shape: the landed quest-Main binding model (CQ_PrisonEscapeRescueMotherScript::Main 0x00DD2CE0) and the WAIT_UNTIL loops of
//   CQ_HeroSoulsNostroScript::Main 0x00D87CB0 (loop, then a trailing terminate check). The gate thing lives to the end of the function:
//   every return after GetThingWithScriptName runs its destructor.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CQ_RANSOMVICTIMSCRIPT_METHODS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_RansomVictimScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"

void NScript::CQ_RansomVictimScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_RansomVictimScript>("VictimsFather", this, &CVictimsFather::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_RansomVictimScript>("VictimsBrother", this, &CVictimsBrother::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_RansomVictimScript>("Victim", this, &CVictim::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_RansomVictimScript>("Ransommer", this, &CRansommer::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_RansomVictimScript>("RansomHelper", this, &CRansomHelper::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_RansomVictimScript>("VictimsBrotherHelper", this, &CVictimsBrotherHelper::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_RansomVictimScript>("RandomAttacker", this, &CRandomAttacker::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_RansomVictimScript>("VictimsFatherGuard", this, &CVictimsFatherGuard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_RansomVictimScript>("RansomRevenge", this, &CRansomRevenge::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_RansomVictimScript>("NPCComment", this, &CNPCComment::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CQ_RansomVictimScript>("WatchForTermination", 0, this, &CQ_RansomVictimScript::WatchForTermination), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_RansomVictimScript>("WatchForNewRegion", 0, this, &CQ_RansomVictimScript::WatchForNewRegion), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_RansomVictimScript>("DisappearFollowers", 0, this, &CQ_RansomVictimScript::DisappearFollowers), CCharString(""));
    while (!Game->IsRegionLoaded("KnotholeGlade")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GossipSetup();
    CScriptThing gate = Game->GetThingWithScriptName("KG_Gate");     // descriptive local
    Game->OpenDoor(gate);
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_RANSOM_VICTIM_B_OBJECTIVE_01", "KnotholeGlade", "KnotholeGlade");
    while (!QuestStartScreened) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->ActivateQuest("Q_RansomVictimStarted");
    Game->ActivateQuest("Q_RansomVictimStartedNature");
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_RANSOM_VICTIM_B_OBJECTIVE_02", "WitchwoodCavern", "Witchwood1");
    while (!RescuedVictim) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->DeactivateQuest("Q_RansomVictimStartedNature", 0);
    Game->SetCreatureGeneratorsEnabled("Witchwood2", false);
    Game->SetCreatureGeneratorsEnabled("Witchwood3", false);
    Game->SetCreatureGeneratorsEnabled("Witchwood4", false);
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_RANSOM_VICTIM_B_OBJECTIVE_03", "KnotholeGlade", "KnotholeGlade");
    if (!RansommerDied) {
        if (IsActiveThreadTerminating())
            return;
        Game->ActivateQuest("Q_RansomVictimNaturesRevenge");
    }
    while (!RansommerDied) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->ActivateQuest("Q_RansomVictimRevenge");
    Game->DeactivateQuest("Q_RansomVictimNaturesRevenge", 0);
    ActiveEnemyName = "RansomRevenge";
}