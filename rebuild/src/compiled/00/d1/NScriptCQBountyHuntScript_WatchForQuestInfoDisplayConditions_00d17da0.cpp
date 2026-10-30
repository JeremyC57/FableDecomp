// NScript::CQ_BountyHuntScript::WatchForQuestInfoDisplayConditions() -- boast thread: while the time-limit boast is active, show the boast's quest info once the quest has started and remove it again when the quest succeeds, fails or the time limit is exceeded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D17DA0, length 203 bytes, plain `ret` / tail `jmp`.
// Identity: Ego_r.pdb `?WatchForQuestInfoDisplayConditions@CQ_BountyHuntScript@NScript@@QAEXXZ` (next to
// AddBoastQuestInfo there, as here: 0x00D17EA0 is landed under that name).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface slot 7 NewScriptFrame (+0x1C);
// CQ_BountyHuntScript::AddBoastQuestInfo 0x00D17EA0; the quest-info removal 0x00D17E70 (DisplayQuestInfo(false),
// BoastQuestInfoDisplayed = false, RemoveQuestInfo(BoastTimerID); no Ego_r.pdb symbol, the debug build inlines it,
// catalog label "CBowerstoneIntroGuard::Init" is propagated): named RemoveBoastQuestInfo_00d17e70.
// Layout: retail quest base 0x48 bytes (GSI +0x40, PMasterData +0x44); QuestFailed +0x48, QuestSucceeded +0x49,
// QuestStarted +0x4B, BoastActiveTimeLimit +0x78, BoastQuestInfoDisplayed +0x84 (Ego_r.pdb less 0x14);
// CQ_SunnyvaleMasterData::BountyHuntTimeLimitExceeded +0xDA (no shift).
// Shape: the frame loop is `for (;;)` entered through a termination return and closed by one; its only other exit
// is the `break` after the removal, which falls into the function's closing discarded IsActiveThreadTerminating()
// (the tail `jmp` shared with the inactive-boast path); the quest-started wait is the rotated script idiom.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class CQ_SunnyvaleMasterData { public:
    unsigned char _pad000[0xda];
    bool          BountyHuntTimeLimitExceeded;          // +0xda
};

}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
public:
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {

class CQ_BountyHuntScript : public CScriptBase { public:
    CGameScriptInterface*   GSI;                        // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    bool QuestFailed;                                   // +0x48
    bool QuestSucceeded;                                // +0x49
    bool QuestOver;                                     // +0x4a
    bool QuestStarted;                                  // +0x4b
    unsigned char _flags04c[0x2c];                      // +0x4c .. +0x78 (kill / intro / boss flags, grunt counter)
    bool BoastActiveTimeLimit;                          // +0x78
    bool BoastActiveDecapitation;                       // +0x79
    long BoastTimerID;                                  // +0x7c
    long BoastTime;                                     // +0x80 (CTimer)
    bool BoastQuestInfoDisplayed;                       // +0x84
    void WatchForQuestInfoDisplayConditions();
    void RemoveBoastQuestInfo_00d17e70();               // 0x00D17E70
    void AddBoastQuestInfo();                           // 0x00D17EA0
};

void CQ_BountyHuntScript::WatchForQuestInfoDisplayConditions()
{
    if (!BoastActiveTimeLimit) {
        IsActiveThreadTerminating();
        return;
    }
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        if (BoastQuestInfoDisplayed) {
            if (QuestSucceeded || QuestFailed || PMasterData->BountyHuntTimeLimitExceeded) {
                if (IsActiveThreadTerminating())
                    return;
                RemoveBoastQuestInfo_00d17e70();
                IsActiveThreadTerminating();
                return;
            }
        } else if (BoastActiveTimeLimit && !QuestSucceeded && !QuestFailed) {
            if (IsActiveThreadTerminating())
                return;
            while (!QuestStarted) {
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            AddBoastQuestInfo();
        }
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}
}