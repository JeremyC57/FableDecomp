// NScript::CHeroBoastsScript::BoastTCGTimeLimit(CCharString questName) -- boast thread: wait for the quest's end, note the boast as taken, then mark boast 35 completed when the quest succeeded inside the time limit, failed otherwise
// Leaf: BoastTCGTimeLimit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE63A0, `ret 4` (the CCharString is passed by value and destroyed here).
// Identity: Ego_r.pdb `?BoastTCGTimeLimit@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z`; spawned by ControlNewBoast (0x00CE35DB
// stores this address next to the "BoastTCGTimeLimit" thread name). The catalog label "CQR_EscortTraderScript::WatchForRegionLoads"
// is propagated over the whole boast family (same shape as the matched BoastBreakSiegeKilledLeader 0x00CE5180).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CGameScriptInterface slots 7 NewScriptFrame (+0x1C),
// 28 MsgOnQuestCompletedBeforeScreenShown (+0x70), 29 MsgOnQuestFailedBeforeScreenShown (+0x74), 305 SetBoastAsFailed
// (+0x4C4), 306 SetBoastAsCompleted (+0x4C8) (ghidra_out/cgsi_slots_aligned.tsv), ~CCharString 0x0099EAE0.
// Layout: CScriptBase (retail 0x40 bytes), GSI +0x40, PMasterData (NScript::CQ_SunnyvaleMasterData*) +0x44; the master
// data's TCGMadeTimeLimit +0x127, TCGTimeLimitBoastTaken +0x132 (Ego_r.pdb, no shift).
// Shape: the failed test comes first, then completed-with-flag (success) and completed (failure); the watch loop is rotated (entry test, bottom test) with its own destructor epilogue on the normal exit;
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
    unsigned char _pad000[0x127];
    bool          TCGMadeTimeLimit;        // +0x127
    unsigned char _pad128[0x132 - 0x128];
    bool          TCGTimeLimitBoastTaken;  // +0x132
};

}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {

enum { BOAST_TCG_TIME_LIMIT = 35 };

class CHeroBoastsScript : public CScriptBase { public:
    CGameScriptInterface*   GSI;                        // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    void BoastTCGTimeLimit(CCharString questName);
};

void CHeroBoastsScript::BoastTCGTimeLimit(CCharString questName)
{
    PMasterData->TCGTimeLimitBoastTaken = true;
    while (!IsActiveThreadTerminating()) {
        if (!GSI->MsgOnQuestFailedBeforeScreenShown(questName)) {
            if (GSI->MsgOnQuestCompletedBeforeScreenShown(questName) && PMasterData->TCGMadeTimeLimit) {
                if (IsActiveThreadTerminating())
                    return;
                GSI->SetBoastAsCompleted(BOAST_TCG_TIME_LIMIT, questName);
                return;
            }
            if (!GSI->MsgOnQuestCompletedBeforeScreenShown(questName)) {
                GSI->NewScriptFrame();
                continue;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        GSI->SetBoastAsFailed(BOAST_TCG_TIME_LIMIT, questName);
        return;
    }
}
}