#include <cstdio>
#include <cstdlib>
static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct B { long r; void (__fastcall* f)(void*); void* o; };
struct P { void* o; B* c; ~P() { if (c) { --c->r; if (c->r == 0) { c->f(c->o); delete c; } } o = 0; c = 0; } };
struct L { struct N { N* n; N* p; P v; }; N* h; void clear() { N* x = h->n; while (x != h) { N* v = x; x = x->n; v->v.~P(); std::free(v); } h->n = h; h->p = h; } };
int main()
{
    L l; L::N head; head.v.o = 0; head.v.c = 0; l.h = &head; L::N* a = (L::N*)std::malloc(sizeof(L::N)); B* b = new B; b->r = 1; b->f = Destroy; b->o = 0; a->v.o = 0; a->v.c = b; head.n = a; a->n = &head; a->p = &head; head.p = a;
    l.clear(); if (g_d != 1 || head.n != &head || head.p != &head) { std::printf("00719280_TEST FAIL\n"); return 1; }

    std::printf("00719280_TEST PASS\n");
    return 0;
}