#include <cstdio>
struct X { unsigned char p[4]; short v; short G() { return v; } };
int main()
{
    X x; x.v = -3; if (x.G() != -3) { std::printf("00c1baa0_TEST FAIL\n"); return 1; }

    std::printf("00c1baa0_TEST PASS\n");
    return 0;
}