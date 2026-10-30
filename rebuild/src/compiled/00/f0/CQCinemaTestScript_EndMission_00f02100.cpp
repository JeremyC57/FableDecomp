// CQ_CinemaTestScript::EndMission() -- wait one script frame at a time until the mission is over, then (unless the script thread is terminating) complete the active quest (second flag set) and mark the mission succeeded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F02100, length 103 bytes.
// Caller: 0x00F02019 in CQ_CinemaTestScript (the label is consistent with its neighbours 0x00F02030..0x00F02210).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), ~CCharString @ 0x0099EAE0, and through the
// quest's game script interface (+0x40): NewScriptFrame (slot 7, +0x1c), GetActiveQuestName (slot 655, +0xa3c, a
// CCharString by value), SetQuestAsCompleted (slot 288, +0x480).
// Layout: the retail quest-script base is 0x48 bytes (CScriptBase 0x40 + interface pointer + owner); MissionSucceeded
// +0x48, MissionOver +0x4a (Ego_r.pdb offsets - 0x14, as the landed 0x00D68CD0 EndMission).
// Shape: the landed EndMission loop (`while (!MissionOver) { NewScriptFrame(); if (terminating) return; }`), then the
// quest name temporary is the first argument of SetQuestAsCompleted (both calls share one vtable load in edi).
class CCharString { public:
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    void*                 Owner;                            // +0x44
};

struct CQ_CinemaTestScript : CQuestScriptBase {
    bool MissionSucceeded;                                  // +0x48
    bool MissionFailed;                                     // +0x49
    bool MissionOver;                                       // +0x4a
    void EndMission();
};

void CQ_CinemaTestScript::EndMission()
{
    while (!MissionOver) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GSI->SetQuestAsCompleted(GSI->GetActiveQuestName(), true, true, false);
    MissionSucceeded = true;
}