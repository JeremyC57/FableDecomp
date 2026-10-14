#include <cstdio>
struct X { virtual void v() {} void* p; X() : p(0) {} };
int main()
{
    X x; if (x.p != 0) { std::printf("00c35170_TEST FAIL\n"); return 1; }

    std::printf("00c35170_TEST PASS\n");
    return 0;
}