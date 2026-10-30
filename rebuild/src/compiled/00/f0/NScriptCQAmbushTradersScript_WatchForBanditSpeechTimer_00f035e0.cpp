// NScript::CQ_AmbushTradersScript::WatchForBanditSpeechTimer() -- speech thread: until the bandit contact has switched allegiance, wait while bandit speech is enabled, then wait for the bandit speech timer to run out and re-enable the speech
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F035E0, length 219 bytes, plain `ret` / tail `jmp`.
// Identity: catalog CQ_AmbushTradersScript::WatchForBanditSpeechTimer, next to the landed WatchForQuestFailure
// 0x00F03270 and WatchForBoastSuccess 0x00F03560 of the same class; the members it reads are the class's own
// (Ego_r.pdb layout).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface slot 7 NewScriptFrame (+0x1C) through
// the script's GSI (+0x40), slot 90 GetTimer(long) (+0x168) through the global g_pGameScriptInterface @ 0x0143E8F8
// (the inline CTimer::GetTime()).
// Layout: retail quest base 0x48 bytes; EnableSpeechBandit +0x6A, BanditSwitchAllegienceComplete +0x98,
// SpeechBanditTimer +0x9C (Ego_r.pdb +0x7E/+0xAC/+0xB0 less the 0x14 debug-container shift).
// Shape: the outer loop is `for (;;)` closed by a termination return (its exit is a plain `ret`); each wait is the
// script idiom `while (!done) { NewScriptFrame(); if (terminating) return; }` followed by its own termination test,
// both waits rotated by VC (the allegiance re-test moves to the bottom, the redundant entry re-test of the second
// wait is folded into the `if (...) break;` before it); the break path re-tests termination and the closing
// discarded IsActiveThreadTerminating() becomes the tail `jmp`.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

namespace NScript {
struct CTimer {
    long TimerIndex;
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
};

}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
public:
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
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
    void WatchForBanditSpeechTimer();
};

void CQ_AmbushTradersScript::WatchForBanditSpeechTimer()
{
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        while (!BanditSwitchAllegienceComplete && EnableSpeechBandit) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (BanditSwitchAllegienceComplete)
            break;
        while (!BanditSwitchAllegienceComplete && SpeechBanditTimer.GetTime() != 0) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (BanditSwitchAllegienceComplete)
            break;
        EnableSpeechBandit = true;
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    IsActiveThreadTerminating();
}
}