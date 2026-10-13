#include <cstdio>
#include <cstdlib>
static int g_f = 0; struct N { unsigned char p[8]; N* n; N* c; };
struct X { void F(N* d) { while (d) { F(d->c); N* x = d->n; std::free(d); ++g_f; d = x; } } };
int main()
{
    N* a = (N*)std::malloc(sizeof(N)); N* b = (N*)std::malloc(sizeof(N)); N* c = (N*)std::malloc(sizeof(N)); a->n = b; a->c = 0; b->n = 0; b->c = c; c->n = 0; c->c = 0;
    X x; x.F(a); if (g_f != 3) { std::printf("00a20f50_TEST FAIL\n"); return 1; }

    std::printf("00a20f50_TEST PASS\n");
    return 0;
}
