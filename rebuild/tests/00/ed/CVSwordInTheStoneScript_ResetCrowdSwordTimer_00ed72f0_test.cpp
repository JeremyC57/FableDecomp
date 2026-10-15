#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
static int g_randNext = 0; static long g_timer = 0, g_seconds = 0; static int g_resets = 0;
static int my_rand() { return g_randNext; }
struct CGameScriptInterface { virtual void v000() { } virtual void ResetTimer(long timer, long seconds) { ++g_resets; g_timer = timer; g_seconds = seconds; } };
static CGameScriptInterface g_interface; CGameScriptInterface* g_pGameScriptInterface = &g_interface;
struct CV_SwordInTheStoneScript {
    virtual void v00() { } unsigned char _pad04[0x78]; long m_crowdSwordTimer;
    void ResetCrowdSwordTimer() { long seconds = my_rand() % 6 + 10; g_pGameScriptInterface->ResetTimer(m_crowdSwordTimer, seconds); }
};
int main()
{
    CV_SwordInTheStoneScript s; s.m_crowdSwordTimer = 42;
    g_randNext = 0; s.ResetCrowdSwordTimer(); if (g_resets != 1 || g_timer != 42 || g_seconds != 10) { std::printf("00ed72f0_TEST FAIL\n"); return 1; }
    g_randNext = 17; s.ResetCrowdSwordTimer(); if (g_resets != 2 || g_seconds != 15) { std::printf("00ed72f0_TEST FAIL\n"); return 1; }
    g_randNext = 12345; s.ResetCrowdSwordTimer(); if (g_seconds != 10 + 12345 % 6) { std::printf("00ed72f0_TEST FAIL\n"); return 1; }
    std::printf("00ed72f0_TEST PASS\n"); return 0;
}