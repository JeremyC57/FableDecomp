#include <cstdio>
struct X { unsigned char p[4]; long v; void F(const long& r) { v = r; } };
int main()
{
    X x; long k = 9; x.F(k); if (x.v != 9) { std::printf("0052e9b0_TEST FAIL\n"); return 1; }

    std::printf("0052e9b0_TEST PASS\n");
    return 0;
}