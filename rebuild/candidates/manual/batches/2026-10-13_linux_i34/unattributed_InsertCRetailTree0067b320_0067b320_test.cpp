#include <cstdio>
#include <new>
#include <cstdlib>
struct V { long k; long d; }; struct N { long c; N* p; N* l; N* r; V v; }; struct It { N* n; }; static int g_rb = 0;
static void __cdecl RB(N*, N*&) { ++g_rb; }
struct Tr { N* h; unsigned long sz; static N* C(const V& v) { N* n = (N*)std::malloc(sizeof(N)); new (&n->v) V(v); return n; }
    It* I(It* r, N* x, N* y, const V& v, N* w) { N* z; if (y == h || (w == 0 && (x != 0 || v.k < y->v.k))) { z = C(v); y->l = z; if (y == h) { h->p = z; h->r = z; } else if (y == h->l) h->l = z; } else { z = C(v); y->r = z; if (y == h->r) h->r = z; } z->p = y; z->l = 0; z->r = 0; RB(z, h->p); ++sz; r->n = z; return r; } };
int main()
{
    N hd; hd.p = 0; hd.l = &hd; hd.r = &hd; Tr t; t.h = &hd; t.sz = 0; V v; v.k = 3; v.d = 4; It it; t.I(&it, 0, &hd, v, 0); if (t.sz != 1 || hd.p != it.n || it.n->v.d != 4 || g_rb != 1) { std::printf("0067b320_TEST FAIL\n"); return 1; }
    V v2; v2.k = 5; v2.d = 0; It it2; t.I(&it2, 0, it.n, v2, 0); if (it.n->r != it2.n || t.sz != 2) { std::printf("0067b320_TEST FAIL\n"); return 1; }

    std::printf("0067b320_TEST PASS\n");
    return 0;
}
