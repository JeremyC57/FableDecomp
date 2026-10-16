#include <cstdio>
static int g_reg = -1, g_released = 0, g_co = 0;
static long g_mapperHr = 0, g_regHr = 0;
static long CoCreate(int** pp) { static int mapper; *pp = &mapper; return g_mapperHr; }
static long RegisterFilter(int flag) { g_reg = flag; return g_regHr; }
static long Run(bool hasSetup)
{
    if (!hasSetup) return 1;
    ++g_co;
    int* m; long hr = CoCreate(&m);
    if (hr >= 0) { hr = RegisterFilter(0); ++g_released; }
    --g_co;
    if (1) { if ((long)0x80070002 == hr) return 0; else return hr; }
    return 0;
}
int main()
{
    if (Run(false) != 1 || g_reg != -1) { std::printf("00caaf10_TEST FAIL\n"); return 1; }
    g_regHr = (long)0x80070002;
    if (Run(true) != 0 || g_reg != 0 || g_released != 1 || g_co != 0) { std::printf("00caaf10_TEST FAIL\n"); return 1; }
    g_regHr = (long)0x80004005;
    if (Run(true) != (1 ? (long)0x80004005 : 0)) { std::printf("00caaf10_TEST FAIL\n"); return 1; }
    std::printf("00caaf10_TEST PASS\n");
    return 0;
}
