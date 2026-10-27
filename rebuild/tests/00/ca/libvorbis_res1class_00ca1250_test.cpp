#include <cstdio>
// res1_class: pack the nonzero channels to the front, classify them (4-arg _01class), NULL when none.
static int g_used = -1;
static float** g_in = 0;
static long g_result[1];
static long* g_ret = g_result;
static long** _01class(void* vb, void* vl, float** in, int ch) { g_used = ch; g_in = in; return &g_ret; }
static long** res1_class(void* vb, void* vl, float** in, int* nonzero, int ch)
{
    int i, used = 0;
    for (i = 0; i < ch; i++)
        if (nonzero[i])
            in[used++] = in[i];
    if (used)
        return (_01class(vb, vl, in, used));
    else
        return (0);
}
int main()
{
    float a[1], b[1], c[1], d[1];
    float* in[4] = { a, b, c, d };
    int nz[4] = { 0, 1, 0, 1 };
    if (res1_class(0, 0, in, nz, 4) != &g_ret || g_used != 2 || in[0] != b || in[1] != d || in[2] != c || in[3] != d) {
        std::printf("00ca1250_TEST FAIL\n"); return 1;
    }
    int none[2] = { 0, 0 };
    g_used = -1;
    if (res1_class(0, 0, in, none, 2) != 0 || g_used != -1) { std::printf("00ca1250_TEST FAIL\n"); return 1; }
    std::printf("00ca1250_TEST PASS\n");
    return 0;
}