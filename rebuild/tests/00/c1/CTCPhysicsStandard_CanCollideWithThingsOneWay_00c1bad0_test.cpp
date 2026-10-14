#include <cstdio>
struct X { unsigned char p[4]; unsigned char f; long G() { return f & 1; } };
int main()
{
    X x; x.f = 0xff; if (x.G() != 1) { std::printf("00c1bad0_TEST FAIL\n"); return 1; }

    std::printf("00c1bad0_TEST PASS\n");
    return 0;
}