#include <cstdio>
struct X { unsigned char p[4]; unsigned char f; long G() { return (f >> 4) & 1; } };
int main()
{
    X x; x.f = 1 << 4; if (x.G() != 1) { std::printf("0053ce41_TEST FAIL\n"); return 1; }
    x.f = 0; if (x.G() != 0) { std::printf("0053ce41_TEST FAIL\n"); return 1; }

    std::printf("0053ce41_TEST PASS\n");
    return 0;
}