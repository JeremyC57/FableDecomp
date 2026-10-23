#include <cstdio>
static int g_d = 0; struct M { void* d; ~M() { ++g_d; } }; struct Mid { virtual void v00() {} ~Mid() {} };
struct D : Mid { unsigned char pad[4]; M m; virtual void v00() {} ~D() {} };
int main()
{
    { D d; } if (g_d != 1) { std::printf("00c1ff90_TEST FAIL\n"); return 1; }

    std::printf("01212860_TEST PASS\n");
    return 0;
}