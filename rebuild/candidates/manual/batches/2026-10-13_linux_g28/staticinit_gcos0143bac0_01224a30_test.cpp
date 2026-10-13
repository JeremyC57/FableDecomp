#include <cstdio>
#include <math.h>
#pragma intrinsic(cos)
extern const double g_a; const double g_a = 0.0; static float g_c = (float)cos(g_a);
int main()
{
    if (g_c != 1.0f) { std::printf("01224a30_TEST FAIL\n"); return 1; }

    std::printf("01224a30_TEST PASS\n");
    return 0;
}
