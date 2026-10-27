// Behaviour test for 0x00C57FC0 (standalone copy of the model and logic; the retail inline-asm block is modelled in C++)
#include <cstdio>
#include <cstring>

// round to nearest (the FPU default the fistp helper uses); the test values avoid exact .5 ties
static int RoundNearest(float v) { return (int)(v >= 0.0f ? v + 0.5f : v - 0.5f); }

void MixAddFloatGain(const float* src, int* dst, float gain) { for (int i = 0; i < 128; ++i) dst[i] += RoundNearest(src[i] * gain); }
int main()
{
    float src[128]; int dst[128];
    for (int i = 0; i < 128; ++i) { src[i] = (float)(i * 10 + 1); dst[i] = 0; }
    MixAddFloatGain(src, dst, 0.3f);
    if (dst[0] != 0 || dst[1] != 3 || dst[10] != 30 || dst[127] != 381) { std::printf("00c57fc0_TEST FAIL\n"); return 1; }
    std::printf("00c57fc0_TEST PASS\n");
    return 0;
}