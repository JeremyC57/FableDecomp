#include <cstdio>
#include <cstdlib>
#include <cstring>
struct C3DVector_006b8730 { float X, Y, Z; C3DVector_006b8730() { } C3DVector_006b8730(float x, float y, float z) : X(x), Y(y), Z(z) { } };
static long g_a, g_b, g_c, g_d, g_e, g_f; static C3DVector_006b8730 g_s, g_r;
struct CEnvironmentSpellEffectInterpolator {
    void RadialBlurFadeTo(long a, long b, const C3DVector_006b8730& s, long c, const C3DVector_006b8730& r, long d, long e, long f) { g_a = a; g_b = b; g_s = s; g_c = c; g_r = r; g_d = d; g_e = e; g_f = f; }
    void RadialBlurFadeTo(long a, long b, float s, long c, float r, long d, long e, long f) { RadialBlurFadeTo(a, b, C3DVector_006b8730(s, s, s), c, C3DVector_006b8730(r, r, r), d, e, f); } };
int main() {
    CEnvironmentSpellEffectInterpolator e; e.RadialBlurFadeTo(1, 2, 0.5f, 3, 2.0f, 4, 5, 6);
    if (g_a != 1 || g_b != 2 || g_s.X != 0.5f || g_s.Y != 0.5f || g_s.Z != 0.5f || g_c != 3 || g_r.X != 2.0f || g_r.Z != 2.0f || g_d != 4 || g_e != 5 || g_f != 6) { std::printf("006b8730_TEST FAIL\n"); return 1; }
    std::printf("006b8730_TEST PASS\n"); return 0;
}