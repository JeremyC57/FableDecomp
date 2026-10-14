#include <cstdio>
static int g_b = 0;
struct Base { virtual void v00() {} Base() { ++g_b; } };
struct CP { void* a; void* b; CP() : a(0), b(0) {} };
struct D : Base { virtual void v00() {} CP p; bool c; D() : Base(), p(), c(true) {} };
int main()
{
    D d;
    if (g_b != 1 || d.p.a != 0 || d.p.b != 0 || !d.c) { std::printf("00af6550_TEST FAIL\n"); return 1; }
    std::printf("00af6550_TEST PASS\n");
    return 0;
}