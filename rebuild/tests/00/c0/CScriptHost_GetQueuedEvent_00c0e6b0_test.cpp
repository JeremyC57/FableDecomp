// behaviour test for 00c0e6b0 (standalone copy of the model)
#include <cstdio>
#include <cstring>

struct CQueuedEvent { long m_type; long m_time; long m_data; };
static CQueuedEvent g_ev[2] = { {3, 100, 7}, {5, 200, 9} };
struct CEventList { CQueuedEvent* Find(const long& type) { for (int i = 0; i < 2; i++) if (g_ev[i].m_type == type) return &g_ev[i]; return 0; } };
struct CScriptHost { CEventList m_events;
    long GetQueuedEvent(long type, long* time, long* data) { CQueuedEvent* e = m_events.Find(type); if (!e) return -1; *time = e->m_time; *data = e->m_data; return 0; } };

int main()
{
    CScriptHost h; long t = 0, d = 0;
    if (!(h.GetQueuedEvent(5, &t, &d) == 0 && t == 200 && d == 9)) { std::printf("00c0e6b0_TEST FAIL\n"); return 1; }
    t = d = -5;
    if (!(h.GetQueuedEvent(4, &t, &d) == -1 && t == -5 && d == -5)) { std::printf("00c0e6b0_TEST FAIL\n"); return 1; }
    std::printf("00c0e6b0_TEST PASS\n");
    return 0;
}