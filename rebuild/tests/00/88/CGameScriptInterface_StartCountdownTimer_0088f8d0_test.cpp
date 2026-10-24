#include <cstdio>
long g_framesPerSecond = 30;
static long g_started = -1;
class CWorld { public: void StartCountdownTimer(long frames) { g_started = frames; } };
class CGameScriptInterface { public: virtual void StartCountdownTimer(float seconds) const; CWorld* m_pWorld; };
void CGameScriptInterface::StartCountdownTimer(float seconds) const
{
    seconds = g_framesPerSecond * seconds;
    long frames;
    __asm fld seconds
    __asm fistp frames
    m_pWorld->StartCountdownTimer(frames);
}
int main()
{
    CWorld w; CGameScriptInterface g; g.m_pWorld = &w;
    g.StartCountdownTimer(2.5f); bool ok = g_started == 75;
    g.StartCountdownTimer(0.01f); ok = ok && g_started == 0;       // 0.3 rounds to 0
    g.StartCountdownTimer(1.025f); ok = ok && g_started == 31;     // 30.75 rounds to 31
    std::printf(ok ? "0088f8d0_TEST PASS\n" : "0088f8d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
