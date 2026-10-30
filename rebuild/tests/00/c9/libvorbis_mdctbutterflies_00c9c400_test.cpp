// Behaviour test for libvorbis mdct.c mdct_butterflies (0x00C9C400): stage dispatch (first / generic / 32-point) by log2n.
#include <cstdio>
struct mdct_lookup { int n; int log2n; float* trig; int* bitrev; float scale; };
static int g_first, g_generic, g_32, g_last_points, g_last_trigint;
static void mdct_butterfly_first(float*, float*, int points) { g_first++; g_last_points = points; }
static void mdct_butterfly_generic(float*, float*, int points, int trigint) { g_generic++; g_last_points = points; g_last_trigint = trigint; }
static void mdct_butterfly_32(float*) { g_32++; }
static void mdct_butterflies(mdct_lookup* init, float* x, int points)
{
    float* T = init->trig; int stages = init->log2n - 5;
    if (--stages > 0) mdct_butterfly_first(T, x, points);
    for (int i = 1; --stages > 0; i++)
        for (int j = 0; j < (1 << i); j++) mdct_butterfly_generic(T, x + (points >> i) * j, points >> i, 4 << i);
    for (int j = 0; j < points; j += 32) mdct_butterfly_32(x + j);
}
int main()
{
    static float x[1024]; mdct_lookup m = { 256, 8, x, 0, 1.0f };   // n=256: stages 3 -> first, generic x2 (i=1), then 4 x 32
    mdct_butterflies(&m, x, 128);
    if (g_first != 1 || g_generic != 2 || g_32 != 4 || g_last_points != 64 || g_last_trigint != 8) { std::printf("00c9c400_TEST FAIL\n"); return 1; }
    std::printf("00c9c400_TEST PASS\n"); return 0;
}