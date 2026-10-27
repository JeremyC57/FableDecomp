// Behaviour test for 0x00C74230 CDspOps::Mix (standalone copy of the logic)
#include <cstdio>
#include <cmath>
static void Mix(float* dst, const float* src, float gain, unsigned int count)
{
    for (unsigned int i = 0; i < count; ++i)
        *dst++ += *src++ * gain;
}
int main()
{
    float src[9], dst[10];
    for (int i = 0; i < 9; ++i) { src[i] = (float)i; dst[i] = 1.0f; }
    dst[9] = 7.0f;
    Mix(dst, src, 2.0f, 9);
    for (int i = 0; i < 9; ++i)
        if (std::fabs(dst[i] - (1.0f + 2.0f * i)) > 1e-4f) { std::printf("00c74230_TEST FAIL\n"); return 1; }
    if (dst[9] != 7.0f) { std::printf("00c74230_TEST FAIL\n"); return 1; }
    std::printf("00c74230_TEST PASS\n");
    return 0;
}