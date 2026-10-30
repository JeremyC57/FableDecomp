// CV_ArcheryCompetitionScript::CalculateScoreForHit(long, float) -- add the hit's points to the score, doubled/tripled/quadrupled by the distance band from the master data, then refresh the game-score HUD counter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E36CA0, length 175 bytes (`ret 8`; 176 with padding).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), and through the game script interface (+0x40)
// UpdateQuestInfoCounter(long, long, long) (slot 335, +0x53c). Globals: the master data object @ 0x0143E90C, whose
// +0x354 points at the three archery distance bands (descriptive name).
// Layout (retail = Ego_r.pdb CV_ArcheryCompetitionScript - 0x14): Score +0x48, ArcheryGameScoreInfoID +0x5c.
// Shape: `distance < band[i]` (`test ah,5 ; jp`) if/else-if chain, each arm guarded by `if (terminating) return;`.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptDef { public:
    unsigned char _pad000[0x354];
    const float*  m_pArcheryDistanceBands;                  // +0x354: three ascending distances
};
extern CScriptDef* pGlobalScriptDef;                          // 0x0143E90C

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    void*                 Owner;                            // +0x44
};

namespace NScript {
class CV_ArcheryCompetitionScript : public CQuestScriptBase {
public:
    long          Score;                                    // +0x48
    unsigned char _pad4c[0x10];                             // +0x4c flags, DummyCounter, ArcheryHighScoreInfoID
    long          ArcheryGameScoreInfoID;                   // +0x5c
    void CalculateScoreForHit(long points, float distance);
};
} // namespace NScript
using NScript::CV_ArcheryCompetitionScript;

void NScript::CV_ArcheryCompetitionScript::CalculateScoreForHit(long points, float distance)
{
    const float* bands = pGlobalScriptDef->m_pArcheryDistanceBands;
    if (distance < bands[0]) {
        if (IsActiveThreadTerminating())
            return;
        Score += points;
    } else if (distance < bands[1]) {
        if (IsActiveThreadTerminating())
            return;
        Score += points * 2;
    } else if (distance < bands[2]) {
        if (IsActiveThreadTerminating())
            return;
        Score += points * 3;
    } else {
        if (IsActiveThreadTerminating())
            return;
        Score += points * 4;
    }
    GSI->UpdateQuestInfoCounter(ArcheryGameScoreInfoID, Score, -1);
}