// NScript::CQ_AmbushTradersScript::WatchForSpySpeechTimer() -- speech thread: forever (until terminated) wait while spy speech is enabled, wait for the spy speech timer to run out, then re-enable spy speech and restart the timer with the script def's delay
// Leaf: ?WatchForSpySpeechTimer@CQ_AmbushTradersScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F036C0, length 214 bytes, plain `ret`.
// Identity: catalog CQ_AmbushTradersScript::WatchForSpySpeechTimer, the spy twin of WatchForBanditSpeechTimer 0x00F035E0
// (same class, same speech-flag / timer pair one slot apart).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface slot 7 NewScriptFrame (+0x1C) through
// the script's GSI (+0x40); slots 89 SetTimer(long, long) (+0x164) and 90 GetTimer(long) (+0x168) through the global
// g_pGameScriptInterface @ 0x0143E8F8 (the inline CTimer::SetTime / GetTime). Data: pGlobalScriptDef @ 0x0143E90C, its
// +0xA50 spy speech delay (descriptive name; the retail CScriptDef does not line up with Ego_r.pdb here).
// Layout: retail quest base 0x48 bytes; EnableSpeechSpy +0x69, SpeechSpyTimer +0xA0 (Ego_r.pdb +0x7D/+0xB4 less the
// 0x14 debug-container shift).
// Shape: `while (!terminating)` rotated (entry and bottom tests) around two script waits, each rotated with its own
// termination test after it; the timer restart pushes the def value first (the inline SetTime's argument), then the
// timer index.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

class CScriptDef { public:
    unsigned char _pad000[0xa50];
    long AmbushTradersSpySpeechDelay;                   // +0xa50
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

namespace NScript {
struct CTimer {
    long TimerIndex;
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
};

}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {

class CQ_AmbushTradersScript : public CScriptBase { public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char _flags048[0x21];                      // +0x48 .. +0x69 (QuestFailed .. BanditContactKilledBeforeQuestEnd)
    bool   EnableSpeechSpy;                             // +0x69
    bool   EnableSpeechBandit;                          // +0x6a
    unsigned char _pad06b[0x2d];                        // +0x6b .. +0x98 (alive and speech counters)
    bool   BanditSwitchAllegienceComplete;              // +0x98
    CTimer SpeechBanditTimer;                           // +0x9c
    CTimer SpeechSpyTimer;                              // +0xa0
    void WatchForSpySpeechTimer();
};

void CQ_AmbushTradersScript::WatchForSpySpeechTimer()
{
    while (!IsActiveThreadTerminating()) {
        while (EnableSpeechSpy) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        while (SpeechSpyTimer.GetTime() != 0) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        EnableSpeechSpy = true;
        SpeechSpyTimer.SetTime(pGlobalScriptDef->AmbushTradersSpySpeechDelay);
        GSI->NewScriptFrame();
    }
}
}