#include <cstdio>
struct X { unsigned char p[4]; unsigned char f; unsigned char G() { return (f >> 2) & 1; } };
int main()
{
    X x; x.f = 1 << 2; if (x.G() != 1) { std::printf("006e1d50_TEST FAIL\n"); return 1; }

    std::printf("006e1d50_TEST PASS\n");
    return 0;
}