#include <cstdio>
static int g_f = 0, g_s = 0; struct FM { unsigned char b[0x30]; ~FM() { ++g_f; } }; struct F { unsigned char h[12]; FM m; }; struct Sd { unsigned char b[4]; ~Sd() { ++g_s; } }; struct P { F f; Sd s; ~P() {} };
static bool Check() { { P p; } return g_f == 1 && g_s == 1; }

int main()
{
    if (!Check()) { std::printf("00c2e140_TEST FAIL\n"); return 1; }
    std::printf("01213c05_TEST PASS\n");
    return 0;
}