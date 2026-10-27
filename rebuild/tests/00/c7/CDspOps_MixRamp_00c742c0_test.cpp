// Behaviour test for 0x00C742C0 CDspOps::MixRamp (standalone copy of the logic)
#include <cstdio>
#include <cmath>
static void MixRamp(float* dst, const float* src, float gain, float step, unsigned int count)
{
    for (unsigned int i = 0; i < count; ++i) {
        *dst++ += *src++ * gain;
        gain += step;
    }
}
int main()
{
    float src[6], dst[6];
    for (int i = 0; i < 6; ++i) { src[i] = 2.0f; dst[i] = (float)i; }
    MixRamp(dst, src, 1.0f, -0.5f, 5);
    for (int i = 0; i < 5; ++i)
        if (std::fabs(dst[i] - ((float)i + 2.0f * (1.0f - 0.5f * i))) > 1e-4f) { std::printf("00c742c0_TEST FAIL\n"); return 1; }
    if (dst[5] != 5.0f) { std::printf("00c742c0_TEST FAIL\n"); return 1; }
    std::printf("00c742c0_TEST PASS\n");
    return 0;
}