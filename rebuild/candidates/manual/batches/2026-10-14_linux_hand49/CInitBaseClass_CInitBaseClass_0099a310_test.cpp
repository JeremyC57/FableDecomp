#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_count = 0; struct CBaseClass { virtual void v00() { } }; struct CInitBaseClass : CBaseClass { bool m_initialised; CInitBaseClass() : m_initialised(false) { ++g_count; } };

int main() {
    g_count = 5; CInitBaseClass a; if (a.m_initialised || g_count != 6) { std::printf("0099a310_TEST FAIL\n"); return 1; } CInitBaseClass b; if (g_count != 7) { std::printf("0099a310_TEST FAIL\n"); return 1; }
    std::printf("0099a310_TEST PASS\n"); return 0;
}
