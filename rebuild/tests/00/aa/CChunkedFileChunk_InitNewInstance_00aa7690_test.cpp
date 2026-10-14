#include <cstdio>
struct X { long F(long v) { return v; } };
int main()
{
    X x; if (x.F(4) != 4) { std::printf("00aa7690_TEST FAIL\n"); return 1; }

    std::printf("00aa7690_TEST PASS\n");
    return 0;
}