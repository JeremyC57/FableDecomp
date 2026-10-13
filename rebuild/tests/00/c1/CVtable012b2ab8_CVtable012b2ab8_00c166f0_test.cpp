#include <cstdio>
struct X { virtual void v00() {} X() {} };
int main()
{
    X x;

    std::printf("00c166f0_TEST PASS\n");
    return 0;
}