#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[8] = ""; static void* g_h[2]; static long g_aa = -1, g_af = -1, g_sf = -1; static float g_strength = 0;
struct CThingVtable_00d371f0 { virtual void StartEffect(void* h, float s, long f) { std::strcat(g_seq, "S"); g_h[1] = h; g_strength = s; g_sf = f; } };
struct CThingVtable2_00d371f0 : CThingVtable_00d371f0 { virtual void AttachEffect(void* h, long a, long f) { std::strcat(g_seq, "A"); g_h[0] = h; g_aa = a; g_af = f; } };
struct CFH_Scythe { long m_0; CThingVtable2_00d371f0* m_pThing; unsigned char m_handle[4]; void Init() { m_pThing->AttachEffect(m_handle, 0, 1); m_pThing->StartEffect(m_handle, 0.7f, 1); } };
int main() {
    CThingVtable2_00d371f0 t; CFH_Scythe s; s.m_pThing = &t; s.Init(); if (std::strcmp(g_seq, "AS") || g_h[0] != s.m_handle || g_h[1] != s.m_handle || g_aa != 0 || g_af != 1 || g_strength != 0.7f || g_sf != 1) { std::printf("00d371f0_TEST FAIL\n"); return 1; }
    std::printf("00d371f0_TEST PASS\n"); return 0;
}
