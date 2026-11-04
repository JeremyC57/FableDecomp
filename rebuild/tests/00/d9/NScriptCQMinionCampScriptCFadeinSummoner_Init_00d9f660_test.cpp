#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_h = 0; static long g_phase = -1, g_flag = -1; static bool g_done = false;
struct CThingVtable_00d9f660 { virtual void StartEffect(void* h, long p, long f) { g_h = h; g_phase = p; g_flag = f; } };
struct CFadeinSummoner { long m_0; CThingVtable_00d9f660* m_pThing; unsigned char m_handle[4]; unsigned char _pad0c[0x10]; long m_phase; float m_rate; bool m_finished; bool IsDone() { return g_done; }
    void Init_00d9f660() { m_phase = 0; if (!IsDone()) { m_rate = -0.00138888880610466f; m_finished = false; m_pThing->StartEffect(m_handle, m_phase, 1); } } };
int main() {
    CThingVtable_00d9f660 t; CFadeinSummoner s; std::memset(&s, 0, sizeof s); s.m_pThing = &t; s.m_phase = 5; s.m_finished = true; s.Init_00d9f660();
    if (s.m_phase != 0 || s.m_rate != -0.00138888880610466f || s.m_finished || g_h != s.m_handle || g_phase != 0 || g_flag != 1) { std::printf("00d9f660_TEST FAIL\n"); return 1; }
    g_done = true; s.m_phase = 5; s.m_rate = 0; g_h = 0; s.Init_00d9f660(); if (s.m_phase != 0 || s.m_rate != 0 || g_h != 0) { std::printf("00d9f660_TEST FAIL\n"); return 1; }
    std::printf("00d9f660_TEST PASS\n"); return 0;
}