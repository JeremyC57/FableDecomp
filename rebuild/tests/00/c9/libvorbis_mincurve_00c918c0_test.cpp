// Behaviour test for libvorbis psy.c min_curve (0x00C918C0): element-wise minimum of two 56-point curves into c.
#include <cstdio>
static void min_curve(float* c, float* c2)
{
    for (int i = 0; i < 56; i++) if (c2[i] < c[i]) c[i] = c2[i];
}
int main()
{
    float c[56], c2[56];
    for (int i = 0; i < 56; i++) { c[i] = (float)i; c2[i] = (float)(55 - i); }
    min_curve(c, c2);
    for (int i = 0; i < 56; i++) {
        float want = (c2[i] == c2[i]) ? ((float)i < (float)(55 - i) ? (float)i : (float)(55 - i)) : 0.0f;
        if (c[i] != want) { std::printf("00c918c0_TEST FAIL\n"); return 1; }
    }
    std::printf("00c918c0_TEST PASS\n"); return 0;
}