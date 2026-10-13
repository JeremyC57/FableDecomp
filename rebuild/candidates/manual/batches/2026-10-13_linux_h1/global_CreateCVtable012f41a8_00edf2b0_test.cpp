#include <cstdio>
static void __fastcall Destroy(void*) { }
struct ST { unsigned char b[12]; ST() {} ST(const ST& o) { b[0] = o.b[0]; } }; struct SC { unsigned char p[0x40]; void* ctx; };
struct R { virtual void v00() {} void* c; R(void* x) : c(x) {} }; struct B : R { virtual void v00() {} ST t; B(SC* o, const ST& s) : R(o->ctx), t(s) {} };
struct D : B { virtual void v00() {} SC* o; void* a; unsigned char tl[4]; D(SC* ow, const ST& s, void* ar) : B(ow, s), o(ow), a(ar) {} };
struct K { long r; void (__fastcall* f)(void*); void* o; K(void* x) : r(1), f(Destroy), o(x) {} };
struct P { void* o; K* c; P(void* x) : o(x) { if (x) c = new K(o); else c = 0; } };
static P __fastcall Create(SC* ow, void* ar, const ST& s) { return P(new D(ow, s, ar)); }
int main()
{
    SC sc; int v; sc.ctx = &v; ST s; s.b[0] = 3; P p = Create(&sc, &v, s); D* d = (D*)p.o; if (!d || !p.c || d->c != &v || d->t.b[0] != 3 || d->o != &sc || p.c->r != 1) { std::printf("00edf2b0_TEST FAIL\n"); return 1; }
    delete p.c; delete d;

    std::printf("00edf2b0_TEST PASS\n");
    return 0;
}
