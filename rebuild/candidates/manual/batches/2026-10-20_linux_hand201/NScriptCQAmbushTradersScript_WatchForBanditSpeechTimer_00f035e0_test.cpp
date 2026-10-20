// Behaviour test for 00f035e0: the model below is a copy of src/00f035e0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __thiscall
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

namespace NScript {
struct CTimer {
    long TimerIndex;
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
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

// ---- test doubles and driver ----
// scripted world: frame counter; the timer reads 0 on chosen frames; flags flip on chosen frames
static int g_frames = 0, g_termAt = 1000, g_switchAt = 1000, g_disableAt = 1000, g_timerZeroFrom = 1000;
static int g_termCalls = 0, g_getTimerCalls = 0, g_badTimer = 0, g_reenables = 0;
static NScript::CQ_AmbushTradersScript* g_s = 0;
static bool g_prevEnable = false;
void CGameScriptInterface::NewScriptFrame() const
{
    if (!g_prevEnable && g_s->EnableSpeechBandit) ++g_reenables;
    ++g_frames;
    if (g_frames == g_switchAt) g_s->BanditSwitchAllegienceComplete = true;
    if (g_frames == g_disableAt) g_s->EnableSpeechBandit = false;
    g_prevEnable = g_s->EnableSpeechBandit;
}
void CGameScriptInterface::SetTimer(long, long) {}
long CGameScriptInterface::GetTimer(long timer) const { ++g_getTimerCalls; if (timer != 42) g_badTimer = 1; return g_frames >= g_timerZeroFrom ? 0 : 5; }
CGameScriptInterface* g_pGameScriptInterface = 0;
bool NScript::CScriptBase::IsActiveThreadTerminating() { ++g_termCalls; return g_frames >= g_termAt; }

int main()
{
    static CGameScriptInterface gsi;
    g_pGameScriptInterface = &gsi;
    bool ok = true;
    NScript::CQ_AmbushTradersScript s;
    std::memset(&s.GSI, 0, sizeof(s) - sizeof(void*) - sizeof(s._body));
    s.GSI = &gsi; s.SpeechBanditTimer.TimerIndex = 42; g_s = &s;

    // 1: terminated at entry: nothing happens
    g_frames = 0; g_termAt = 0; s.EnableSpeechBandit = true;
    s.WatchForBanditSpeechTimer();
    ok = ok && g_frames == 0 && s.EnableSpeechBandit && g_getTimerCalls == 0;

    // 2: speech enabled until frame 3, timer runs out at frame 5 -> speech re-enabled; allegiance switches on frame 8
    g_frames = 0; g_termAt = 1000; g_switchAt = 8; g_disableAt = 3; g_timerZeroFrom = 5; g_termCalls = 0;
    s.EnableSpeechBandit = true; s.BanditSwitchAllegienceComplete = false; g_prevEnable = true; g_getTimerCalls = 0;
    s.WatchForBanditSpeechTimer();
    ok = ok && s.BanditSwitchAllegienceComplete && g_frames == 8 && s.EnableSpeechBandit && g_getTimerCalls > 0 && !g_badTimer;

    // 3: speech disabled, timer never runs out, thread terminated on frame 4: speech stays disabled
    g_frames = 0; g_termAt = 4; g_switchAt = 1000; g_disableAt = 1000; g_timerZeroFrom = 1000;
    s.EnableSpeechBandit = false; s.BanditSwitchAllegienceComplete = false;
    s.WatchForBanditSpeechTimer();
    ok = ok && g_frames == 4 && !s.EnableSpeechBandit;

    // 4: allegiance already switched: the thread ends without waiting (two termination checks after the break)
    g_frames = 0; g_termAt = 1000; s.BanditSwitchAllegienceComplete = true; s.EnableSpeechBandit = true; g_termCalls = 0;
    s.WatchForBanditSpeechTimer();
    ok = ok && g_frames == 0 && s.EnableSpeechBandit && g_termCalls == 4;

    if (!ok) { std::printf("00f035e0_TEST FAIL\n"); return 1; }
    std::printf("00f035e0_TEST PASS\n");
    return 0;
}
