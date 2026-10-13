#include <cstdio>
extern long g_a; long g_a = 3; static long g_b = g_a * 5;
int main()
{
    if (g_b != 15) { std::printf("01221cf0_TEST FAIL\n"); return 1; }

    std::printf("01221cf0_TEST PASS\n");
    return 0;
}
