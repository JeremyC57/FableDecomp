// CV_ArcheryCompetitionScript::WatchForSignpostConditions() -- every script frame, pick the archery signpost's state from the contest phase and the hero's conduct
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E33120, length 193 bytes (208 with padding).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), and through the game script interface
// (+0x40): NewScriptFrame (slot 7, +0x1c).
// Layout (retail = Ego_r.pdb CV_ArcheryCompetitionScript - 0x14): QuestActivated +0x4e, AttackedByHero +0x4f,
// QuestFinished +0x51. The object at +0x44 (the quest-script base's second word, typed only as padding in the PDB)
// carries two longs the method reads/writes: a phase at +0x3c (0 and 2 handled) and the signpost state at +0x40
// (2 = closed, 1 = finished, 0 = open). The member names are descriptive.
// Shape: the signpost store precedes the loop; inside, `if (phase == 0) {...} else if (phase == 2) {...}`, every
// store guarded by its own terminating check (the script code's `if (terminating) return;` before each action).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct CQuestOwnerState {                                   // the +0x44 object (descriptive model)
    unsigned char _pad00[0x3c];
    long          m_phase;                                  // +0x3c
    long          m_signpostState;                          // +0x40
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    CQuestOwnerState*     Owner;                            // +0x44
};

namespace NScript {
class CV_ArcheryCompetitionScript : public CQuestScriptBase {
public:
    long          Score;                                    // +0x48
    bool          IsOwnerDead;                              // +0x4c
    bool          PrizeAlreadyGot;                          // +0x4d
    bool          QuestActivated;                           // +0x4e
    bool          AttackedByHero;                           // +0x4f
    bool          GiveAttackWarning;                        // +0x50
    bool          QuestFinished;                            // +0x51
    void WatchForSignpostConditions();
};
} // namespace NScript
using NScript::CV_ArcheryCompetitionScript;

void NScript::CV_ArcheryCompetitionScript::WatchForSignpostConditions()
{
    Owner->m_signpostState = 2;
    while (!IsActiveThreadTerminating()) {
        long phase = Owner->m_phase;
        if (phase == 0) {
            if (IsActiveThreadTerminating())
                return;
            if (AttackedByHero) {
                if (IsActiveThreadTerminating())
                    return;
                Owner->m_signpostState = 2;
            } else if (QuestFinished) {
                if (IsActiveThreadTerminating())
                    return;
                Owner->m_signpostState = 1;
            }
        } else if (phase == 2) {
            if (IsActiveThreadTerminating())
                return;
            if (QuestActivated && !AttackedByHero) {
                if (IsActiveThreadTerminating())
                    return;
                Owner->m_signpostState = 0;
            }
        }
        GSI->NewScriptFrame();
    }
}