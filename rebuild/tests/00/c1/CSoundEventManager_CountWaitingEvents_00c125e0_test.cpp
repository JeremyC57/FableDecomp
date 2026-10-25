// Behaviour test for 00c125e0: CSoundEventManager::CountWaitingEvents -- under the module lock, counts the active events that are waiting (state 2..4)
#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct CSoundEvent { long m_active; unsigned short m_state; long IsActive() { return m_active; } long IsWaiting() { return m_state == 2 || m_state == 3 || m_state == 4; } };
static CSoundEvent g_e[5];
struct CSoundEventPool { long a; unsigned long c; CSoundEvent* Get(unsigned long i) { return &g_e[i]; } };
struct CSoundEventManager { unsigned char p[0x5c]; CSoundEventPool m_events; unsigned long CountWaitingEvents() { L l(&g_g); unsigned long n = 0; for (unsigned long i = 0; i < m_events.c; ++i) { CSoundEvent* e = m_events.Get(i); if (e->IsActive() && e->IsWaiting()) ++n; } return n; } };
static bool Check()
{
    g_e[0].m_active = 1; g_e[0].m_state = 2; g_e[1].m_active = 1; g_e[1].m_state = 6; g_e[2].m_active = 0; g_e[2].m_state = 3;
    g_e[3].m_active = 1; g_e[3].m_state = 4; g_e[4].m_active = 1; g_e[4].m_state = 5;
    CSoundEventManager* m = new CSoundEventManager; m->m_events.c = 5;
    bool ok = m->CountWaitingEvents() == 2 && g_l == 0;
    delete m; return ok;
}

int main()
{
    if (!Check()) { std::printf("00c125e0_TEST FAIL\n"); return 1; }
    std::printf("00c125e0_TEST PASS\n");
    return 0;
}
