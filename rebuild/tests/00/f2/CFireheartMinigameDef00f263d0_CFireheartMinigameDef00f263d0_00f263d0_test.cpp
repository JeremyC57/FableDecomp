#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_str = 0; static void* g_strAt[4];
struct CCharString { void* m_pData; CCharString() : m_pData(0) { g_strAt[g_str++] = this; } };
static int g_base = 0;
struct CPSCBase { virtual void v00() { } unsigned char _pad[0x24]; CPSCBase() { ++g_base; } };
struct CFireheartMinigameDef_00f263d0 : CPSCBase { CCharString m_name; long m_2c; CCharString m_text; virtual void v00() { } CFireheartMinigameDef_00f263d0() : CPSCBase() { } };
int main() {
    CFireheartMinigameDef_00f263d0* d = new CFireheartMinigameDef_00f263d0; if (g_base != 1 || g_str != 2 || g_strAt[0] != &d->m_name || g_strAt[1] != &d->m_text) { std::printf("00f263d0_TEST FAIL\n"); return 1; } delete d;
    std::printf("00f263d0_TEST PASS\n"); return 0;
}