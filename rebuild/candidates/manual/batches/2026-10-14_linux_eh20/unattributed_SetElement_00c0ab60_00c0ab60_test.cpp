#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_rf = 0, g_up = 0; struct Dp { void Refresh() { ++g_rf; } }; struct Sp { void Update() { ++g_up; } }; struct Ow { unsigned char p[0x38]; Dp* d; unsigned char q[0x24]; Sp* s; };
struct X { unsigned char p[0x344]; float e[8]; unsigned char q[0x60]; Ow* o; void Set(unsigned long i, float v) { L l(&g_g); if (i < 8 && e[i] != v) { e[i] = v; o->d->Refresh(); if (o->s) o->s->Update(); } } };
static bool Check() { Dp d; Sp sp; Ow o; o.d = &d; o.s = &sp; X* x = new X; x->o = &o; for (int i = 0; i < 8; ++i) x->e[i] = 0; x->Set(2, 1.5f); x->Set(2, 1.5f); x->Set(8, 3.0f); bool ok = g_rf == 1 && g_up == 1 && x->e[2] == 1.5f && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c0ab60_TEST FAIL\n"); return 1; }
    std::printf("00c0ab60_TEST PASS\n");
    return 0;
}
