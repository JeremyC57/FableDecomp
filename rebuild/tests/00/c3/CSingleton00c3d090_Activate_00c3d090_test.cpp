#include <cstdio>
typedef void (*ActivateCallback)(void*);
static void* g_current = 0; static void* g_seen = 0; static int g_calls = 0;
static void cb(void* p) { ++g_calls; g_seen = p; }
struct CSingleton { unsigned char _pad[0x7c]; ActivateCallback m_pfnActivate;
    void Activate(void* inst) { if (g_current == 0) { g_current = inst; if (m_pfnActivate) m_pfnActivate(inst); } } };
int main() { int a, b; CSingleton s; s.m_pfnActivate = 0; s.Activate(&a); if (g_current != &a || g_calls) { std::printf("00c3d090_TEST FAIL\n"); return 1; }
    g_current = 0; s.m_pfnActivate = cb; s.Activate(&b); if (g_current != &b || g_calls != 1 || g_seen != &b) { std::printf("00c3d090_TEST FAIL\n"); return 1; }
    s.Activate(&a); if (g_current != &b || g_calls != 1) { std::printf("00c3d090_TEST FAIL\n"); return 1; } std::printf("00c3d090_TEST PASS\n"); return 0; }