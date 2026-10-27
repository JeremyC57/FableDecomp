#include <cstdio>

// Behaviour model of PKCTimerManager::ThreadMain (0x00C259E0): until shutdown, tick and sleep 1000/rate ms.
static int g_sleeps = 0; static unsigned int g_lastSleep = 0;
struct Manager;
struct Target { Manager* m_pOwner; int m_ticks; void Tick(); };
struct Manager {
    bool m_bShutdown; Target* m_pTarget; float m_rate;
    void ThreadMain()
    {
        long period = (long)(1000.0f / m_rate);
        while (!m_bShutdown) { m_pTarget->Tick(); ++g_sleeps; g_lastSleep = (unsigned int)period; }
    }
};
void Target::Tick() { if (++m_ticks == 3) m_pOwner->m_bShutdown = true; }

int main()
{

    Manager m; Target t; t.m_pOwner = &m; t.m_ticks = 0;
    m.m_bShutdown = false; m.m_pTarget = &t; m.m_rate = 50.0f;
    m.ThreadMain();
    if (t.m_ticks != 3 || g_sleeps != 3 || g_lastSleep != 20) { std::printf("00c259e0_TEST FAIL\n"); return 1; }
    m.ThreadMain();
    if (t.m_ticks != 3) { std::printf("00c259e0_TEST FAIL\n"); return 1; }
    std::printf("00c259e0_TEST PASS\n");
    return 0;
}