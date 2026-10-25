#include <cstdio>
#include <new>
static int g_lk = 0; static unsigned char g_sec[8]; struct Pl { void* s; unsigned long t; Pl(void* x) : s(x), t(0) { ++g_lk; } ~Pl() { --g_lk; } };
#include <cstdlib>
static int g_int = 0, g_det = 0, g_edel = 0, g_rem = 0, g_reset = 0, g_items = 0; static void* g_hitkey = 0; static void* g_lastkey = 0;
struct Tg { unsigned char pad[0x2c]; void** keys; long m30; unsigned long n; };
struct Rec { long m0; Tg* t; }; struct X; struct Ac { unsigned char pad[0x18]; Rec* r; long active; long IsActive() { return active; } void Stop(X* h) { ++g_int; } };
struct Al { long m0; unsigned long n; Ac* a[4]; Ac* GetAt(unsigned long i) { return a[i]; } };
struct En { void Detach(X* h) { ++g_det; } ~En() { ++g_edel; } };
struct Rg { virtual void s0() {} virtual void s1() {} virtual void s2() {} virtual void s3() {} virtual void s4() {} virtual void s5() {} virtual void s6() {} virtual void s7() {} virtual void s8() {} virtual void s9() {} virtual void s10() {} virtual void s11() {} virtual void s12() {} virtual void s13() {} virtual void s14() {} virtual void s15() {} virtual void s16() {} virtual En* Find(void* const* k) { g_lastkey = *k; return *k == g_hitkey ? new En : 0; } };
struct It { unsigned char pad[0x20]; long m20; void Reset(X* h) { ++g_reset; } }; static It g_it[2];
struct Il { unsigned char b[0x30]; void Remove(Tg* t) { ++g_rem; } It* FindNext(Tg* t) { return g_items < 2 ? &g_it[g_items++] : 0; } };
struct X { Il items; Al acts; Rg reg; void Rel(Tg* target) { Pl l(g_sec); for (unsigned long i = 0; i < acts.n; ++i) { Ac* a = acts.GetAt(i); if (a->IsActive() && a->r && a->r->t == target) a->Stop(this); } for (unsigned long k = 0; k < target->n; ++k) { void* key = target->keys[k]; En* e = reg.Find(&key); if (e) { e->Detach(this); delete e; } } items.Remove(target); It* it; while ((it = items.FindNext(target)) != 0) { it->Reset(this); it->m20 = 0; } } };
static bool Check() { Tg t; Tg u; int k1, k2, k3; void* keys[3] = { &k1, &k2, &k3 }; t.keys = keys; t.n = 3; g_hitkey = &k2; Rec r0 = { 0, &t }; Rec r3 = { 0, &u }; Ac a[4]; a[0].r = &r0; a[0].active = 1; a[1].r = &r0; a[1].active = 0; a[2].r = 0; a[2].active = 1; a[3].r = &r3; a[3].active = 1; X* x = new X; x->acts.n = 4; for (int i = 0; i < 4; ++i) x->acts.a[i] = &a[i]; g_it[0].m20 = 5; g_it[1].m20 = 6; x->Rel(&t); delete x; return g_int == 1 && g_det == 1 && g_edel == 1 && g_rem == 1 && g_reset == 2 && g_it[0].m20 == 0 && g_it[1].m20 == 0 && g_lastkey == &k3 && g_lk == 0; }

int main()
{
    if (!Check()) { std::printf("00c12140_TEST FAIL\n"); return 1; }
    std::printf("00c12140_TEST PASS\n");
    return 0;
}