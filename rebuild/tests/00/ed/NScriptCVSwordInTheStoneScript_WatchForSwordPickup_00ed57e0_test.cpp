#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
static int g_termCalls = 0, g_termAt = 0, g_waits = 0, g_yields = 0, g_cutscenes = 0; static float g_waitSeconds = 0;
class CGameScriptInterface { public: virtual void v000() { } virtual void YieldFrame() { ++g_yields; } virtual void Wait(float seconds) { ++g_waits; g_waitSeconds = seconds; } };
struct CV_SwordInTheStoneScript {
    virtual void v00() { }
    unsigned char _pad04[0x3c]; CGameScriptInterface* m_pInterface; unsigned char _pad44[8]; unsigned char m_crowdGathered;
    unsigned char IsActiveThreadTerminating() { ++g_termCalls; return g_termCalls >= g_termAt; }
    void SwordPullCutscene() { ++g_cutscenes; m_crowdGathered = 0; }
    void WatchForSwordPickup()
    {
        while (!IsActiveThreadTerminating()) {
            if (m_crowdGathered) { if (IsActiveThreadTerminating()) return; m_pInterface->Wait(0.2f); SwordPullCutscene(); }
            m_pInterface->YieldFrame();
        }
    }
};
int main()
{
    CGameScriptInterface iface; CV_SwordInTheStoneScript s; s.m_pInterface = &iface;
    s.m_crowdGathered = 0; g_termAt = 1; s.WatchForSwordPickup();                      // terminating at once: nothing runs
    if (g_termCalls != 1 || g_yields != 0 || g_waits != 0) { std::printf("00ed57e0_TEST FAIL\n"); return 1; }
    g_termCalls = 0; g_termAt = 4; s.m_crowdGathered = 1; s.WatchForSwordPickup();   // entry ok, inner ok, cutscene, yield, bottom test ok, crowd clear -> yield, bottom test terminates
    if (g_cutscenes != 1 || g_waits != 1 || g_waitSeconds != 0.2f || g_yields != 2 || g_termCalls != 4 || s.m_crowdGathered != 0) { std::printf("00ed57e0_TEST FAIL\n"); return 1; }
    g_termCalls = 0; g_termAt = 2; s.m_crowdGathered = 1; s.WatchForSwordPickup();   // the inner test terminates before the wait
    if (g_cutscenes != 1 || g_waits != 1 || g_yields != 2 || g_termCalls != 2) { std::printf("00ed57e0_TEST FAIL\n"); return 1; }
    std::printf("00ed57e0_TEST PASS\n"); return 0;
}