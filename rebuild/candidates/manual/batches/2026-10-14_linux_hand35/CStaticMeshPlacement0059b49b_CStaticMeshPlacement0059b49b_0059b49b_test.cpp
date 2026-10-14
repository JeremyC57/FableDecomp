#include <cstdio>
struct S16_0059b49b { float m_x, m_y, m_z; unsigned char m_flags[4]; };
struct CStaticMeshPlacement_0059b49b; static void* g_self = 0; static unsigned char g_flag = 0; static S16_0059b49b g_a, g_b, g_c;
static void InitPlacement_0059b2cc(CStaticMeshPlacement_0059b49b* self, unsigned char* flag, S16_0059b49b a, S16_0059b49b b, S16_0059b49b c) { g_self = self; g_flag = *flag; g_a = a; g_b = b; g_c = c; }
struct CStaticMeshPlacement_0059b49b { CStaticMeshPlacement_0059b49b(S16_0059b49b a, S16_0059b49b b, S16_0059b49b c) { InitPlacement_0059b2cc(this, &c.m_flags[3], a, b, c); } };
int main() {
    S16_0059b49b a = { 1, 2, 3, { 0, 0, 0, 0 } }, b = { 4, 5, 6, { 0, 0, 0, 0 } }, c = { 7, 8, 9, { 1, 2, 3, 4 } };
    CStaticMeshPlacement_0059b49b p(a, b, c); if (g_self != &p || g_flag != 4 || g_a.m_x != 1 || g_b.m_y != 5 || g_c.m_z != 9) { std::printf("0059b49b_TEST FAIL\n"); return 1; }
    std::printf("0059b49b_TEST PASS\n"); return 0;
}
