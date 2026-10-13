#include <cstdio>
struct X { virtual void v00() {} virtual bool Is() { return Q() ? true : false; } unsigned char Q() { return 4; } };
int main()
{
    X x; if (!x.Is()) { std::printf("0083be20_TEST FAIL\n"); return 1; }

    std::printf("0083be20_TEST PASS\n");
    return 0;
}