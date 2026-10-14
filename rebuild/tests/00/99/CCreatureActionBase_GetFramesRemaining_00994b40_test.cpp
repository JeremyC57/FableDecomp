#include <cstdio>
struct X { unsigned char p[4]; long s; long e; long D() { return e - s; } };
int main()
{
    X x; x.s = 2; x.e = 7; if (x.D() != 5) { std::printf("00994b40_TEST FAIL\n"); return 1; }

    std::printf("00994b40_TEST PASS\n");
    return 0;
}