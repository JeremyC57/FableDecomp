#include <cstdio>
static int g_h = 0, g_r = 0; struct Blk { long r; }; struct CP { void* o; Blk* c; CP(const CP& x) : o(x.o), c(x.c) { if (c) ++c->r; } CP(void* a, Blk* b) : o(a), c(b) {} ~CP() { if (c) --c->r; } };
struct EB { virtual void v() {} EB() {} }; struct E : EB { CP t; E(const CP& x) : t(x) {} };
struct H { virtual void v() {} void On(E e) { g_h = (e.t.c ? (int)e.t.c->r : -1); } };
struct D { unsigned char p0[5]; bool f; unsigned char p1[0x34 - 6]; void (H::*fn)(E); H* h; unsigned char p2[4]; CP t; D(H* x, const CP& y) : h(x), fn(&H::On), t(y) { f = false; } void Fire() { (h->*fn)(E(t)); f = true; } };
int main()
{
    Blk b; b.r = 1; H h; { CP cp(&b, &b); D d(&h, cp); d.Fire(); if (!d.f || g_h != 3 || b.r != 2) { std::printf("00ec52f0_TEST FAIL\n"); return 1; }
    }

    std::printf("00ec52f0_TEST PASS\n");
    return 0;
}