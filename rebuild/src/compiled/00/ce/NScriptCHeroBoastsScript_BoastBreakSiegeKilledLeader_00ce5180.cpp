// NScript::CHeroBoastsScript::BoastBreakSiegeKilledLeader(CCharString questName) -- boast thread: wait for the quest's end and mark boast 26 completed when it succeeded with the siege leader killed, failed otherwise
// Leaf: BoastBreakSiegeKilledLeader
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE5180, length 196 bytes, `ret 4` (the CCharString is passed by value
// and destroyed here).
// Identity: Ego_r.pdb `?BoastBreakSiegeKilledLeader@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z`; spawned by
// ControlNewBoast (0x00CE292D stores this address next to the "BoastBreakSiegeKilledLeader" thread name). The catalog
// label "CQR_EscortTraderScript::WatchForRegionLoads" is propagated over the whole boast family.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CGameScriptInterface slots 7 NewScriptFrame (+0x1C),
// 28 MsgOnQuestCompletedBeforeScreenShown (+0x70), 29 MsgOnQuestFailedBeforeScreenShown (+0x74), 305 SetBoastAsFailed
// (+0x4C4), 306 SetBoastAsCompleted (+0x4C8) (ghidra_out/cgsi_slots_aligned.tsv), ~CCharString 0x0099EAE0.
// Layout: CScriptBase (retail 0x40 bytes), GSI +0x40, PMasterData (NScript::CQ_SunnyvaleMasterData*) +0x44; the master
// data's BreakSiegeKilledLeader flag +0x117 (Ego_r.pdb, no shift).
// Shape: the watch loop is rotated (entry test, bottom test) and its normal exit keeps its own destructor epilogue; the
// completed and failed outcomes each re-check thread termination before reporting; the two failure tests share one
// failure block, which falls into the terminating exit.
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
    unsigned char _pad000[0x117];
    bool          BreakSiegeKilledLeader;               // +0x117
};

}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
namespace NScript {

enum { BOAST_BREAK_SIEGE_KILLED_LEADER = 26 };

class CHeroBoastsScript : public CScriptBase { public:
    CGameScriptInterface*   GSI;                        // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    void BoastBreakSiegeKilledLeader(CCharString questName);
};

void CHeroBoastsScript::BoastBreakSiegeKilledLeader(CCharString questName)
{
    while (!IsActiveThreadTerminating()) {
        if (!GSI->MsgOnQuestFailedBeforeScreenShown(questName)) {
            if (GSI->MsgOnQuestCompletedBeforeScreenShown(questName) && PMasterData->BreakSiegeKilledLeader) {
                if (IsActiveThreadTerminating())
                    return;
                GSI->SetBoastAsCompleted(BOAST_BREAK_SIEGE_KILLED_LEADER, questName);
                return;
            }
            if (!GSI->MsgOnQuestCompletedBeforeScreenShown(questName)) {
                GSI->NewScriptFrame();
                continue;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        GSI->SetBoastAsFailed(BOAST_BREAK_SIEGE_KILLED_LEADER, questName);
        return;
    }
}
}