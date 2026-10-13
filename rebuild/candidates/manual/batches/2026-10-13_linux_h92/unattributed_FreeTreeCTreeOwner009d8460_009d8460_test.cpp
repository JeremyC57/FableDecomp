#include <cstdio>
#include <cstdlib>
static int g_f = 0, g_d = 0; struct M { void* d; ~M() { ++g_d; } }; struct N { unsigned char p[8]; N* n; N* c; M m; };
struct X { void F(N* d) { while (d) { F(d->c); N* x = d->n; d->m.~M(); std::free(d); ++g_f; d = x; } } };
int main()
{
    N* a = (N*)std::malloc(sizeof(N)); N* b = (N*)std::malloc(sizeof(N)); a->n = b; a->c = 0; b->n = 0; b->c = 0;
    X x; x.F(a); if (g_f != 2 || g_d != 2) { std::printf("009d8460_TEST FAIL\n"); return 1; }

    std::printf("009d8460_TEST PASS\n");
    return 0;
}
