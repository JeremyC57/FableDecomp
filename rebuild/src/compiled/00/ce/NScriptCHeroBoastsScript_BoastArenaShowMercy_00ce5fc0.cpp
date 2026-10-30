// NScript::CHeroBoastsScript::BoastArenaShowMercy(CCharString questName) -- boast thread: wait for the quest's end, mark boast 31 completed when the quest succeeded with Whisper spared, failed when Whisper was killed or the quest failed
// Leaf: BoastArenaShowMercy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE5FC0, `ret 4` (the CCharString is passed by value and destroyed here).
// Identity: Ego_r.pdb `?BoastArenaShowMercy@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z`; spawned by ControlNewBoast (0x00CE32B3
// stores this address next to the "BoastArenaShowMercy" thread name). The catalog label "CQR_EscortTraderScript::WatchForRegionLoads"
// is propagated over the whole boast family (same shape as the matched BoastBreakSiegeKilledLeader 0x00CE5180).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CGameScriptInterface slots 7 NewScriptFrame (+0x1C),
// 28 MsgOnQuestCompletedBeforeScreenShown (+0x70), 29 MsgOnQuestFailedBeforeScreenShown (+0x74), 305 SetBoastAsFailed
// (+0x4C4), 306 SetBoastAsCompleted (+0x4C8) (ghidra_out/cgsi_slots_aligned.tsv), ~CCharString 0x0099EAE0.
// Layout: CScriptBase (retail 0x40 bytes), GSI +0x40, PMasterData (NScript::CQ_SunnyvaleMasterData*) +0x44; the master
// data's WhisperKilledByHero +0x74 (Ego_r.pdb, no shift).
// Shape: completed-without-flag (success) is tested first, then completed-with-flag and failed (both failure); the watch loop is rotated (entry test, bottom test) with its own destructor epilogue on the normal exit;
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
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {

enum { BOAST_ARENA_SHOW_MERCY = 31 };

class CHeroBoastsScript : public CScriptBase { public:
    CGameScriptInterface*   GSI;                        // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    void BoastArenaShowMercy(CCharString questName);
};

void CHeroBoastsScript::BoastArenaShowMercy(CCharString questName)
{
    while (!IsActiveThreadTerminating()) {
        if (GSI->MsgOnQuestCompletedBeforeScreenShown(questName) && !PMasterData->WhisperKilledByHero) {
            if (IsActiveThreadTerminating())
                return;
            GSI->SetBoastAsCompleted(BOAST_ARENA_SHOW_MERCY, questName);
            return;
        }
        if (!(GSI->MsgOnQuestCompletedBeforeScreenShown(questName) && PMasterData->WhisperKilledByHero) && !GSI->MsgOnQuestFailedBeforeScreenShown(questName)) {
            GSI->NewScriptFrame();
            continue;
        }
        if (IsActiveThreadTerminating())
            return;
        GSI->SetBoastAsFailed(BOAST_ARENA_SHOW_MERCY, questName);
        return;
    }
}
}