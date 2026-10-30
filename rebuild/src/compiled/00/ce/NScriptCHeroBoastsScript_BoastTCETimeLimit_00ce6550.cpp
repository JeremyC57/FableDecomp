// NScript::CHeroBoastsScript::BoastTCETimeLimit(CCharString questName) -- boast thread: wait for the quest's end, note the boast as taken, then mark boast 37 completed when the quest succeeded inside the time limit, failed otherwise
// Leaf: BoastTCETimeLimit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE6550, `ret 4` (the CCharString is passed by value and destroyed here).
// Identity: Ego_r.pdb `?BoastTCETimeLimit@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z`; spawned by ControlNewBoast (0x00CE376C
// stores this address next to the "BoastTCETimeLimit" thread name). The catalog label "CQR_EscortTraderScript::WatchForRegionLoads"
// is propagated over the whole boast family (same shape as the matched BoastBreakSiegeKilledLeader 0x00CE5180).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CGameScriptInterface slots 7 NewScriptFrame (+0x1C),
// 28 MsgOnQuestCompletedBeforeScreenShown (+0x70), 29 MsgOnQuestFailedBeforeScreenShown (+0x74), 305 SetBoastAsFailed
// (+0x4C4), 306 SetBoastAsCompleted (+0x4C8) (ghidra_out/cgsi_slots_aligned.tsv), ~CCharString 0x0099EAE0.
// Layout: CScriptBase (retail 0x40 bytes), GSI +0x40, PMasterData (NScript::CQ_SunnyvaleMasterData*) +0x44; the master
// data's TCEMadeTimeLimit +0x129, TCETimeLimitBoastTaken +0x131 (Ego_r.pdb, no shift).
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
    unsigned char _pad000[0x129];
    bool          TCEMadeTimeLimit;        // +0x129
    unsigned char _pad12a[0x131 - 0x12a];
    bool          TCETimeLimitBoastTaken;  // +0x131
};

}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
public:
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {

enum { BOAST_TCE_TIME_LIMIT = 37 };

class CHeroBoastsScript : public CScriptBase { public:
    CGameScriptInterface*   GSI;                        // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    void BoastTCETimeLimit(CCharString questName);
};

void CHeroBoastsScript::BoastTCETimeLimit(CCharString questName)
{
    PMasterData->TCETimeLimitBoastTaken = true;
    while (!IsActiveThreadTerminating()) {
        if (!GSI->MsgOnQuestFailedBeforeScreenShown(questName)) {
            if (GSI->MsgOnQuestCompletedBeforeScreenShown(questName) && PMasterData->TCEMadeTimeLimit) {
                if (IsActiveThreadTerminating())
                    return;
                GSI->SetBoastAsCompleted(BOAST_TCE_TIME_LIMIT, questName);
                return;
            }
            if (!GSI->MsgOnQuestCompletedBeforeScreenShown(questName)) {
                GSI->NewScriptFrame();
                continue;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        GSI->SetBoastAsFailed(BOAST_TCE_TIME_LIMIT, questName);
        return;
    }
}
}