#include <cstdio>
#include <cstdlib>
struct N { long c; N* p; N* l; N* r; }; static int g_u = 0, g_e = 0;
static N* __cdecl UL(N* n, N**, N**, N**) { ++g_u; return 0; }
struct Tr { N* h; unsigned long sz; void E(N*) { ++g_e; }
    static N* Succ(N* n) { if (n->r) { n = n->r; while (n->l) n = n->l; } else { N* y = n->p; while (n == y->r) { n = y; y = y->p; } if (n->r != y) n = y; } return n; }
    void ER(N* f, N* l) { if (f == h->l && l == h) { if (sz) { E(h->p); h->l = h; h->p = 0; h->r = h; sz = 0; } } else { while (f != l) { N* v = f; f = Succ(f); N* n = UL(v, &h->p, &h->l, &h->r); if (n) std::free(n); --sz; } } } };
int main()
{
    N hd, a, b; hd.p = &a; hd.l = &a; hd.r = &b; a.p = &hd; a.l = 0; a.r = &b; b.p = &a; b.l = 0; b.r = 0; Tr t; t.h = &hd; t.sz = 2; t.ER(&a, &b); if (g_u != 1 || t.sz != 1) { std::printf("004c6810_TEST FAIL\n"); return 1; }
    t.ER(&a, &hd); if (g_e != 1 || t.sz != 0) { std::printf("004c6810_TEST FAIL\n"); return 1; }

    std::printf("004c6810_TEST PASS\n");
    return 0;
}
