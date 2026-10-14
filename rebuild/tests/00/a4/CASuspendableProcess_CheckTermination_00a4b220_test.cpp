#include <cstdio>
struct X { unsigned char p[4]; unsigned char v; unsigned char G() { return v != 0; } };
int main()
{
    X x; x.v = 2; if (!x.G()) { std::printf("00a4b220_TEST FAIL\n"); return 1; }
    x.v = 0; if (x.G()) { std::printf("00a4b220_TEST FAIL\n"); return 1; }

    std::printf("00a4b220_TEST PASS\n");
    return 0;
}