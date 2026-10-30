// CV_ArcheryCompetitionScript::AddArcheryQuestInfo() -- add the high-score and game-score counters and the game timer to the quest-info HUD, show the current high score (also posted as the online archery score) and display the quest info
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E33040, length 222 bytes (224 with padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, and through the
// game script interface (+0x40): AddQuestInfoTimer(const CTimer&, const CCharString&, float) (slot 326, +0x518),
// AddQuestInfoCounter(const CCharString&, long, float) (slot 327, +0x51c), UpdateQuestInfoCounter(long, long, long)
// (slot 335, +0x53c), UpdateOnlineScore_Archery(float) (slot 725, +0xb54), DisplayQuestInfo(bool) (slot 321, +0x504).
// Layout (retail = Ego_r.pdb CV_ArcheryCompetitionScript - 0x14): ArcheryHighScoreInfoID +0x58,
// ArcheryGameScoreInfoID +0x5c, ArcheryGameTimerID +0x60, GameTimeLeft (CTimer) +0x6c; the +0x44 object's long at
// +0x44 is the high score (descriptive name).
// Shape: the icon names are IMPLICIT CCharString conversions of the literals (built before the interface is read,
// re-addressed with `lea`); the high score is a named long local (homed in the string's dead frame word for the fild).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#define FABLE_RETAIL_NSCRIPT
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct CQuestOwnerState {                                   // the +0x44 object (descriptive model)
    unsigned char _pad00[0x44];
    long          m_highScore;                              // +0x44
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    CQuestOwnerState*     Owner;                            // +0x44
};

namespace NScript {
class CV_ArcheryCompetitionScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x10];                             // +0x48 Score, flags, DummyCounter
    long          ArcheryHighScoreInfoID;                   // +0x58
    long          ArcheryGameScoreInfoID;                   // +0x5c
    long          ArcheryGameTimerID;                       // +0x60
    unsigned char _pad64[8];                                // +0x64 SteppedOut .. AddedFirstHighScorePrizeToOwner
    CTimer        GameTimeLeft;                             // +0x6c
    void AddArcheryQuestInfo();
};
} // namespace NScript
using NScript::CV_ArcheryCompetitionScript;

void NScript::CV_ArcheryCompetitionScript::AddArcheryQuestInfo()
{
    ArcheryHighScoreInfoID = GSI->AddQuestInfoCounter("HUD_ICON_MULTI_ARROW", 0, 1.0f);
    ArcheryGameScoreInfoID = GSI->AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0f);
    ArcheryGameTimerID = GSI->AddQuestInfoTimer(GameTimeLeft, "HUD_CLOCK_ICON", 1.0f);
    long highScore = Owner->m_highScore;
    GSI->UpdateQuestInfoCounter(ArcheryHighScoreInfoID, highScore, -1);
    GSI->UpdateOnlineScore_Archery((float)highScore);
    GSI->DisplayQuestInfo(true);
}