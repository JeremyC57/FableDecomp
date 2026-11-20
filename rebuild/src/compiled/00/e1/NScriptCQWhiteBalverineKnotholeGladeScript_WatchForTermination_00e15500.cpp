// NScript::CQ_WhiteBalverineKnotholeGladeScript::WatchForTermination() -- wait until the quest is won or lost; on failure fail the quest; then activate the Witchwood quest, set objective 6, put Knothole Glade's village out of limbo, wait for the white balverine to finish and complete the quest
// Leaf: ?WatchForTermination@CQ_WhiteBalverineKnotholeGladeScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E15500.
// Identity (two sources): Ego_r.pdb ?WatchForTermination@CQ_WhiteBalverineKnotholeGladeScript@NScript@@QAEXXZ (the class has no other spawned
//   method); the landed CQ_WhiteBalverineKnotholeGladeScript::Main 0x00E13F10 stores 0x00E15500 in the CSpawnedFunc<CQ_WhiteBalverineKnotholeGladeScript>
//   (vtable 0x012E077C, +0x34) it registers under the thread-name literal "WatchForTermination" and binds this spelling through the shared
//   header; the body waits for the PDB members MissionFailed +0x48 / MissionSucceeded +0x49 like every quest WatchForTermination.
//   Name-fix NF186 (q6421): was the descriptive Row00e15500::CQ_WhiteBalverineKnotholeGladeScript::HandleWhiteBalverineQuestObjective local model
//   (a 655-slot local interface; catalog _global::CScriptGameResourceObjectScriptedThingBase_HandleWhiteBalverineQuestObjective).
// Callees: IsActiveThreadTerminating 0x00CB7940; the quest interface (+0x40) slots 7 NewScriptFrame, 72 GetThingWithScriptName,
//   276 ActivateQuest, 281 DeactivateQuestLater, 288 SetQuestAsCompleted, 289 SetQuestAsFailed, 296 SetQuestCardObjective, 374 FadeScreenIn,
//   376 Pause, 440 SetVillageLimbo, 655 GetActiveQuestName; ~CScriptThing 0x004AA840, CCharString / CWideString temporaries.
// Layout: engine/CQ_WhiteBalverineKnotholeGladeScript.h (MissionFailed +0x48, MissionSucceeded +0x49); the master data (+0x44, a
//   CQ_SunnyvaleMasterData*) WhiteBalverineFinished +0xe1 (engine/CQ_SunnyvaleMasterData.h; the local model's parent->ready).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_WhiteBalverineKnotholeGladeScript.h"
namespace NScript {
void CQ_WhiteBalverineKnotholeGladeScript::WatchForTermination()
{
    while (!MissionFailed && !MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (MissionFailed) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), false, L"", true);
    }
    Game->ActivateQuest("Q_WhiteBalverineWW");
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_06", "Witchwood4", "KnotholeGlade");
    CScriptThing village = Game->GetThingWithScriptName("VILLAGE_KNOTHOLEGLADE");
    Game->SetVillageLimbo(village, false);
    while (!MasterScriptData->WhiteBalverineFinished) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
    Game->Pause(1.0f);
    Game->FadeScreenIn();
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
}
