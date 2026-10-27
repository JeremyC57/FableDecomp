// Behaviour test for 0x00C57F60 (standalone copy of the model and logic; the retail inline-asm block is modelled in C++)
#include <cstdio>
#include <cstring>

// round to nearest (the FPU default the fistp helper uses); the test values avoid exact .5 ties
static int RoundNearest(float v) { return (int)(v >= 0.0f ? v + 0.5f : v - 0.5f); }

void MixAddFloatGain16(const float* src, int* dst, unsigned short gain)
{
    float scale = gain * (1.0f / 32768.0f);
    for (int i = 0; i < 128; ++i) dst[i] += RoundNearest(scale * src[i]);
}
int main()
{
    float src[128]; int dst[128];
    for (int i = 0; i < 128; ++i) { src[i] = (float)(i * 100 + 3); dst[i] = 1; }
    MixAddFloatGain16(src, dst, 0x4000);
    if (dst[0] != 1 + 2 || dst[1] != 1 + 52 || dst[127] != 1 + 6352) { std::printf("00c57f60_TEST FAIL\n"); return 1; }
    std::printf("00c57f60_TEST PASS\n");
    return 0;
}