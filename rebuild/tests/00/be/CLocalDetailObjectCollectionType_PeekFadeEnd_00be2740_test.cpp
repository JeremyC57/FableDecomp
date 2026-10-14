#include <cstdio>
#include <cstdlib>
#include <cstring>
static unsigned long g_mask = 0; static float g_dist = 0; struct CGen { float Modify(unsigned long m, float d) { g_mask = m; g_dist = d; return d * 2; } }; static CGen g_gen; static CGen* g_pGen = &g_gen;
struct CType { unsigned char _pad[0x20]; float m_fadeEnd; unsigned char _p[0x20]; unsigned long m_detailClass; float PeekFadeEnd() { return g_pGen->Modify(1 << m_detailClass, m_fadeEnd); } };

int main() {
    CType t; t.m_fadeEnd = 40.0f; t.m_detailClass = 3; if (t.PeekFadeEnd() != 80.0f || g_mask != 8 || g_dist != 40.0f) { std::printf("00be2740_TEST FAIL\n"); return 1; }
    std::printf("00be2740_TEST PASS\n"); return 0;
}