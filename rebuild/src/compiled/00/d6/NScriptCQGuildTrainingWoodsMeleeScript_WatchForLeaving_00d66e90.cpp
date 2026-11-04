// NScript::CQ_GuildTrainingWoodsMeleeScript::WatchForLeaving() -- wait (one script frame at a time) until the mission has failed or succeeded; unless the thread is terminating or the mission succeeded, mark it failed
// Leaf: ?WatchForLeaving@CQ_GuildTrainingWoodsMeleeScript@NScript@@QAEXXZ
// Retail: Fable.exe @ 0x00D66E90, 73 bytes (+ int3 padding). Referenced once as a thread entry (0x00D66DA8).
// Naming wave 35c: Ego_r.pdb + FableWin.pdb ?WatchForLeaving@CQ_GuildTrainingWoodsMeleeScript@NScript@@QAEXXZ (the catalog carries the
//   name); its flags are Ego_r.pdb CQ_GuildTrainingWoodsMeleeScript MissionSucceeded +0x5c / MissionFailed +0x5d, retail - 0x14.
//   Was CV_AmbushScamScript::WatchForQuestFinished (old file CVAmbushScamScript_WatchForQuestFinished_00d66e90.cpp; a propagated label
//   shared by several script bodies), with the flags named QuestFinished / QuestComplete.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (twice), CGameScriptInterface::NewScriptFrame
// (slot 7, +0x1c) through Game at +0x40.
// Shape: the script WAIT macro `while (!(MissionFailed || MissionSucceeded)) { NewScriptFrame(); if (terminating)
// return; }` (rotated: MissionFailed is tested at the entry and at the bottom, MissionSucceeded at the aligned loop
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

namespace NScript {
class CQ_GuildTrainingWoodsMeleeScript : public CQuestScriptBase {
public:
    bool MissionSucceeded;                               // +0x48 (Ego_r.pdb +0x5c)
    bool MissionFailed;                                  // +0x49 (Ego_r.pdb +0x5d)
    void WatchForLeaving();
};
}

void NScript::CQ_GuildTrainingWoodsMeleeScript::WatchForLeaving()
{
    while (!(MissionFailed || MissionSucceeded)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!MissionSucceeded)
        MissionFailed = true;
}
