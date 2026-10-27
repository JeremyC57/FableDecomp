// Behaviour test for 0x00C6B5D0 (standalone copy of the logic; the retail SSE2/SSE inline-asm blocks are modelled in C++ by the same int-to-float conversion as the x87 fallback)
#include <cstdio>
#include <cstring>

static void ConvertInt128BlockToFloatInPlace(float* block)
{
    for (int i = 0; i < 128; ++i) {
        int v; std::memcpy(&v, &block[i], sizeof(v));
        block[i] = (float)v;
    }
}
int main()
{
    float block[128];
    for (int i = 0; i < 128; ++i) { int v = (i - 64) * 1000; std::memcpy(&block[i], &v, sizeof(v)); }
    ConvertInt128BlockToFloatInPlace(block);
    if (block[0] != -64000.0f || block[64] != 0.0f || block[65] != 1000.0f || block[127] != 63000.0f) { std::printf("00c6b5d0_TEST FAIL\n"); return 1; }
    std::printf("00c6b5d0_TEST PASS\n");
    return 0;
}