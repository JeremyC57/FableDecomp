// Behaviour test for 00c60410: CEaxContext::ClearEvents -- every queued event is unlinked and freed in order
#include <cstdio>
#include <cstdlib>
static int g_freed = 0; static void* g_order[4];
struct SEaxEvent { unsigned long m_flags, m_type, m_environment; SEaxEvent* m_pNext; };
static void PKFree(void* p) { g_order[g_freed++] = p; }
class CEaxContext {
public:
    SEaxEvent* m_pEvents;
    void ClearEvents() { while (m_pEvents != 0) { SEaxEvent* ev = m_pEvents; m_pEvents = ev->m_pNext; PKFree(ev); } }
};
int main()
{
    SEaxEvent a, b, c; a.m_pNext = &b; b.m_pNext = &c; c.m_pNext = 0;
    CEaxContext ctx; ctx.m_pEvents = &a; ctx.ClearEvents();
    int ok = ctx.m_pEvents == 0 && g_freed == 3 && g_order[0] == &a && g_order[1] == &b && g_order[2] == &c;
    ctx.ClearEvents(); ok = ok && g_freed == 3;
    std::printf(ok ? "00c60410_TEST PASS\n" : "00c60410_TEST FAIL\n");
    return ok ? 0 : 1;
}
