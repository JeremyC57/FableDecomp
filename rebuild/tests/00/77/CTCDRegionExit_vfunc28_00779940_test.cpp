#include <cstdio>
struct X { bool F(long k) { return k == 25; } };
int main()
{
    X x; if (!x.F(25) || x.F(26)) { std::printf("00779940_TEST FAIL\n"); return 1; }

    std::printf("00779940_TEST PASS\n");
    return 0;
}