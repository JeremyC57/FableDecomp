#include <cstdio>
static void* g_a = 0; static void* g_b = 0; struct Base { virtual void v00() {} Base(void* a, void* b) { g_a = a; g_b = b; } };
struct D : Base { virtual void v00() {} D(void* a, void* b) : Base(a, b) {} };
static D* __fastcall Alloc(void* a, void* b) { return new D(a, b); }
int main()
{
    int x, y; D* d = Alloc(&x, &y); if (!d || g_a != &x || g_b != &y) { std::printf("008b6410_TEST FAIL\n"); return 1; }
    delete d;

    std::printf("008b6410_TEST PASS\n");
    return 0;
}
