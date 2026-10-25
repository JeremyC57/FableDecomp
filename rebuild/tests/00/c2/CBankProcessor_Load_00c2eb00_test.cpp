// Behaviour test for 00c2eb00: CBankProcessor::Load -- the steps run in order and stop at the first failed status check
#include <cstdio>
struct CLoadLog { bool m_ok; int checks; int failAfter; bool IsOk() { ++checks; if (checks > failAfter) m_ok = false; return m_ok; } };
static char g_log[16]; static int g_n = 0;
struct CBankData { bool Prepare() { g_log[g_n++] = 'P'; return true; } };
struct CBankProcessor {
    CBankData* m_pData; void* m_pManager; CLoadLog* m_pLog;
    void AddSamples() { g_log[g_n++] = '1'; }
    void AddDrivers() { g_log[g_n++] = '2'; }
    void AddNumberTriggers() { g_log[g_n++] = '3'; }
    void ProcessAllDrivers() { g_log[g_n++] = '4'; }
    void ProcessEventTriggers() { g_log[g_n++] = '5'; }
    void AddEvents() { g_log[g_n++] = '6'; }
    void AddSubEvents() { g_log[g_n++] = '7'; }
    void Load()
    {
        if (!m_pLog->IsOk()) return;
        m_pData->Prepare(); AddSamples();
        if (!m_pLog->IsOk()) return;
        AddDrivers();
        if (!m_pLog->IsOk()) return;
        AddNumberTriggers();
        if (!m_pLog->IsOk()) return;
        ProcessAllDrivers();
        if (!m_pLog->IsOk()) return;
        ProcessEventTriggers();
        if (!m_pLog->IsOk()) return;
        AddEvents();
        if (!m_pLog->IsOk()) return;
        AddSubEvents();
        if (!m_pLog->IsOk()) return;
    }
};
int main()
{
    CBankData src; CLoadLog st; st.m_ok = true; st.checks = 0; st.failAfter = 100;
    CBankProcessor l; l.m_pData = &src; l.m_pManager = 0; l.m_pLog = &st;
    l.Load(); g_log[g_n] = 0;
    if (g_n != 8 || g_log[0] != 'P' || g_log[1] != '1' || g_log[7] != '7' || st.checks != 8) { std::printf("00c2eb00_TEST FAIL\n"); return 1; }
    g_n = 0; st.m_ok = true; st.checks = 0; st.failAfter = 3;
    l.Load(); g_log[g_n] = 0;
    if (g_n != 4 || g_log[3] != '3' || st.checks != 4) { std::printf("00c2eb00_TEST FAIL\n"); return 1; }
    std::printf("00c2eb00_TEST PASS\n");
    return 0;
}