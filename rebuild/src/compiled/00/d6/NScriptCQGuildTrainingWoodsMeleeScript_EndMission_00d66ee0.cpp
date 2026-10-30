// CQ_GuildTrainingWoodsMeleeScript::EndMission() -- wait one script frame at a time until the mission is over, then (unless the script thread is terminating) complete the active quest and mark the mission succeeded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D66EE0, length 103 bytes.
// Label: the catalog says CQ_CinemaTestScript::EndMission (propagated from the byte-near twin 0x00F02100); the only
// caller is 0x00D66E7C inside CQ_GuildTrainingWoodsMeleeScript::DoMission (0x00D66CA0), so this is that quest's EndMission.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), ~CCharString @ 0x0099EAE0, and through the
// quest's game script interface (+0x40): NewScriptFrame (slot 7, +0x1c), GetActiveQuestName (slot 655, +0xa3c, a
// CCharString by value), SetQuestAsCompleted (slot 288, +0x480).
// Layout: the retail quest-script base is 0x48 bytes (CScriptBase 0x40 + interface pointer + owner); MissionSucceeded
// +0x48, MissionOver +0x4a (Ego_r.pdb offsets - 0x14, as the landed 0x00D68CD0 EndMission).
// Shape: the landed EndMission loop (`while (!MissionOver) { NewScriptFrame(); if (terminating) return; }`), then the
// quest name temporary is the first argument of SetQuestAsCompleted (both calls share one vtable load in edi).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
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

namespace NScript {
class CQ_GuildTrainingWoodsMeleeScript : public CQuestScriptBase {
public:
    bool MissionSucceeded;                                  // +0x48
    bool MissionFailed;                                     // +0x49
    bool MissionOver;                                       // +0x4a
    void EndMission();
};
} // namespace NScript
using NScript::CQ_GuildTrainingWoodsMeleeScript;

void NScript::CQ_GuildTrainingWoodsMeleeScript::EndMission()
{
    while (!MissionOver) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GSI->SetQuestAsCompleted(GSI->GetActiveQuestName(), true, false, false);
    MissionSucceeded = true;
}