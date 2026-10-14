#include <cstdio>
struct X { unsigned char p[4]; unsigned char f; long F(long) { f = 0; return 0; } };
int main()
{
    X x; x.f = 9; if (x.F(1) != 0 || x.f != 0) { std::printf("00c6abf0_TEST FAIL\n"); return 1; }

    std::printf("00c6abf0_TEST PASS\n");
    return 0;
}