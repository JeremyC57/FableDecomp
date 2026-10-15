#include <cstdio>
// Behaviour of CTCJackPillar::OnInitialActivate (0x007319A0).
struct CEngineGlobals { bool m_answer; virtual bool QuerySlot9() { return m_answer; } };
CEngineGlobals g_engine;
CEngineGlobals* g_pEngineGlobals = &g_engine;
struct CTCJackPillar {
    long m_enableCalls; bool m_enabled;
    void Enable(bool enable) { ++m_enableCalls; m_enabled = enable; }
    void OnInitialActivate()
    {
        if (!g_pEngineGlobals->QuerySlot9())
            Enable(false);
    }
};
int main()
{
    CTCJackPillar p; p.m_enableCalls = 0; p.m_enabled = true;
    g_engine.m_answer = true; p.OnInitialActivate();
    int bad = (p.m_enableCalls != 0 || !p.m_enabled);
    g_engine.m_answer = false; p.OnInitialActivate();
    bad |= (p.m_enableCalls != 1 || p.m_enabled);
    if (bad) { std::printf("007319a0_TEST FAIL\n"); return 1; }
    std::printf("007319a0_TEST PASS\n");
    return 0;
}