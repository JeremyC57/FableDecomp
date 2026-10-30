// Behaviour test for libvorbis attenuate_curve (0x00C91A40): adds att to all 56 points and nothing past them.
#include <cstdio>
#define EHMER_MAX 56
static void attenuate_curve(float* c, float att)
{
    int i;
    for (i = 0; i < EHMER_MAX; i++)
        c[i] += att;
}
int main()
{
    float c[EHMER_MAX + 2];
    int i;
    for (i = 0; i < EHMER_MAX + 2; i++) c[i] = (float)i;
    attenuate_curve(c, -2.5f);
    for (i = 0; i < EHMER_MAX; i++)
        if (c[i] != (float)i - 2.5f) { std::printf("00c91a40_TEST FAIL\n"); return 1; }
    if (c[EHMER_MAX] != (float)EHMER_MAX || c[EHMER_MAX + 1] != (float)(EHMER_MAX + 1)) { std::printf("00c91a40_TEST FAIL\n"); return 1; }
    std::printf("00c91a40_TEST PASS\n");
    return 0;
}