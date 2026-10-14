#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_a = 10, g_b = 10, g_c = 10; struct SInst { unsigned char _pad[0x14]; bool m_flagA; bool m_flagB; ~SInst() { --g_a; if (m_flagA) --g_b; if (m_flagB) --g_c; } };

int main()
{
    SInst* p = new SInst; p->m_flagA = true; p->m_flagB = false; delete p; if (g_a != 9 || g_b != 9 || g_c != 10) { std::printf("00a205c0_TEST FAIL\n"); return 1; }
    p = new SInst; p->m_flagA = false; p->m_flagB = true; delete p; if (g_a != 8 || g_b != 9 || g_c != 9) { std::printf("00a205c0_TEST FAIL\n"); return 1; }
    std::printf("00a205c0_TEST PASS\n");
    return 0;
}
