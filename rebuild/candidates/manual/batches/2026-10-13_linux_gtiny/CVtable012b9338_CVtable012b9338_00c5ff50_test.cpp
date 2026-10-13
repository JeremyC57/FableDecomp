#include <cstdio>
struct X { virtual void v00() {} X() {} };
int main()
{
    X x;

    std::printf("00c5ff50_TEST PASS\n");
    return 0;
}
