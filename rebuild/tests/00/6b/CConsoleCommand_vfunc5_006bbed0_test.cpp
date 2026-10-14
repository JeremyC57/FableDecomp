#include <cstdio>
static void* g_o = 0; static void* g_a = 0; static void __fastcall CB(void* o, void* a) { g_o = o; g_a = a; }
struct X { unsigned char p[4]; void* o; void (__fastcall* f)(void*, void*); void F(void* a) { f(o, a); } };
int main()
{
    X x; int i, j; x.o = &i; x.f = CB; x.F(&j); if (g_o != &i || g_a != &j) { std::printf("006bbed0_TEST FAIL\n"); return 1; }

    std::printf("006bbed0_TEST PASS\n");
    return 0;
}