#include <cstdio>
static int g_b = 0; struct Base { virtual void v00() {} Base() { ++g_b; } };
struct D : Base { virtual void v00() {} D() : Base() {} };
int main()
{
    D d; if (g_b != 1) { std::printf("006b2a70_TEST FAIL\n"); return 1; }

    std::printf("006b2a70_TEST PASS\n");
    return 0;
}
