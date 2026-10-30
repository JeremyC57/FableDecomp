// Behaviour test for libvorbis psy.c max_curve (0x00C91980): element-wise maximum of two 56-point curves into c.
#include <cstdio>
static void max_curve(float* c, float* c2)
{
    for (int i = 0; i < 56; i++) if (c2[i] > c[i]) c[i] = c2[i];
}
int main()
{
    float c[56], c2[56];
    for (int i = 0; i < 56; i++) { c[i] = (float)i; c2[i] = (float)(55 - i); }
    max_curve(c, c2);
    for (int i = 0; i < 56; i++) {
        float want = (c2[i] == c2[i]) ? ((float)i > (float)(55 - i) ? (float)i : (float)(55 - i)) : 0.0f;
        if (c[i] != want) { std::printf("00c91980_TEST FAIL\n"); return 1; }
    }
    std::printf("00c91980_TEST PASS\n"); return 0;
}