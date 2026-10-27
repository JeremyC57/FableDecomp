// Behaviour test for 0x00C74130 CDspOps::ScaleRampCopy (standalone copy of the logic)
#include <cstdio>
#include <cmath>
static void ScaleRampCopy(float* dst, const float* src, float gain, float step, unsigned int count)
{
    for (unsigned int i = 0; i < count; ++i) {
        *dst++ = *src++ * gain;
        gain += step;
    }
}
int main()
{
    float src[7], dst[8];
    for (int i = 0; i < 7; ++i) src[i] = (float)(i + 1);
    dst[7] = -5.0f;
    ScaleRampCopy(dst, src, 0.5f, 0.25f, 7);
    for (int i = 0; i < 7; ++i) {
        float want = (float)(i + 1) * (0.5f + 0.25f * i);
        if (std::fabs(dst[i] - want) > 1e-4f) { std::printf("00c74130_TEST FAIL\n"); return 1; }
    }
    if (dst[7] != -5.0f) { std::printf("00c74130_TEST FAIL\n"); return 1; }
    std::printf("00c74130_TEST PASS\n");
    return 0;
}