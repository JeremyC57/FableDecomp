// CV_ArcheryCompetitionScript::RemoveArcheryQuestInfo() -- hide the quest info panel and remove the high-score, game-score and game-timer info elements
// Retail: Fable.exe @ 0x00E32EF0, 63 bytes (+ int3 padding). Callees (virtual, CGameScriptInterface through Game at +0x40):
// DisplayQuestInfo(false) slot 321 (+0x504), RemoveQuestInfoElement slot 338 (+0x548) three times.
// Layout: Ego_r.pdb CV_ArcheryCompetitionScript minus 0x14: ArcheryHighScoreInfoID +0x58, ArcheryGameScoreInfoID
// +0x5c, ArcheryGameTimerID +0x60; the interface pointer is re-read from +0x40 for every call.
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

void NScript::CV_ArcheryCompetitionScript::RemoveArcheryQuestInfo()
{
    Game->DisplayQuestInfo(false);
    Game->RemoveQuestInfoElement(ArcheryHighScoreInfoID);
    Game->RemoveQuestInfoElement(ArcheryGameScoreInfoID);
    Game->RemoveQuestInfoElement(ArcheryGameTimerID);
}