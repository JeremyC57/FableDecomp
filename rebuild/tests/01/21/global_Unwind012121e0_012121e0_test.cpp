#include <cstdio>
static int g_f = 0, g_s = 0; struct F { unsigned char b[28]; ~F() { ++g_f; } }; struct Sd { unsigned char b[4]; ~Sd() { ++g_s; } }; struct P { F f; Sd s; ~P() {} };
int main()
{
    { P p; } if (g_f != 1 || g_s != 1) { std::printf("00c1adb0_TEST FAIL\n"); return 1; }
    std::printf("012121e0_TEST PASS\n");
    return 0;
}