#include <cstdio>
static int g_n = 0; struct CB { virtual void v() {} CB() { ++g_n; } ~CB() {} }; struct Ch : CB { Ch() {} }; struct X : CB { unsigned char p[0xc]; Ch c; X() : c() {} };
static bool Check() { X x; return g_n == 2; }

int main()
{
    if (!Check()) { std::printf("00c75530_TEST FAIL\n"); return 1; }
    std::printf("00c75530_TEST PASS\n");
    return 0;
}