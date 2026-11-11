#include <cstdio>
struct X { virtual void v00() {} virtual bool Get() { return b2; } unsigned char p[4]; unsigned char b0 : 1; unsigned char b1 : 1; unsigned char b2 : 1; };
int main()
{
    X x; x.b0 = 0; x.b1 = 0; x.b2 = 1; if (!x.Get()) { std::printf("00724c70_TEST FAIL\n"); return 1; }
    x.b2 = 0; if (x.Get()) { std::printf("00724c70_TEST FAIL\n"); return 1; }

    std::printf("00724c70_TEST PASS\n");
    return 0;
}