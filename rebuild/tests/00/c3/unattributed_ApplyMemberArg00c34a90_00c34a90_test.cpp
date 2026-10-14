#include <cstdio>
static void* g_v = 0; struct M { void A(void* v) { g_v = v; } }; struct X { unsigned char p[4]; M m; void F(long unused0, void* value) { m.A(value); } };
int main()
{
    X x; int i; x.F(0, &i); if (g_v != &i) { std::printf("00c34a90_TEST FAIL\n"); return 1; }

    std::printf("00c34a90_TEST PASS\n");
    return 0;
}