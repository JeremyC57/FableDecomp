// Behaviour test for libvorbis res0.c res2_class (0x00CA1310): classify only when some channel is nonzero.
#include <cstdio>
static int g_called, g_ch;
static long** _2class(void*, void*, float**, int ch) { g_called++; g_ch = ch; static long* p[1]; return p; }
static long** res2_class(void* vb, void* vl, float** in, int* nonzero, int ch)
{
    int used = 0;
    for (int i = 0; i < ch; i++) if (nonzero[i]) used++;
    if (used) return _2class(vb, vl, in, ch);
    return 0;
}
int main()
{
    int nz0[2] = { 0, 0 }, nz1[2] = { 0, 1 };
    if (res2_class(0, 0, 0, nz0, 2) != 0 || g_called != 0) { std::printf("00ca1310_TEST FAIL\n"); return 1; }
    if (res2_class(0, 0, 0, nz1, 2) == 0 || g_called != 1 || g_ch != 2) { std::printf("00ca1310_TEST FAIL\n"); return 1; }
    std::printf("00ca1310_TEST PASS\n"); return 0;
}