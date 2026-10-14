#include <cstdio>
static int g_n = 0; struct T { void M() { ++g_n; } }; struct X { unsigned char p[4]; T* t; void F(long a0) { t->M(); } };
int main()
{
    T t; X x; x.t = &t; x.F(0); if (g_n != 1) { std::printf("0068eb10_TEST FAIL\n"); return 1; }

    std::printf("0068eb10_TEST PASS\n");
    return 0;
}