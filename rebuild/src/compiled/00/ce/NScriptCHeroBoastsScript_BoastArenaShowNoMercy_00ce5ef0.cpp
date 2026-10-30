// NScript::CHeroBoastsScript::BoastArenaShowNoMercy(CCharString questName) -- boast thread: wait for the quest's end, mark boast 30 completed when the quest succeeded with Whisper killed by the hero, failed when it succeeded without that or failed
// Leaf: BoastArenaShowNoMercy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE5EF0, `ret 4` (the CCharString is passed by value and destroyed here).
// Identity: Ego_r.pdb `?BoastArenaShowNoMercy@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z`; spawned by ControlNewBoast (0x00CE31E9
// stores this address next to the "BoastArenaShowNoMercy" thread name). The catalog label "CQR_EscortTraderScript::WatchForRegionLoads"
// is propagated over the whole boast family (same shape as the matched BoastBreakSiegeKilledLeader 0x00CE5180).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CGameScriptInterface slots 7 NewScriptFrame (+0x1C),
// 28 MsgOnQuestCompletedBeforeScreenShown (+0x70), 29 MsgOnQuestFailedBeforeScreenShown (+0x74), 305 SetBoastAsFailed
// (+0x4C4), 306 SetBoastAsCompleted (+0x4C8) (ghidra_out/cgsi_slots_aligned.tsv), ~CCharString 0x0099EAE0.
// Layout: CScriptBase (retail 0x40 bytes), GSI +0x40, PMasterData (NScript::CQ_SunnyvaleMasterData*) +0x44; the master
// data's WhisperKilledByHero +0x74 (Ego_r.pdb, no shift).
// Shape: completed-with-flag (success) is tested first, then completed-without-flag and failed (both failure); the watch loop is rotated (entry test, bottom test) with its own destructor epilogue on the normal exit;
// both outcomes re-check thread termination before reporting, the failure tests share one block that falls into the
// terminating exit.
class CCharString { public:
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class CQ_SunnyvaleMasterData { public:
    unsigned char _pad000[0x74];
    bool          WhisperKilledByHero;     // +0x74
};

}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
public:
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {

enum { BOAST_ARENA_SHOW_NO_MERCY = 30 };

class CHeroBoastsScript : public CScriptBase { public:
    CGameScriptInterface*   GSI;                        // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    void BoastArenaShowNoMercy(CCharString questName);
};

void CHeroBoastsScript::BoastArenaShowNoMercy(CCharString questName)
{
    while (!IsActiveThreadTerminating()) {
        if (GSI->MsgOnQuestCompletedBeforeScreenShown(questName) && PMasterData->WhisperKilledByHero) {
            if (IsActiveThreadTerminating())
                return;
            GSI->SetBoastAsCompleted(BOAST_ARENA_SHOW_NO_MERCY, questName);
            return;
        }
        if (!(GSI->MsgOnQuestCompletedBeforeScreenShown(questName) && !PMasterData->WhisperKilledByHero) && !GSI->MsgOnQuestFailedBeforeScreenShown(questName)) {
            GSI->NewScriptFrame();
            continue;
        }
        if (IsActiveThreadTerminating())
            return;
        GSI->SetBoastAsFailed(BOAST_ARENA_SHOW_NO_MERCY, questName);
        return;
    }
}
}