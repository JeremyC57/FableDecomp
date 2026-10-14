#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_set = 0, g_rf = 0, g_up = 0; static long g_mode = -1; struct Sl { unsigned char b[0x34]; void Set(void* a, long m, void* b) { ++g_set; g_mode = m; } }; struct Dp { void Refresh() { ++g_rf; } }; struct Sp { void Update() { ++g_up; } }; struct Ow { unsigned char p[0x38]; Dp* d; unsigned char q[0x24]; Sp* s; };
struct X { unsigned char p[4]; Sl s[16]; unsigned char q[0x80]; Ow* o; void SetSlot(unsigned long i, void* a, void* b) { L l(&g_g); if (i < 16) { s[i].Set(a, 0, b); o->d->Refresh(); if (o->s) o->s->Update(); } } };
static bool Check() { Dp d; Sp sp; Ow o; o.d = &d; o.s = 0; X* x = new X; x->o = &o; x->SetSlot(2, 0, 0); x->SetSlot(16, 0, 0); o.s = &sp; x->SetSlot(15, 0, 0); bool ok = g_set == 2 && g_rf == 2 && g_up == 1 && g_mode == 0 && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c0b380_TEST FAIL\n"); return 1; }
    std::printf("00c0b380_TEST PASS\n");
    return 0;
}
