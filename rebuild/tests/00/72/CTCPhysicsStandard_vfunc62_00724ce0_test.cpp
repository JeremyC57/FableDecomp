#include <cstdio>
struct X { unsigned char p[4]; unsigned char f; unsigned char G() { return f >> 7; } };
int main()
{
    X x; x.f = 0x80; if (x.G() != 1) { std::printf("00724ce0_TEST FAIL\n"); return 1; }

    std::printf("00724ce0_TEST PASS\n");
    return 0;
}