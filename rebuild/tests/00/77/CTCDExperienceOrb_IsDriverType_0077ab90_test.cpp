#include <cstdio>
struct X { bool F(long k) { return k == 26; } };
int main()
{
    X x; if (!x.F(26) || x.F(27)) { std::printf("0077ab90_TEST FAIL\n"); return 1; }

    std::printf("0077ab90_TEST PASS\n");
    return 0;
}