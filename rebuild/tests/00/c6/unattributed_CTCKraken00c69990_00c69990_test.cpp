#include <cstdio>
static int g_b = 0; struct CB { virtual void v() {} ~CB() { ++g_b; } }; struct Ch : CB { ~Ch() {} }; struct __declspec(novtable) X : CB { unsigned char p[0xc]; Ch c; ~X() {} };
static bool Check() { { X x; } return g_b == 2; }

int main()
{
    if (!Check()) { std::printf("00c69990_TEST FAIL\n"); return 1; }
    std::printf("00c69990_TEST PASS\n");
    return 0;
}