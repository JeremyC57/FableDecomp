#include <cstdio>
// CDspOps::ScaleCopy: dst[i] = src[i] * gain.
static void ScaleCopy(float* dst, const float* src, float gain, unsigned int count)
{
    for (unsigned int i = 0; i < count; ++i)
        *dst++ = *src++ * gain;
}
int main()
{
    float s[5] = { 1, -2, 3, -4, 5 }, d[6] = { 0, 0, 0, 0, 0, 9 };
    ScaleCopy(d, s, 3.0f, 5);
    for (int i = 0; i < 5; ++i) if (d[i] != s[i] * 3.0f) { std::printf("00c74030_TEST FAIL\n"); return 1; }
    if (d[5] != 9.0f) { std::printf("00c74030_TEST FAIL\n"); return 1; }
    std::printf("00c74030_TEST PASS\n");
    return 0;
}