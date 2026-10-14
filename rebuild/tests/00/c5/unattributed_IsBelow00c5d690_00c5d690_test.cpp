#include <cstdio>
struct X { unsigned char p[4]; unsigned long c; unsigned long l; long G() { return c < l; } };
int main()
{
    X x; x.c = 1; x.l = 2; if (x.G() != 1) { std::printf("00c5d690_TEST FAIL\n"); return 1; }
    x.c = 3; if (x.G() != 0) { std::printf("00c5d690_TEST FAIL\n"); return 1; }

    std::printf("00c5d690_TEST PASS\n");
    return 0;
}