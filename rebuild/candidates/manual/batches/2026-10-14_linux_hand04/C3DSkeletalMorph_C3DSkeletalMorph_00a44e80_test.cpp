#include <cstdio>
static int g_b = 0;
struct Base { virtual void v00() {} Base() { ++g_b; } };
struct Arr { void* a; void* b; void* c; Arr() : a(0), b(0), c(0) {} };
struct CP { void* a; void* b; CP() : a(0), b(0) {} };
struct M : Base { virtual void v00() {} Arr arr; CP cp; M() : Base(), arr(), cp() {} };
int main()
{
    M m;
    if (g_b != 1 || m.arr.a || m.arr.b || m.arr.c || m.cp.a || m.cp.b) { std::printf("00a44e80_TEST FAIL\n"); return 1; }
    std::printf("00a44e80_TEST PASS\n");
    return 0;
}
