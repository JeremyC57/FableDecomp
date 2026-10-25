// Behaviour test for 00c2eb00: CChildListLoader::Load -- the steps run in order and stop at the first failed status check
#include <cstdio>
struct CLoadStatus { bool m_ok; int checks; int failAfter; bool IsOk() { ++checks; if (checks > failAfter) m_ok = false; return m_ok; } };
static char g_log[16]; static int g_n = 0;
struct CLoadSource { void Prepare() { g_log[g_n++] = 'P'; } };
struct CChildListLoader {
    CLoadSource* m_pSource; void* m_pTarget; CLoadStatus* m_pStatus;
    void AddChildren() { g_log[g_n++] = '1'; }
    void ProcessChunks() { g_log[g_n++] = '2'; }
    void ProcessChunkDrivers() { g_log[g_n++] = '3'; }
    void ProcessAllDrivers() { g_log[g_n++] = '4'; }
    void ProcessEventTriggers() { g_log[g_n++] = '5'; }
    void ProcessAllEvents() { g_log[g_n++] = '6'; }
    void ProcessAllEventMappings() { g_log[g_n++] = '7'; }
    void Load()
    {
        if (!m_pStatus->IsOk()) return;
        m_pSource->Prepare(); AddChildren();
        if (!m_pStatus->IsOk()) return;
        ProcessChunks();
        if (!m_pStatus->IsOk()) return;
        ProcessChunkDrivers();
        if (!m_pStatus->IsOk()) return;
        ProcessAllDrivers();
        if (!m_pStatus->IsOk()) return;
        ProcessEventTriggers();
        if (!m_pStatus->IsOk()) return;
        ProcessAllEvents();
        if (!m_pStatus->IsOk()) return;
        ProcessAllEventMappings();
        if (!m_pStatus->IsOk()) return;
    }
};
int main()
{
    CLoadSource src; CLoadStatus st; st.m_ok = true; st.checks = 0; st.failAfter = 100;
    CChildListLoader l; l.m_pSource = &src; l.m_pTarget = 0; l.m_pStatus = &st;
    l.Load(); g_log[g_n] = 0;
    if (g_n != 8 || g_log[0] != 'P' || g_log[1] != '1' || g_log[7] != '7' || st.checks != 8) { std::printf("00c2eb00_TEST FAIL\n"); return 1; }
    g_n = 0; st.m_ok = true; st.checks = 0; st.failAfter = 3;
    l.Load(); g_log[g_n] = 0;
    if (g_n != 4 || g_log[3] != '3' || st.checks != 4) { std::printf("00c2eb00_TEST FAIL\n"); return 1; }
    std::printf("00c2eb00_TEST PASS\n");
    return 0;
}