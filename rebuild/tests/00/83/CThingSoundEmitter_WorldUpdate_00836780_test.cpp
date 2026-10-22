#include <cstdio>
static unsigned char g_s = 1; struct X { unsigned char G() { return g_s; } unsigned char F() { return G() == 1; } };
int main()
{
    X x; if (!x.F()) { std::printf("00836780_TEST FAIL\n"); return 1; }
    g_s = 2; if (x.F()) { std::printf("00836780_TEST FAIL\n"); return 1; }

    std::printf("00836780_TEST PASS\n");
    return 0;
}