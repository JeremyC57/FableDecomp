#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_init = 0; static void InitializeCriticalSection(void* s) { g_init = s; }
struct CFireEffectCreationSet { virtual void v00() { } unsigned char m_section[0x18]; CFireEffectCreationSet() { InitializeCriticalSection(m_section); } };
int main() {
    CFireEffectCreationSet* s = new CFireEffectCreationSet; if (g_init != s->m_section) { std::printf("00c5d550_TEST FAIL\n"); return 1; } delete s;
    std::printf("00c5d550_TEST PASS\n"); return 0;
}