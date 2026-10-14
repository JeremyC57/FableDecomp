#include <cstdio>
#include <cstdlib>
#include <cstring>
struct C3DVector_008853a0 { float X, Y, Z; C3DVector_008853a0() { } C3DVector_008853a0(float x, float y, float z) : X(x), Y(y), Z(z) { } };
static long g_a, g_b, g_c, g_d, g_e; static C3DVector_008853a0 g_s, g_r;
struct CEnvironmentSpellEffectInterface {
    void SetRadialBlurParams(const C3DVector_008853a0& s, long a, const C3DVector_008853a0& r, long b, long c, long d, long e) { g_s = s; g_a = a; g_r = r; g_b = b; g_c = c; g_d = d; g_e = e; }
    void SetRadialBlurParams(float s, long a, float r, long b, long c, long d, long e) { SetRadialBlurParams(C3DVector_008853a0(s, s, s), a, C3DVector_008853a0(r, r, r), b, c, d, e); } };
int main() {
    CEnvironmentSpellEffectInterface e; e.SetRadialBlurParams(0.5f, 3, 2.0f, 4, 5, 6, 7);
    if (g_s.X != 0.5f || g_s.Y != 0.5f || g_s.Z != 0.5f || g_a != 3 || g_r.X != 2.0f || g_r.Y != 2.0f || g_r.Z != 2.0f || g_b != 4 || g_c != 5 || g_d != 6 || g_e != 7) { std::printf("008853a0_TEST FAIL\n"); return 1; }
    std::printf("008853a0_TEST PASS\n"); return 0;
}
