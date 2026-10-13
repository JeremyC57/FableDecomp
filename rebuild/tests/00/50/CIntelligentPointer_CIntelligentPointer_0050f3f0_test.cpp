#include <cstdio>
static int g_b = 0; static void* g_a = 0; struct Base { virtual void v00() {} Base() { ++g_b; } };
struct D : Base { virtual void v00() {} D(void* a) : Base() { Set(a); } void Set(void* a) { g_a = a; } };
int main()
{
    int x; D d(&x); if (g_b != 1 || g_a != &x) { std::printf("0050f3f0_TEST FAIL\n"); return 1; }

    std::printf("0050f3f0_TEST PASS\n");
    return 0;
}