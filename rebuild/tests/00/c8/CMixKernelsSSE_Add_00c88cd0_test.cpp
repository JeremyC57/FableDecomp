// behaviour test for 0x00C88CD0 CMixKernelsSSE::Add: dst += src over count/4 whole float4 blocks, the remainder untouched
#include <cstdio>
struct F4 { float v[4]; };
static void Add(float* dst, const float* src, unsigned long count)
{
    F4* d = (F4*)dst;
    const F4* s = (const F4*)src;
    for (unsigned long i = 0; i < count / 4; i++) {
        for (int k = 0; k < 4; ++k) d->v[k] += s->v[k];
        d++; s++;
    }
}
int main()
{
    float dst[12], src[12];
    for (int i = 0; i < 12; ++i) { dst[i] = (float)i; src[i] = (float)(100 + i); }
    Add(dst, src, 10);                               // two blocks; samples 8..11 untouched
    for (int i = 0; i < 12; ++i) {
        float want = i < 8 ? (float)(i + 100 + i) : (float)i;
        if (dst[i] != want) { std::printf("00c88cd0_TEST FAIL\n"); return 1; }
    }
    Add(dst, src, 3);                                // zero blocks
    if (dst[0] != 100.0f) { std::printf("00c88cd0_TEST FAIL\n"); return 1; }
    std::printf("00c88cd0_TEST PASS\n");
    return 0;
}