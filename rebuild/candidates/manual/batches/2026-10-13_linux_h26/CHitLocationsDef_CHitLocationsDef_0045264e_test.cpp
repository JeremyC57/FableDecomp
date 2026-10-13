#include <cstdio>
static int g_b = 0; struct Base { virtual void v00() {} Base() { ++g_b; } };
struct Z { long a, b, c; Z() : a(0), b(0), c(0) {} }; struct D : Base { unsigned char pad[4]; Z z; virtual void v00() {} D() : Base(), z() {} };
int main()
{
    D d; if (g_b != 1 || d.z.a || d.z.b || d.z.c) { std::printf("0045264e_TEST FAIL\n"); return 1; }

    std::printf("0045264e_TEST PASS\n");
    return 0;
}
