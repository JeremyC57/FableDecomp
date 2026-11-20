// NScript::CQ_UndeadRisingScript::WatchForTermination() -- wait until the mission succeeded (+0x52) or failed (+0x53); on success
//   complete the active quest and hand over the Q_SecretPassage card, otherwise fail it; then deactivate the active quest
// Leaf: ?WatchForTermination@CQ_UndeadRisingScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E0C350, 342 bytes (padded to 352), plain `ret` (no handle parameter).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~ @ 0x0099EAE0, CWideString(const wchar_t*) @ 0x0099B6B0 / ~
//          @ 0x0099B510, CScriptBase::IsActiveThreadTerminating @ 0x00CB7940; CGameScriptInterface slots 7
//          NewScriptFrame (+0x1c), 281 DeactivateQuestLater (+0x464), 288 SetQuestAsCompleted (+0x480), 289
//          SetQuestAsFailed (+0x484), 295 GiveHeroQuestCardDirectly (+0x49c), 655 GetActiveQuestName (+0xa3c).
// Layout: engine/CQ_UndeadRisingScript.h: Game +0x40, MissionSucceeded +0x52, MissionFailed +0x53.
// Identity (Naming NF194, 2026-11-20, q6491): Ego_r.pdb + FableWin.pdb ?WatchForTermination@CQ_UndeadRisingScript@NScript@@QAEXXZ;
//   CQ_UndeadRisingScript::Main 0x00E0B160 spawns it under the literal "WatchForTermination" and binds that spelling. Was the
//   propagated label CV_LostTraderScript::WatchForQuestFailureState on local string / script models.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_UndeadRisingScript.h"

void NScript::CQ_UndeadRisingScript::WatchForTermination()
{
    while (!MissionFailed && !MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (MissionSucceeded) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_PRISON_SECRET_PASSAGE", "Q_SecretPassage", false);
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), false, L"", true);
    }
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
