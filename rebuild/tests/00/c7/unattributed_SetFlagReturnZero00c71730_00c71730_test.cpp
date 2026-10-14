#include <cstdio>
struct X { unsigned char p[4]; unsigned char f; long F(long) { f = 1; return 0; } };
int main()
{
    X x; x.f = 9; if (x.F(1) != 0 || x.f != 1) { std::printf("00c71730_TEST FAIL\n"); return 1; }

    std::printf("00c71730_TEST PASS\n");
    return 0;
}