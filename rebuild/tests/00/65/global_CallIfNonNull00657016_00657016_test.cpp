#include <cstdio>
static long g_k = 0; struct T { void M(long k) { g_k = k; } }; static void __fastcall F(T* t) { if (t) t->M(1); }
int main()
{
    T t; F(0); F(&t); if (g_k != 1) { std::printf("00657016_TEST FAIL\n"); return 1; }

    std::printf("00657016_TEST PASS\n");
    return 0;
}