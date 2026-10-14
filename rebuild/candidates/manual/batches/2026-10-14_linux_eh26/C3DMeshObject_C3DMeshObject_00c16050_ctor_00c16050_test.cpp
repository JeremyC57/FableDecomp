#include <cstdio>
#include <new>
#include <cstdlib>
static int g_vol = 0; static bool g_fail = false; static float g_ox = 0, g_oy = 0, g_oz = 0, g_sc = 0; static void* g_bind = 0; static void* g_a = 0;
struct V { float x, y, z; }; struct Vol { unsigned char b[0x340]; Vol(void* bind, const V* o, void* a, void* b2, void* c, float s) { ++g_vol; g_bind = bind; g_ox = o->x; g_oy = o->y; g_oz = o->z; g_a = a; g_sc = s; } static void* operator new(size_t n) throw() { return g_fail ? 0 : std::malloc(n); } static void operator delete(void* p) { std::free(p); } };
struct Ds { unsigned short w0, w2; void* a; void* b; void* c; V o; }; struct Ow { unsigned char p[0x5c]; void* bind; }; struct Bs { virtual void v0() {} Bs() {} ~Bs() {} };
struct X : Bs { Ow* ow; Vol* vol; unsigned short mc, me; float m10, m14; float Scale() { return 2.5f; } X(Ow* O, const Ds* d, void* u) : ow(O), vol(0), mc(d->w0), me(d->w2), m10(1.0f), m14(1.0f) { V o; o.x = d->o.x; o.y = d->o.z; o.z = d->o.y; float s = Scale(); vol = new Vol(ow->bind, &o, d->a, d->b, d->c, s); } };
static bool Check() { Ow ow; int bd, a; ow.bind = &bd; Ds d; d.w0 = 7; d.w2 = 9; d.a = &a; d.b = 0; d.c = 0; d.o.x = 1; d.o.y = 2; d.o.z = 3; X* x = new X(&ow, &d, 0); bool ok = x->ow == &ow && x->vol != 0 && g_vol == 1 && g_bind == &bd && g_ox == 1 && g_oy == 3 && g_oz == 2 && g_a == &a && g_sc == 2.5f && x->mc == 7 && x->me == 9 && x->m10 == 1.0f; delete x->vol; delete x; g_fail = true; X* y = new X(&ow, &d, 0); ok = ok && y->vol == 0 && g_vol == 1; delete y; return ok; }

int main()
{
    if (!Check()) { std::printf("00c16050_TEST FAIL\n"); return 1; }
    std::printf("00c16050_TEST PASS\n");
    return 0;
}
