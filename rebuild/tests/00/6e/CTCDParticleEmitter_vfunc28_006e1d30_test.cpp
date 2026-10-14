#include <cstdio>
struct X { bool F(long k) { return k == 19; } };
int main()
{
    X x; if (!x.F(19) || x.F(20)) { std::printf("006e1d30_TEST FAIL\n"); return 1; }

    std::printf("006e1d30_TEST PASS\n");
    return 0;
}