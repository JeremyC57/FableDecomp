#include <cstdio>
struct X { unsigned char p[4]; long i; unsigned char G() { return i != -1; } };
int main()
{
    X x; x.i = 3; if (!x.G()) { std::printf("00789ab0_TEST FAIL\n"); return 1; }
    x.i = -1; if (x.G()) { std::printf("00789ab0_TEST FAIL\n"); return 1; }

    std::printf("00789ab0_TEST PASS\n");
    return 0;
}