#include <cstdio>
struct X; static int g_rel = 0, g_free = 0; static void Rel(void* b) { ++g_free; } struct Rg { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void Remove(X* e) { removed = e; } X* removed; Rg() : removed(0) {} }; struct Ow { unsigned char p[0x74]; Rg r; };
struct Bh { void* p; ~Bh() { if (p) { Rel(p); p = 0; } } };
struct X { Ow* o; long a; Bh f; long c, d; Bh s; void R() { ++g_rel; } ~X() { R(); o->r.Remove(this); } };
static bool Check() { Ow o; X* x = new X; x->o = &o; int b; x->f.p = &b; x->s.p = 0; delete x; return g_rel == 1 && g_free == 1 && o.r.removed == x; }

int main()
{
    if (!Check()) { std::printf("00c1e850_TEST FAIL\n"); return 1; }
    std::printf("00c1e850_TEST PASS\n");
    return 0;
}
