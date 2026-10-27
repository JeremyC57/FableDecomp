// Behaviour test for 0x00C5D490 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

static const float* g_src = 0; static int* g_dst = 0;
void AddFloat128BlockSSE2(const float* src, int* dst) { g_src = src; g_dst = dst; for (int i = 0; i < 128; ++i) dst[i] += (int)src[i]; }
struct V { virtual void MixAddFloat(const float* src, int* dst) { AddFloat128BlockSSE2(src, dst); } virtual ~V() {} };
int main()
{
    float src[128]; int dst[128];
    for (int i = 0; i < 128; ++i) { src[i] = (float)(i * 2); dst[i] = 1; }
    V v; v.MixAddFloat(src, dst);
    if (g_src != src || g_dst != dst || dst[5] != 11 || dst[127] != 255) { std::printf("00c5d490_TEST FAIL\n"); return 1; }
    std::printf("00c5d490_TEST PASS\n");
    return 0;
}