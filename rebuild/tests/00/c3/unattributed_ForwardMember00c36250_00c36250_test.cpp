#include <cstdio>
static long g_v = 0; struct T { void R(long v) { g_v = v; } }; struct X { unsigned char p[4]; long v; void F(T* t) { t->R(v); } };
int main()
{
    T t; X x; x.v = 3; x.F(&t); if (g_v != 3) { std::printf("00c36250_TEST FAIL\n"); return 1; }

    std::printf("00c36250_TEST PASS\n");
    return 0;
}