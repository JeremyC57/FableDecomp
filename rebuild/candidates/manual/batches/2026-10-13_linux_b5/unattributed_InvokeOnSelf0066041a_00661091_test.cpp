#include <cstdio>
static void* g_t = 0; static void* g_a = 0; struct T { void M(T* a) { g_t = this; g_a = a; } };
struct I { void F(T* t) { t->M(t); } };
int main()
{
    T t; I i; i.F(&t); if (g_t != &t || g_a != &t) { std::printf("00661091_TEST FAIL\n"); return 1; }

    std::printf("00661091_TEST PASS\n");
    return 0;
}
