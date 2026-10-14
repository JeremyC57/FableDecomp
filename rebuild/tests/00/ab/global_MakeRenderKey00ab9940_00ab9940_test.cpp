#include <cstdio>
struct SKey4 { long m_a, m_b, m_c, m_d; }; struct SRenderKey { long m_a, m_b, m_c, m_d, m_e, m_f, m_g; };
static SRenderKey MakeRenderKey(const SKey4* o, long e, long f, long g) { SRenderKey k; k.m_a = o->m_a; k.m_b = o->m_b; k.m_c = o->m_c; k.m_d = o->m_d; k.m_e = e; k.m_f = f; k.m_g = g; return k; }
int main() { SKey4 o = { 1, 2, 3, 4 }; SRenderKey k = MakeRenderKey(&o, 5, 6, 7); if (k.m_a != 1 || k.m_d != 4 || k.m_e != 5 || k.m_f != 6 || k.m_g != 7) { std::printf("00ab9940_TEST FAIL\n"); return 1; } std::printf("00ab9940_TEST PASS\n"); return 0; }