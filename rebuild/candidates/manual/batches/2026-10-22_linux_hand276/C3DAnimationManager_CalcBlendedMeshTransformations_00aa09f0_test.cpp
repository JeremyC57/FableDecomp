#include <cstdio>
// Behaviour copy of C3DAnimationManager::CalcBlendedMeshTransformations (0x00AA09F0): 4x4 blend, narrowed to 3x4.
struct M4 { float r[4][4]; };
struct M3 { float r[4][3]; };
static int g_freeAll = 0;
static void Blend4x4(M4* out, unsigned long count) { for (unsigned long i = 0; i < count; ++i) for (int a = 0; a < 4; ++a) for (int b = 0; b < 4; ++b) out[i].r[a][b] = (float)(i * 100 + a * 10 + b); }
static void CalcBlended(unsigned long count, M3* out) {
    M4 pool[16];
    M4* blended = count ? pool : 0;
    Blend4x4(blended, count);
    for (unsigned long i = 0; i < count; ++i) {
        M3& dest = out[i]; const M4& src = blended[i];
        for (int a = 0; a < 4; ++a) { dest.r[a][0] = src.r[a][0]; dest.r[a][1] = src.r[a][1]; dest.r[a][2] = src.r[a][2]; }
    }
    ++g_freeAll;
}
int main() {
    M3 out[7];
    CalcBlended(7, out);
    bool ok = out[0].r[0][0] == 0.0f && out[6].r[3][2] == 632.0f && out[5].r[1][1] == 511.0f && out[2].r[2][0] == 220.0f && g_freeAll == 1;
    CalcBlended(0, out);
    ok = ok && g_freeAll == 2;
    if (!ok) { std::printf("00aa09f0_TEST FAIL\n"); return 1; }
    std::printf("00aa09f0_TEST PASS\n"); return 0;
}
