// NScript::CV_MayorsInvitationScript::MonitorParanoidQuest() -- spawned watcher thread: activate the Paranoid Whispers quest, then wait frame by frame until the game reports it completed (or the thread is told to stop)
// Leaf: ?MonitorParanoidQuest@CV_MayorsInvitationScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EBCB60, 176 bytes (to its `ret` at 0x00EBCC0F), thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): the landed CV_MayorsInvitationScript::CLadyGrey::Main 0x00EB9980 builds the CSpawnedFunc whose
//   member-function pointer is this address (store at 0x00EBAFEE, `LG_SPAWN(MonitorParanoidQuest)`); Ego_r.pdb and FableWin.pdb both hold
//   ?MonitorParanoidQuest@CV_MayorsInvitationScript@NScript@@QAEXXZ (declared in engine/CV_MayorsInvitationScript.h). The row was outside
//   the catalog until chunk 818 trimmed CLadyGrey::Main's extent (it sat after that Main's int3 padding).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940;
//   CGameScriptInterface slots 7 NewScriptFrame, 25 MsgOnQuestCompleted, 276 ActivateQuest. Data: "V_ParanoidWhispers" 0x012C4CA0 (all
//   three string pushes).
// Shape: the wait is a top-tested `while (!MsgOnQuestCompleted(..))` that VC7.1 rotates (test duplicated at the bottom, `neg; sbb; inc`
//   for the negated bool held in bl across the temporary's dtor); the trailing `if (T) return;` compiles to the bare call before `ret`.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_MayorsInvitationScript.h"
#include "engine/CGameScriptInterfaceBase.h"

void NScript::CV_MayorsInvitationScript::MonitorParanoidQuest()
{
    Game->ActivateQuest("V_ParanoidWhispers");
    while (!Game->MsgOnQuestCompleted("V_ParanoidWhispers")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
}