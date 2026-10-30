// CQ_BountyHuntScript::AddBoastQuestInfo() -- start the boast timer from the quest globals, show it as a quest-info timer and display the quest info
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D17EA0, length 119 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, SetTimer (slot 89,
// +0x164) through the game script interface global @ 0x0143E8F8, and through the quest's interface (+0x40):
// AddQuestInfoTimer (slot 326, +0x518, `(const CTimer&, const CCharString&, float)`), DisplayQuestInfo (slot 321,
// +0x504). String 0x012CA7C8 (masked); the quest-globals record @ 0x0143E90C holds the time limit at +0xb60.
// Layout: Ego_r.pdb CQ_BountyHuntScript - 0x14: BoastTimerID +0x7c, BoastTime (CTimer) +0x80,
// BoastQuestInfoDisplayed +0x84.
// Shape: straight-line; the string is a NAMED local in its own block (built before the interface is read, destroyed
// before the flag store).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
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
extern CGameScriptInterface* g_pGameScriptInterface;       // 0x0143E8F8

class CScriptDef { public:
    unsigned char _pad[0xb60];
    long          BountyHuntBoastTimeLimit;                 // +0xb60
};
extern CScriptDef* pGlobalScriptDef;                      // 0x0143E90C

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    void*                 Owner;                            // +0x44
};

namespace NScript {
class CQ_BountyHuntScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x7c - 0x48];                      // +0x48 quest flags, BanditGruntTextCounter, boast flags
    long          BoastTimerID;                             // +0x7c
    CTimer        BoastTime;                                // +0x80
    bool          BoastQuestInfoDisplayed;                  // +0x84
    void AddBoastQuestInfo();
};
} // namespace NScript
using NScript::CQ_BountyHuntScript;

void NScript::CQ_BountyHuntScript::AddBoastQuestInfo()
{
    g_pGameScriptInterface->SetTimer(BoastTime.TimerIndex, pGlobalScriptDef->BountyHuntBoastTimeLimit);
    {
        CCharString text("HUD_CLOCK_ICON", -1);
        BoastTimerID = GSI->AddQuestInfoTimer(BoastTime, text, 1.0f);
    }
    BoastQuestInfoDisplayed = true;
    GSI->DisplayQuestInfo(true);
}