// CQ_BountyHuntScript::WatchForQuestSuccess() -- every script frame, mark the quest succeeded once both hostages are safe
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D150F0, length 66 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), CGameScriptInterface slot 7
// NewScriptFrame (+0x1c) through the interface pointer at +0x40.
// Layout (Ego_r.pdb - 0x14): QuestSucceeded +0x49, Hostage1Safe +0x56, Hostage2Safe +0x57.
// Shape: `while (!terminating) { if (Hostage1Safe && Hostage2Safe) { if (terminating) return;
// QuestSucceeded = true; } NewScriptFrame(); }` -- entry test, aligned loop head (`lea esp,[esp]`), bottom test.
struct CGameScriptInterface_Calls {                      // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame();   // slot 7 (+0x1c)
};

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface_Calls* GSI;                              // +0x40
    unsigned char _pad44[4];                                // +0x44
};

struct CQ_BountyHuntScript : CQuestScriptBase {
    bool QuestFailed;                                       // +0x48
    bool QuestSucceeded;                                    // +0x49
    unsigned char _pad4a[0x0c];                             // +0x4a
    bool Hostage1Safe;                                      // +0x56
    bool Hostage2Safe;                                      // +0x57
    void WatchForQuestSuccess();
};

void CQ_BountyHuntScript::WatchForQuestSuccess()
{
    while (!IsActiveThreadTerminating()) {
        if (Hostage1Safe && Hostage2Safe) {
            if (IsActiveThreadTerminating())
                return;
            QuestSucceeded = true;
        }
        GSI->NewScriptFrame();
    }
}