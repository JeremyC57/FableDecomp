#include <cstdio>
struct X { long v; void S(long x) { v = x; } };
int main()
{
    X x; x.S(6); if (x.v != 6) { std::printf("00c1bd60_TEST FAIL\n"); return 1; }

    std::printf("00c1bd60_TEST PASS\n");
    return 0;
}