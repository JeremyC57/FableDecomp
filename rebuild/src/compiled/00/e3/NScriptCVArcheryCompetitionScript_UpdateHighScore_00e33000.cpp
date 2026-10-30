// CV_ArcheryCompetitionScript::UpdateHighScore(long) -- show the new high score in its quest info counter (no maximum) and post it as the online archery score
// Retail: Fable.exe @ 0x00E33000, 48 bytes, `ret 4`. Callees (virtual, CGameScriptInterface through Game at +0x40):
// UpdateQuestInfoCounter(id, value, -1) slot 335 (+0x53c), UpdateOnlineScore_Archery(float) slot 725 (+0xb54).
// Layout: Ego_r.pdb CV_ArcheryCompetitionScript minus 0x14: ArcheryHighScoreInfoID +0x58.
// Shape: the score is converted with `fild` straight from its argument slot into the pushed float.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class CV_ArcheryCompetitionScript {
public:                 // quest-script object (retail = Ego_r.pdb - 0x14)
    unsigned char _base[0x40];                       // CScriptBase
    CGameScriptInterface* Game;                      // +0x40
    void* Owner;                                     // +0x44
    long Score;                                      // +0x48
    unsigned char _flags4c[8];                       // +0x4c IsOwnerDead .. Pickpocketed
    long DummyCounter;                               // +0x54
    long ArcheryHighScoreInfoID;                     // +0x58
    long ArcheryGameScoreInfoID;                     // +0x5c
    long ArcheryGameTimerID;                         // +0x60
    void RemoveArcheryQuestInfo();
    void UpdateHighScore(long score);
};
} // namespace NScript
using NScript::CV_ArcheryCompetitionScript;

void NScript::CV_ArcheryCompetitionScript::UpdateHighScore(long score)
{
    Game->UpdateQuestInfoCounter(ArcheryHighScoreInfoID, score, -1);
    Game->UpdateOnlineScore_Archery((float)score);
}