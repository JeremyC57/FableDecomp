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
class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057();
    virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088();
    virtual void SetTimer(long timer, long time);       // slot 89 (+0x164)
    virtual long GetTimer(long timer) const;            // slot 90 (+0x168)
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

namespace NScript {
struct CTimer {
    long TimerIndex;
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
};

struct CScriptBase {                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};

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