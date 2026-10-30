#include <cstdio>
static unsigned char g_f = 3; struct X { unsigned char G() { return g_f; } };
int main()
{
    X x; if (x.G() != 3) { std::printf("00cb28e0_TEST FAIL\n"); return 1; }

    std::printf("00cb28e0_TEST PASS\n");
    return 0;
}