// Behaviour test for 0x00C57F20 (standalone copy of the model and logic; the retail inline-asm block is modelled in C++)
#include <cstdio>
#include <cstring>

// round to nearest (the FPU default the fistp helper uses); the test values avoid exact .5 ties
static int RoundNearest(float v) { return (int)(v >= 0.0f ? v + 0.5f : v - 0.5f); }

void MixAddFloat(const float* src, int* dst) { for (int i = 0; i < 128; ++i) dst[i] += RoundNearest(src[i]); }
int main()
{
    float src[128]; int dst[128];
    for (int i = 0; i < 128; ++i) { src[i] = (float)(i - 64) * 1.3f; dst[i] = 10; }
    MixAddFloat(src, dst);
    if (dst[0] != 10 - 83 || dst[64] != 10 || dst[65] != 11 || dst[127] != 10 + 82) { std::printf("00c57f20_TEST FAIL\n"); return 1; }
    std::printf("00c57f20_TEST PASS\n");
    return 0;
}