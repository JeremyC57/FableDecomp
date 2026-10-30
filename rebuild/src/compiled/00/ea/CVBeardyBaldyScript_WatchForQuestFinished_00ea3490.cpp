// CV_BeardyBaldyScript::WatchForQuestFinished() -- wait (one script frame at a time) until the quest is finished or completed; unless the thread is terminating, set the flag at +0x4a
// Retail: Fable.exe @ 0x00EA3490, 66 bytes (+ int3 padding). Referenced once as a thread entry (0x00EA3389).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (twice), CGameScriptInterface::NewScriptFrame
// (slot 7, +0x1c) through Game at +0x40.
// Layout: the common quest-script flags (Ego_r.pdb - 0x14): QuestFinished +0x48, QuestComplete +0x49,
// QuestActivated +0x4a. Same macro as 0x00E50100 with two conditions.
// Shape: the WAIT macro `while (!(QuestFinished || QuestComplete)) { NewScriptFrame(); if (terminating) return; }`,
// the trailing terminate test, then the unconditional flag store.
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

struct CV_BeardyBaldyScript : CQuestScriptBase {
    bool QuestFinished;                                  // +0x48
    bool QuestComplete;                                  // +0x49
    bool QuestActivated;                                 // +0x4a
    void WatchForQuestFinished();
};

void CV_BeardyBaldyScript::WatchForQuestFinished()
{
    while (!(QuestFinished || QuestComplete)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    QuestActivated = true;
}