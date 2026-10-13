#include <cstdio>
static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct Blk { long r; void (__fastcall* f)(void*); void* o; }; struct Th { long f; long t; };
struct Ent { virtual Th* G() = 0; }; struct E2 : Ent { Th* t; virtual Th* G() { return t; } };
struct P { Ent* d; Blk* i; ~P() { if (i) { --i->r; if (i->r == 0) { i->f(i->o); delete i; } } i = 0; d = 0; } };
struct B { virtual void v() {} ~B() {} }; struct ST : B { P p; ~ST() {} Th* G() const { return p.d ? p.d->G() : 0; } };
struct I { void S(ST s, long v) { if (!s.G()) return; Th* t = s.G(); t->t = v; } };
int main()
{
    Th th; th.t = 0; E2 e; e.t = &th; Blk* b = new Blk; b->r = 1; b->f = Destroy; b->o = 0; ST s; s.p.d = &e; s.p.i = b; I i; i.S(s, 5); s.p.i = 0; s.p.d = 0; if (th.t != 5 || g_d != 1) { std::printf("008a6b10_TEST FAIL\n"); return 1; }

    std::printf("008a6b10_TEST PASS\n");
    return 0;
}
