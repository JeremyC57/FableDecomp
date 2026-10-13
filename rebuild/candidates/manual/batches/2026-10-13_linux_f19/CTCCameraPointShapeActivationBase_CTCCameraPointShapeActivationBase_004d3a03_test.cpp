#include <cstdio>
static void* g_a = 0; struct Base { virtual void v00() {} Base(void* a) { g_a = a; } };
struct D : Base { virtual void v00() {} D(void* a) : Base(a) {} };
int main()
{
    int x; D d(&x); if (g_a != &x) { std::printf("004d3a03_TEST FAIL\n"); return 1; }

    std::printf("004d3a03_TEST PASS\n");
    return 0;
}
