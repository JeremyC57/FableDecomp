#include <cstdio>
struct X { unsigned char p[4]; long v; long G() { return v + 256; } };
int main()
{
    X x; x.v = 1; if (x.G() != 257) { std::printf("006a99c0_TEST FAIL\n"); return 1; }

    std::printf("006a99c0_TEST PASS\n");
    return 0;
}