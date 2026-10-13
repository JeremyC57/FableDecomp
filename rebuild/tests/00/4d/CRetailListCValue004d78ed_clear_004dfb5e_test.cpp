#include <cstdio>
#include <cstdlib>
static int g_d = 0; struct M { void* d; ~M() { ++g_d; } };
struct L { struct N { N* n; N* p; M v; }; N* h; void clear() { N* x = h->n; while (x != h) { N* v = x; x = x->n; v->v.~M(); std::free(v); } h->n = h; h->p = h; } };
int main()
{
    L l; L::N head; l.h = &head; L::N* a = (L::N*)std::malloc(sizeof(L::N)); head.n = a; a->n = &head; a->p = &head; head.p = a; l.clear(); if (g_d != 1 || head.n != &head || head.p != &head) { std::printf("004dfb5e_TEST FAIL\n"); return 1; }

    std::printf("004dfb5e_TEST PASS\n");
    return 0;
}