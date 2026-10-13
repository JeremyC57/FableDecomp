#include <cstdio>
struct C; static void* g_c = 0; static void* g_v = 0; struct T { void M(C* c, void* v) { g_c = c; g_v = v; } };
struct C { void F(T* t, void* v) { t->M(this, v); } };
int main()
{
    T t; C c; int x; c.F(&t, &x); if (g_c != &c || g_v != &x) { std::printf("00996970_TEST FAIL\n"); return 1; }

    std::printf("00996970_TEST PASS\n");
    return 0;
}