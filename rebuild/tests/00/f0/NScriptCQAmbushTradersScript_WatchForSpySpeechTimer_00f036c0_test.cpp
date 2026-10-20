// Behaviour test for 00f036c0: the model below is a copy of src/00f036c0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
struct CGameScriptInterface {
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {} virtual void v006() {}
    virtual void NewScriptFrame() const;                // slot 7 (+0x1c)
    virtual void v008() {} virtual void v009() {} virtual void v010() {} virtual void v011() {} virtual void v012() {} virtual void v013() {} virtual void v014() {} virtual void v015() {} virtual void v016() {} virtual void v017() {}
    virtual void v018() {} virtual void v019() {} virtual void v020() {} virtual void v021() {} virtual void v022() {} virtual void v023() {} virtual void v024() {} virtual void v025() {} virtual void v026() {} virtual void v027() {}
    virtual void v028() {} virtual void v029() {} virtual void v030() {} virtual void v031() {} virtual void v032() {} virtual void v033() {} virtual void v034() {} virtual void v035() {} virtual void v036() {} virtual void v037() {}
    virtual void v038() {} virtual void v039() {} virtual void v040() {} virtual void v041() {} virtual void v042() {} virtual void v043() {} virtual void v044() {} virtual void v045() {} virtual void v046() {} virtual void v047() {}
    virtual void v048() {} virtual void v049() {} virtual void v050() {} virtual void v051() {} virtual void v052() {} virtual void v053() {} virtual void v054() {} virtual void v055() {} virtual void v056() {} virtual void v057() {}
    virtual void v058() {} virtual void v059() {} virtual void v060() {} virtual void v061() {} virtual void v062() {} virtual void v063() {} virtual void v064() {} virtual void v065() {} virtual void v066() {} virtual void v067() {}
    virtual void v068() {} virtual void v069() {} virtual void v070() {} virtual void v071() {} virtual void v072() {} virtual void v073() {} virtual void v074() {} virtual void v075() {} virtual void v076() {} virtual void v077() {}
    virtual void v078() {} virtual void v079() {} virtual void v080() {} virtual void v081() {} virtual void v082() {} virtual void v083() {} virtual void v084() {} virtual void v085() {} virtual void v086() {} virtual void v087() {}
    virtual void v088() {}
    virtual void SetTimer(long timer, long time);       // slot 89 (+0x164)
    virtual long GetTimer(long timer) const;            // slot 90 (+0x168)
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

struct CScriptDef {
    unsigned char _pad000[0xa50];
    long AmbushTradersSpySpeechDelay;                   // +0xa50
};
extern CScriptDef* g_pScriptDef;                        // 0x0143E90C

namespace NScript {
struct CTimer {
    long TimerIndex;
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
};

struct CScriptBase {                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00() {}
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};

struct CQ_AmbushTradersScript : CScriptBase {
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
        SpeechSpyTimer.SetTime(g_pScriptDef->AmbushTradersSpySpeechDelay);
        GSI->NewScriptFrame();
    }
}
}

// ---- test doubles and driver ----
static int g_frames = 0, g_termAt = 1000, g_disableEvery = 3, g_timerZeroAfter = 2;
static int g_setCalls = 0, g_lastSetIndex = 0, g_lastSetTime = 0, g_timerStart = 0, g_bad = 0;
static NScript::CQ_AmbushTradersScript* g_s = 0;
void CGameScriptInterface::NewScriptFrame() const
{
    ++g_frames;
    if (g_s->EnableSpeechSpy && g_frames % g_disableEvery == 0) g_s->EnableSpeechSpy = false;   // the speech was spoken
}
void CGameScriptInterface::SetTimer(long timer, long time) { ++g_setCalls; g_lastSetIndex = timer; g_lastSetTime = time; g_timerStart = g_frames; }
long CGameScriptInterface::GetTimer(long timer) const { if (timer != 9) g_bad = 1; return g_frames - g_timerStart >= g_timerZeroAfter ? 0 : 1; }
CGameScriptInterface* g_pGameScriptInterface = 0;
CScriptDef* g_pScriptDef = 0;
bool NScript::CScriptBase::IsActiveThreadTerminating() { return g_frames >= g_termAt; }

int main()
{
    static CGameScriptInterface gsi;
    static CScriptDef def;
    def.AmbushTradersSpySpeechDelay = 1234;
    g_pGameScriptInterface = &gsi; g_pScriptDef = &def;
    bool ok = true;
    NScript::CQ_AmbushTradersScript s;
    std::memset(&s.GSI, 0, sizeof(s) - sizeof(void*) - sizeof(s._body));
    s.GSI = &gsi; s.SpeechSpyTimer.TimerIndex = 9; g_s = &s;

    // terminated at entry
    g_frames = 0; g_termAt = 0; s.EnableSpeechSpy = false;
    s.WatchForSpySpeechTimer();
    ok = ok && g_frames == 0 && g_setCalls == 0 && !s.EnableSpeechSpy;

    // run 20 frames: every cycle re-enables the speech and restarts the timer with the def delay
    g_frames = 0; g_termAt = 20; s.EnableSpeechSpy = true; g_timerStart = 0;
    s.WatchForSpySpeechTimer();
    ok = ok && g_frames == 20 && g_setCalls >= 3 && g_lastSetIndex == 9 && g_lastSetTime == 1234 && !g_bad;

    if (!ok) { std::printf("00f036c0_TEST FAIL\n"); return 1; }
    std::printf("00f036c0_TEST PASS\n");
    return 0;
}