#include <cstdio>
static long g_k = 0; struct Base { virtual void v00() {} Base(long k) { g_k = k; } };
struct D : Base { virtual void v00() {} D() : Base(14) {} };
int main()
{
    D d; if (g_k != 14) { std::printf("00aa9650_TEST FAIL\n"); return 1; }

    std::printf("00aa9650_TEST PASS\n");
    return 0;
}
