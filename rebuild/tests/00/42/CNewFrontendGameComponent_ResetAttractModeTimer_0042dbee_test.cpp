#include <stdio.h>
static const void* g_this; static int g_calls;
struct CStopWatch { double start; void Reset() { g_this = this; ++g_calls; start = 0; } };
struct CNewFrontendGameComponent {
    unsigned char _pad00[0x100];
    CStopWatch    m_attractModeTimer;
    void ResetAttractModeTimer();
};

void CNewFrontendGameComponent::ResetAttractModeTimer()
{
    m_attractModeTimer.Reset();
}

int main()
{
    static CNewFrontendGameComponent c; c.m_attractModeTimer.start = 5;
    c.ResetAttractModeTimer();
    bool ok = g_calls == 1 && g_this == (const void*)((char*)&c + 0x100) && c.m_attractModeTimer.start == 0;
    c.ResetAttractModeTimer(); ok = ok && g_calls == 2;
    printf(ok ? "0042dbee_TEST PASS\n" : "0042dbee_TEST FAIL\n");
    return ok ? 0 : 1;
}