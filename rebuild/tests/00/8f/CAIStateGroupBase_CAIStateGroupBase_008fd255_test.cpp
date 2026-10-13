#include <cstdio>
static int g_b = 0, g_m = 0; struct Base { virtual void v00() {} Base() { ++g_b; } };
struct M { M() { ++g_m; } }; struct D : Base { unsigned char pad[0x70]; M m; virtual void v00() {} D() : Base(), m() {} };
int main()
{
    D d; if (g_b != 1 || g_m != 1) { std::printf("008fd255_TEST FAIL\n"); return 1; }

    std::printf("008fd255_TEST PASS\n");
    return 0;
}