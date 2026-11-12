#include <cstdio>
struct X { virtual void v00() {} virtual long Is(long v) { return v == 23; } };
int main()
{
    X x; if (!x.Is(23) || x.Is(5)) { std::printf("004df1bc_TEST FAIL\n"); return 1; }

    std::printf("004df1bc_TEST PASS\n");
    return 0;
}