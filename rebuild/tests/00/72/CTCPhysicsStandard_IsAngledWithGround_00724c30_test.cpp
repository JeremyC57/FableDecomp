#include <cstdio>
struct X { unsigned char p[4]; unsigned char f; unsigned char G() { return (f >> 1) & 1; } };
int main()
{
    X x; x.f = 1 << 1; if (x.G() != 1) { std::printf("00724c30_TEST FAIL\n"); return 1; }

    std::printf("00724c30_TEST PASS\n");
    return 0;
}