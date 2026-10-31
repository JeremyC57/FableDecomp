// behaviour test for GFFloatRandom 0x004978A0 (32-bit seed modelled as unsigned int for the 64-bit g++ host)
#include <cstdio>
#include <cmath>
typedef unsigned int u32;
static int GFROR13(u32 v) { return (int)((v >> 13) | (v << 19)); }
static float GFFloatRandom(float range, u32* seed)
{
    double scaled = (double)range;
    *seed = *seed * 0x24A1 + 0x24DF;
    scaled *= 65536.0;
    *seed = GFROR13(*seed);
    if (scaled > 0.0001)
        return (float)(fmod((double)*seed, scaled) * (1.0 / 65536.0));
    return 0.0f;
}
int main()
{
    u32 s = 1;
    for (int i = 0; i < 1000; ++i) {
        float r = GFFloatRandom(10.0f, &s);
        if (r < 0.0f || r >= 10.0f) { std::printf("004978a0_TEST FAIL\n"); return 1; }
    }
    u32 z = 5;
    if (GFFloatRandom(0.0f, &z) != 0.0f) { std::printf("004978a0_TEST FAIL\n"); return 1; }
    u32 e = 5u * 0x24A1u + 0x24DFu; e = (e >> 13) | (e << 19);
    if (z != e) { std::printf("004978a0_TEST FAIL\n"); return 1; }
    u32 w = 7; float f = GFFloatRandom(2.0f, &w);
    u32 e2 = 7u * 0x24A1u + 0x24DFu; e2 = (e2 >> 13) | (e2 << 19);
    if (w != e2 || std::fabs(f - (float)(std::fmod((double)e2, 131072.0) / 65536.0)) > 1e-6f) { std::printf("004978a0_TEST FAIL\n"); return 1; }
    std::printf("004978a0_TEST PASS\n");
    return 0;
}