#include <cstdio>
static void* g_t = 0; static void* g_a = 0; struct T { void M(T* a) { g_t = this; g_a = a; } };
static void __stdcall F(T* t) { t->M(t); }
int main()
{
    T t; F(&t); if (g_t != &t || g_a != &t) { std::printf("0066107f_TEST FAIL\n"); return 1; }

    std::printf("0066107f_TEST PASS\n");
    return 0;
}