#include <cstdio>
// CDspOps::Add: dst[i] += src[i].
static void Add(float* dst, const float* src, unsigned int count)
{
    for (unsigned int i = 0; i < count; ++i)
        *dst++ += *src++;
}
int main()
{
    float s[5] = { 1, 2, 3, 4, 5 }, d[6] = { 10, 20, 30, 40, 50, 60 };
    Add(d, s, 5);
    for (int i = 0; i < 5; ++i) if (d[i] != 11.0f * (i + 1)) { std::printf("00c741c0_TEST FAIL\n"); return 1; }
    if (d[5] != 60.0f) { std::printf("00c741c0_TEST FAIL\n"); return 1; }
    std::printf("00c741c0_TEST PASS\n");
    return 0;
}