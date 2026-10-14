#include <cstdio>
static int g_d = 0; struct M { void* p; ~M() { ++g_d; } }; struct Base { virtual void v00() {} ~Base() {} };
struct D : Base { unsigned char pad[4]; M m; virtual void v00() {} ~D() {} };
int main()
{
    { D d; } if (g_d != 1) { std::printf("00c2b940_TEST FAIL\n"); return 1; }

    std::printf("00c2b940_TEST PASS\n");
    return 0;
}