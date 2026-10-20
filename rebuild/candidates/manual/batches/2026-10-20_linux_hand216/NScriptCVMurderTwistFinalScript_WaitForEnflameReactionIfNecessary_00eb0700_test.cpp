// Behaviour test for 0x00EB0700 CV_MurderTwistFinalScript::WaitForEnflameReactionIfNecessary: no-op when the thing was
// not set alight; otherwise will disabled, 3-second timer waited frame by frame, will re-enabled, timer deregistered.
#include <cstdio>
struct CScriptThing { bool enflamed; };
struct CGameScriptInterface {
    long timeLeft; int registered, deregistered, frames; int willCalls; bool willUsable;
    virtual void NewScriptFrame() { ++frames; --timeLeft; }
    virtual long RegisterTimer() { ++registered; return 5; }
    virtual void DeregisterTimer(long timer) { if (timer == 5) ++deregistered; }
    virtual void SetTimer(long timer, long seconds) { if (timer == 5) timeLeft = seconds; }
    virtual long GetTimer(long timer) { return timer == 5 ? timeLeft : -99; }
    virtual void SetHeroWillAsUsable(bool usable) { ++willCalls; willUsable = usable; }
};
static CGameScriptInterface g_iface;
static CGameScriptInterface* g_pGameScriptInterface = &g_iface;
struct CTimer {
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(Index); }
    void SetTime(long seconds) { g_pGameScriptInterface->SetTimer(Index, seconds); }
    long GetTime() { return g_pGameScriptInterface->GetTimer(Index); }
};
struct CV_MurderTwistFinalScript {
    CGameScriptInterface* Game; int checks, terminateAt;
    bool IsActiveThreadTerminating() { return ++checks == terminateAt; }
    bool HasHeroEnflamedEntity(const CScriptThing& t) { return t.enflamed; }
    void WaitForEnflameReactionIfNecessary(const CScriptThing& thing)
    {
        if (!HasHeroEnflamedEntity(thing)) return;
        if (IsActiveThreadTerminating()) return;
        Game->SetHeroWillAsUsable(false);
        CTimer timer;
        timer.SetTime(3);
        while (timer.GetTime() > 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->SetHeroWillAsUsable(true);
    }
};
static void Reset(CV_MurderTwistFinalScript& s, int term)
{
    CGameScriptInterface fresh; g_iface = fresh;
    g_iface.timeLeft = 0; g_iface.registered = g_iface.deregistered = g_iface.frames = g_iface.willCalls = 0; g_iface.willUsable = true;
    s.Game = &g_iface; s.checks = 0; s.terminateAt = term;
}
int main()
{
    CV_MurderTwistFinalScript s; CScriptThing cold = { false }, hot = { true };
    Reset(s, 100); s.WaitForEnflameReactionIfNecessary(cold);
    bool ok = g_iface.registered == 0 && g_iface.willCalls == 0 && s.checks == 0;
    Reset(s, 100); s.WaitForEnflameReactionIfNecessary(hot);
    ok = ok && g_iface.frames == 3 && g_iface.registered == 1 && g_iface.deregistered == 1 && g_iface.willCalls == 2 && g_iface.willUsable;
    Reset(s, 3); s.WaitForEnflameReactionIfNecessary(hot);      // terminates inside the wait
    ok = ok && g_iface.frames == 2 && g_iface.deregistered == 1 && g_iface.willCalls == 1 && !g_iface.willUsable;
    if (!ok) { std::printf("00eb0700_TEST FAIL\n"); return 1; }
    std::printf("00eb0700_TEST PASS\n");
    return 0;
}
