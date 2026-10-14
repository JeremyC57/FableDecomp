#include <cstdio>
static int g_idle = 0, g_busy = 0; static void* g_a = 0; static void* g_b = 0;
struct CNav { unsigned char _pad[0x10]; unsigned long m_flags; unsigned char _pad14[0x18]; void* m_pCurrent; void* m_pPending;
    void UpdateIdle(void* a, void* b) { ++g_idle; g_a = a; g_b = b; } void UpdateBusy(void* a, void* b) { ++g_busy; g_a = a; g_b = b; } };
static void Dispatch(CNav* n, void* a, void* b) { if (n->m_flags & 0x13) { if (n->m_pCurrent == 0 && n->m_pPending == 0) n->UpdateIdle(a, b); else n->UpdateBusy(a, b); } }
int main()
{
    int x, y; CNav n; n.m_flags = 0x4; n.m_pCurrent = 0; n.m_pPending = 0;
    Dispatch(&n, &x, &y); if (g_idle || g_busy) { std::printf("00c35c50_TEST FAIL\n"); return 1; }
    n.m_flags = 0x10; Dispatch(&n, &x, &y); if (g_idle != 1 || g_busy != 0 || g_a != &x || g_b != &y) { std::printf("00c35c50_TEST FAIL\n"); return 1; }
    n.m_flags = 0x2; n.m_pPending = &x; Dispatch(&n, &y, &x); if (g_idle != 1 || g_busy != 1 || g_a != &y) { std::printf("00c35c50_TEST FAIL\n"); return 1; }
    n.m_pPending = 0; n.m_pCurrent = &x; n.m_flags = 1; Dispatch(&n, &x, &y); if (g_busy != 2) { std::printf("00c35c50_TEST FAIL\n"); return 1; }
    std::printf("00c35c50_TEST PASS\n"); return 0;
}
