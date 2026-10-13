#include <cstdio>
#include <cstdlib>
static int g_d = 0; struct V { void* d; ~V() { ++g_d; } };
struct Tr { struct N { N* a; N* p; N* l; N* r; V v; }; N* h; unsigned long s; unsigned long E(N* w); };
static Tr::N* __cdecl U(Tr::N* w, Tr::N**, Tr::N**, Tr::N**) { return w; }
unsigned long Tr::E(N* w) { N* v = U(w, &h->p, &h->l, &h->r); v->v.~V(); if (v) std::free(v); return --s; }
int main()
{
    Tr t; Tr::N hd; t.h = &hd; t.s = 2; Tr::N* n = (Tr::N*)std::malloc(sizeof(Tr::N)); if (t.E(n) != 1 || g_d != 1) { std::printf("006e6e50_TEST FAIL\n"); return 1; }

    std::printf("006e6e50_TEST PASS\n");
    return 0;
}
