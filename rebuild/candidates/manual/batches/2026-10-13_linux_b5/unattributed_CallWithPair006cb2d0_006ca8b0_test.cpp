#include <cstdio>
static void* g_a = 0; static void* g_b = 0;
struct P { void* f; void* s; void* H(void* a, void* b) { g_a = a; g_b = b; return a; } void* F() { return H(f, s); } };
int main()
{
    int x, y; P p; p.f = &x; p.s = &y; if (p.F() != &x || g_a != &x || g_b != &y) { std::printf("006ca8b0_TEST FAIL\n"); return 1; }

    std::printf("006ca8b0_TEST PASS\n");
    return 0;
}
