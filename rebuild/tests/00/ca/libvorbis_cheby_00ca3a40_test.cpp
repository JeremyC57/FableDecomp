// Behaviour test for libvorbis lsp.c cheby (0x00CA3A40): convert a symmetric polynomial to Chebyshev form in place.
#include <cstdio>
static void cheby(float* g, int ord)
{
    g[0] *= .5f;
    for (int i = 2; i <= ord; i++)
        for (int j = ord; j >= i; j--) { g[j - 2] -= g[j]; g[j] += g[j]; }
}
int main()
{
    float g[3] = { 2.0f, 1.0f, 1.0f };
    cheby(g, 2);
    if (g[0] != 0.0f || g[1] != 1.0f || g[2] != 2.0f) { std::printf("00ca3a40_TEST FAIL\n"); return 1; }
    float h[4] = { 4.0f, 0.0f, 0.0f, 1.0f };
    cheby(h, 3);
    if (h[0] != 2.0f || h[1] != -3.0f || h[2] != 0.0f || h[3] != 4.0f) { std::printf("00ca3a40_TEST FAIL\n"); return 1; }
    std::printf("00ca3a40_TEST PASS\n"); return 0;
}