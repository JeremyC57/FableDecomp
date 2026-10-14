#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_str = 0; static void* g_strAt[4];
struct CCharString { void* m_pData; CCharString() : m_pData(0) { g_strAt[g_str++] = this; } };
static int g_base = 0;
struct CShoveHaPennyRegionDef_Base { virtual void v00() { } unsigned char _pad[0x24]; CShoveHaPennyRegionDef_Base() { ++g_base; } };
struct CFireheartMinigameDef : CShoveHaPennyRegionDef_Base { CCharString m_name; CCharString m_text; virtual void v00() { } CFireheartMinigameDef() : CShoveHaPennyRegionDef_Base() { } };
int main() {
    CFireheartMinigameDef* d = new CFireheartMinigameDef; if (g_base != 1 || g_str != 2 || g_strAt[0] != &d->m_name || g_strAt[1] != &d->m_text) { std::printf("004d6638_TEST FAIL\n"); return 1; } delete d;
    std::printf("004d6638_TEST PASS\n"); return 0;
}
