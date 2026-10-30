// CQ_BountyHuntScript::WatchForQuestFailure() -- every script frame, fail the quest once a villager is killed before the intro or a hostage is killed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D15090, length 88 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed; also the tail `jmp`), CGameScriptInterface
// slot 7 NewScriptFrame (+0x1c) through the interface pointer at +0x40.
// Layout (Ego_r.pdb CQ_BountyHuntScript - 0x14): QuestFailed +0x48, VillagerKilled +0x4d, VillagerIntroDone
// +0x4e, Hostage1Killed +0x54, Hostage2Killed +0x55.
// Shape: the failure branch leaves the loop: `if (terminating) return; QuestFailed = true;` and a final
// IsActiveThreadTerminating() whose discarded result VC7.1 turns into the tail `jmp`; the branch body is
// placed after the loop's `ret`.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
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

namespace NScript {
class CQ_BountyHuntScript : public CQuestScriptBase {
public:
    bool QuestFailed;                                       // +0x48
    unsigned char _pad49[4];                                // +0x49
    bool VillagerKilled;                                    // +0x4d
    bool VillagerIntroDone;                                 // +0x4e
    unsigned char _pad4f[5];                                // +0x4f
    bool Hostage1Killed;                                    // +0x54
    bool Hostage2Killed;                                    // +0x55
    void WatchForQuestFailure();
};
} // namespace NScript
using NScript::CQ_BountyHuntScript;

void NScript::CQ_BountyHuntScript::WatchForQuestFailure()
{
    while (!IsActiveThreadTerminating()) {
        if ((VillagerKilled && !VillagerIntroDone) || Hostage1Killed || Hostage2Killed) {
            if (IsActiveThreadTerminating())
                return;
            QuestFailed = true;
            IsActiveThreadTerminating();
            return;
        }
        GSI->NewScriptFrame();
    }
}