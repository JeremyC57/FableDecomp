#include <cstdio>
#include <cmath>
// vorbis_invsq2explook: INVSQ2EXP_LOOKUP[a + 32] = 2^(-a/2), a in -32..32.
static float INVSQ2EXP_LOOKUP[65];
static float vorbis_invsq2explook(int a) { return INVSQ2EXP_LOOKUP[a - (-32)]; }
int main()
{
    for (int i = 0; i < 65; ++i) INVSQ2EXP_LOOKUP[i] = (float)std::pow(2.0, -(i - 32) / 2.0);
    if (vorbis_invsq2explook(-32) != 65536.0f || vorbis_invsq2explook(0) != 1.0f || vorbis_invsq2explook(2) != 0.5f) {
        std::printf("00ca3ef0_TEST FAIL\n"); return 1;
    }
    if (std::fabs(vorbis_invsq2explook(1) - 0.70710678f) > 1e-6f) { std::printf("00ca3ef0_TEST FAIL\n"); return 1; }
    std::printf("00ca3ef0_TEST PASS\n");
    return 0;
}