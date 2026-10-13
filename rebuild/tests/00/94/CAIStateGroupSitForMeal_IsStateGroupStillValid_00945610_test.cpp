#include <cstdio>
struct X { virtual void v00() {} virtual bool Is() { return Q() ? true : false; } unsigned char Q() { return 4; } };
int main()
{
    X x; if (!x.Is()) { std::printf("00945610_TEST FAIL\n"); return 1; }

    std::printf("00945610_TEST PASS\n");
    return 0;
}