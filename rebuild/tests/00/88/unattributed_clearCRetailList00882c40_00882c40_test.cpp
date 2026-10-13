#include <cstdio>
#include <cstdlib>
#include <new>
static int g_d = 0; struct V { virtual ~V() { ++g_d; } };
struct L { struct N { N* n; N* p; V v; }; N* h; void C() { N* c = h->n; while (c != h) { N* t = c; c = c->n; t->v.~V(); std::free(t); } h->n = h; h->p = h; } };
int main()
{
    L l; L::N hd; l.h = &hd; L::N* a = (L::N*)std::malloc(sizeof(L::N)); new (&a->v) V; hd.n = a; a->n = &hd; a->p = &hd; hd.p = a; l.C(); if (g_d != 1 || hd.n != &hd || hd.p != &hd) { std::printf("00882c40_TEST FAIL\n"); return 1; }

    std::printf("00882c40_TEST PASS\n");
    return 0;
}