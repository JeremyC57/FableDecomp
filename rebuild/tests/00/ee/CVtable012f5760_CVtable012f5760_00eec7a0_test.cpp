#include <cstdio>
static int g_b = 0; struct Base { virtual void v00() {} Base() { ++g_b; } };
struct D : Base { void* a; virtual void v00() {} D(void* x) : Base(), a(x) {} };
int main()
{
    int x; D d(&x); if (g_b != 1 || d.a != &x) { std::printf("00eec7a0_TEST FAIL\n"); return 1; }

    std::printf("00eec7a0_TEST PASS\n");
    return 0;
}