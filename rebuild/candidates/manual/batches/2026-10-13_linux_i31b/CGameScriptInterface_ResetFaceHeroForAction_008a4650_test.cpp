#include <cstdio>
static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct Blk { long r; void (__fastcall* f)(void*); void* o; }; struct Df { long v; }; struct Th { long f; long t; Df* d; };
struct Ent { virtual Th* G() = 0; }; struct E2 : Ent { Th* t; virtual Th* G() { return t; } };
struct P { Ent* d; Blk* i; ~P() { if (i) { --i->r; if (i->r == 0) { i->f(i->o); delete i; } } i = 0; d = 0; } };
struct B { virtual void v() {} ~B() {} }; struct ST : B { P p; ~ST() {} Th* G() const { return p.d ? p.d->G() : 0; } };
struct I { void S(ST s) { if (!s.G()) return; Th* t = s.G(); t->t = t->d->v; } };
int main()
{
    Df d; d.v = 9; Th th; th.t = 0; th.d = &d; E2 e; e.t = &th; Blk* b = new Blk; b->r = 1; b->f = Destroy; b->o = 0; ST s; s.p.d = &e; s.p.i = b; I i; i.S(s); s.p.i = 0; s.p.d = 0; if (th.t != 9 || g_d != 1) { std::printf("008a4650_TEST FAIL\n"); return 1; }

    std::printf("008a4650_TEST PASS\n");
    return 0;
}
