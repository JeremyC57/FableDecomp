// CV_AmbushScamScript::WatchForQuestFinished() -- wait (one script frame at a time) until the quest is completed or finished; unless the thread is terminating or the quest finished, mark it completed
// Retail: Fable.exe @ 0x00D66E90, 73 bytes (+ int3 padding). Referenced once as a thread entry (0x00D66DA8).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (twice), CGameScriptInterface::NewScriptFrame
// (slot 7, +0x1c) through Game at +0x40.
// Layout: the common quest-script flags (Ego_r.pdb - 0x14): QuestFinished +0x48, QuestComplete +0x49. The
// owning class is not identified (the catalog label is shared by several script bodies).
// Shape: the script WAIT macro `while (!(QuestComplete || QuestFinished)) { NewScriptFrame(); if (terminating)
// return; }` (rotated: QuestComplete is tested at the entry and at the bottom, QuestFinished at the aligned loop
// head), then the macro's trailing terminate test.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* Game;                          // +0x40
    void* Owner;                                         // +0x44
};

struct CV_AmbushScamScript : CQuestScriptBase {
    bool QuestFinished;                                  // +0x48
    bool QuestComplete;                                  // +0x49
    void WatchForQuestFinished();
};

void CV_AmbushScamScript::WatchForQuestFinished()
{
    while (!(QuestComplete || QuestFinished)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!QuestFinished)
        QuestComplete = true;
}