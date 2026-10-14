#include <cstdio>
struct X { unsigned char p[4]; unsigned char f; long G() { return (f >> 1) & 1; } };
int main()
{
    X x; x.f = 1 << 1; if (x.G() != 1) { std::printf("00c27b80_TEST FAIL\n"); return 1; }
    x.f = 0; if (x.G() != 0) { std::printf("00c27b80_TEST FAIL\n"); return 1; }

    std::printf("00c27b80_TEST PASS\n");
    return 0;
}