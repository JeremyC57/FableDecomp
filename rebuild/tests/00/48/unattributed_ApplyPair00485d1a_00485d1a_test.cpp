#include <cstdio>
static void* g_a = 0; static void* g_b = 0; struct X { void* f; void* s; void A(void* a_, void* b_) { g_a = a_; g_b = b_; } void F() { A(f, s); } };
int main()
{
    X x; int i, j; x.f = &i; x.s = &j; x.F(); if (g_a != &i || g_b != &j) { std::printf("00485d1a_TEST FAIL\n"); return 1; }

    std::printf("00485d1a_TEST PASS\n");
    return 0;
}