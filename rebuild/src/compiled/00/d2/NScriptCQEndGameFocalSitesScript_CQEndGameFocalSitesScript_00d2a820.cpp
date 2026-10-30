// CQ_EndGameFocalSitesScript::CQ_EndGameFocalSitesScript(void* owner, CGameScriptInterface* game) -- quest-script constructor: script base, interface/owner pair, vtable 0x012CBEB4, two arrays of five empty strings and two registered timers
// Leaf: ??0CQ_EndGameFocalSitesScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D2A820, length 125 bytes, `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, CCharString::CCharString() @ 0x0099E4B0 (2 x 5 times, two inline
// counted loops), CGameScriptInterface::RegisterTimer (slot 87, +0x15c) through the interface global @ 0x0143E8F8.
// Layout: Ego_r.pdb CQ_EndGameFocalSitesScript - 0x14: CurrentFocalSiteRegion +0x48, TeleportHSPs +0x4c,
// RegionDialogueNames +0x60, TeleportPosition +0x74, JackTimer +0x88, WorldTimer +0x8c.
// Shape: the landed quest-script constructor family (0x00E68F70 novtable script-object base; CTimer registers itself
// in its inline constructor); the second loop head is aligned (`jmp ; lea ecx,[ecx]`).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString();                                   // 0x0099E4B0
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8

// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct __declspec(novtable) CScriptObject : CScriptBase {
    CGameScriptInterface* m_pGame;                   // +0x40
    void* m_pOwner;                                  // +0x44
    CScriptObject(void* owner, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pOwner(owner) { }
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
};

namespace NScript {
class CQ_EndGameFocalSitesScript : public CScriptObject {
public:  // vtable 0x012CBEB4
    long        CurrentFocalSiteRegion;              // +0x48
    CCharString TeleportHSPs[5];                     // +0x4c
    CCharString RegionDialogueNames[5];              // +0x60
    C3DVector   TeleportPosition;                    // +0x74
    bool        MissionFailed;                       // +0x80
    bool        JackLeaves;                          // +0x81
    bool        EndCutScenePlayed;                   // +0x82
    bool        QuestStartScreenShown;               // +0x83
    long        counter;                             // +0x84
    CTimer      JackTimer;                           // +0x88
    CTimer      WorldTimer;                          // +0x8c
    bool        JackReached;                         // +0x90
    virtual void v00();
    CQ_EndGameFocalSitesScript(void* owner, CGameScriptInterface* game);
};
} // namespace NScript
using NScript::CQ_EndGameFocalSitesScript;

NScript::CQ_EndGameFocalSitesScript::CQ_EndGameFocalSitesScript(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game)
{
}