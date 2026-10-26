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