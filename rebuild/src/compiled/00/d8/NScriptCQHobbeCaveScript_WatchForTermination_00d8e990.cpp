// NScript::CQ_HobbeCaveScript::WatchForTermination() -- switch the cave creature generators off until the mission ends, switch them back on, fail the quest with the granny/grandson reason or complete it, take the hexagon door key from the hero, wait for Granny's House to unload and deactivate the quest
// Leaf: ?WatchForTermination@CQ_HobbeCaveScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D8E990, 1200 bytes (int3 padded).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, CWideString(const wchar_t*) 0x0099B6B0 / ~ 0x0099B510,
//   IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface slots 7, 11 IsLevelLoaded, 70 GetHero, 125 TakeObjectFromHero,
//   184 IsObjectInThingsPossession, 281 DeactivateQuestLater, 288 SetQuestAsCompleted, 289 SetQuestAsFailed,
//   558 SetCreatureGeneratorsEnabled, 655 GetActiveQuestName.
// Layout: Game +0x40, MissionSucceeded +0x4B, MissionFailed +0x4C, ReasonForFailure +0x70 (as CQ_HobbeCaveScript::Init 0x00D8D640).
// Shape: each failure reason is a whole SetQuestAsFailed call in its own arm (VC cross-jumps them into one tail).
// Naming (NF162, 2026-11-20, q6267): was a row-local global `struct CQ_HobbeCaveScript : CScriptBase` (leaf
//   ?WatchForTermination@CQ_HobbeCaveScript@@QAEXXZ) with local CCharString / CWideString / CEntityScriptBinding models; now on the
//   shared engine/CQ_HobbeCaveScript.h (NScript, CLevelScriptBase<CQ_SunnyvaleMasterData>; FailReason is the header's
//   ReasonForFailure +0x70) and fable_string.h. Identity: Ego_r.pdb ?WatchForTermination@CQ_HobbeCaveScript@NScript@@QAEXXZ, the
//   thread "WatchForTermination" that the landed Main 0x00D8DD10 spawns (it binds this spelling; its only referencer).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_HobbeCaveScript.h"

void NScript::CQ_HobbeCaveScript::WatchForTermination()
{
    Game->SetCreatureGeneratorsEnabled("HobbeCaveSideRoom", false);
    Game->SetCreatureGeneratorsEnabled("HobbeCaveNymphChamber", false);
    Game->SetCreatureGeneratorsEnabled("HobbeCaveMainChamber", false);
    Game->SetCreatureGeneratorsEnabled("HobbeCaveEntranceTunnel", false);
    Game->SetCreatureGeneratorsEnabled("HobbeCaveLarder", false);
    Game->SetCreatureGeneratorsEnabled("Greatwood", false);
    Game->SetCreatureGeneratorsEnabled("GreatwoodCaves", false);
    while (!MissionFailed && !MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetCreatureGeneratorsEnabled("HobbeCaveSideRoom", true);
    Game->SetCreatureGeneratorsEnabled("HobbeCaveNymphChamber", true);
    Game->SetCreatureGeneratorsEnabled("HobbeCaveMainChamber", true);
    Game->SetCreatureGeneratorsEnabled("HobbeCaveEntranceTunnel", true);
    Game->SetCreatureGeneratorsEnabled("HobbeCaveLarder", true);
    Game->SetCreatureGeneratorsEnabled("Greatwood", true);
    Game->SetCreatureGeneratorsEnabled("GreatwoodCaves", true);
    if (MissionFailed) {
        if (IsActiveThreadTerminating())
            return;
        if (ReasonForFailure == 0) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"TEXT_QST_036_QUEST_FAILED_GRANNY_DIED", true);
        } else if (ReasonForFailure == 1) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"TEXT_QST_036_QUEST_FAILED_GRANDSON_DIED", true);
        } else {
            if (IsActiveThreadTerminating())
                return;
            Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"", true);
        }
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, false);
    }
    bool hasKey = Game->IsObjectInThingsPossession("OBJECT_HEXAGON_DOOR_KEY", Game->GetHero());
    if (hasKey) {
        if (IsActiveThreadTerminating())
            return;
        Game->TakeObjectFromHero("OBJECT_HEXAGON_DOOR_KEY");
    }
    while (Game->IsLevelLoaded("GrannysHouse") != false) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}