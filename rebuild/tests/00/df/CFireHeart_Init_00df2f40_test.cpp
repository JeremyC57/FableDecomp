#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[8] = ""; static void* g_h[2]; static long g_aa = -1, g_af = -1, g_sv = -1;
struct CThingVtable_00df2f40 { virtual void SetEffectParam(void* h, long v) { std::strcat(g_seq, "S"); g_h[1] = h; g_sv = v; } virtual void AttachEffect(void* h, long a, long f) { std::strcat(g_seq, "A"); g_h[0] = h; g_aa = a; g_af = f; } };
struct CFireHeart { long m_0; CThingVtable_00df2f40* m_pThing; unsigned char m_handle[4]; void Init() { m_pThing->AttachEffect(m_handle, 0, 1); m_pThing->SetEffectParam(m_handle, 0); } };
int main() {
    CThingVtable_00df2f40 t; CFireHeart f; f.m_pThing = &t; f.Init(); if (std::strcmp(g_seq, "AS") || g_h[0] != f.m_handle || g_h[1] != f.m_handle || g_aa != 0 || g_af != 1 || g_sv != 0) { std::printf("00df2f40_TEST FAIL\n"); return 1; }
    std::printf("00df2f40_TEST PASS\n"); return 0;
}