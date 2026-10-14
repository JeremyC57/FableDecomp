#include <cstdio>
#include <new>
static int g_lk = 0, g_key = 0, g_set = 0, g_clr = 0, g_kd = 0; static void* g_so = 0; static void* g_res = 0; static long g_state = 0; static bool g_hit = true;
struct Pl { void* s; unsigned long t; Pl(void* x) : s(x), t(0) { ++g_lk; } ~Pl() { --g_lk; } }; struct Tb { void* Lookup(void* tag) { return tag; } }; struct En { long m0; void* ow; Tb tb; };
struct Kf { unsigned char b[0xc]; void Set(void* o, En* e, void* r) { ++g_set; g_so = o; g_res = r; } }; struct Ky { long m0; Kf f; Ky() { ++g_key; } void Clear() { ++g_clr; } ~Ky() { ++g_kd; } }; struct Ht { unsigned char p[0xc]; long st; }; static Ht g_h;
struct Mp { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void v9() {} virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {} virtual void v16() {} virtual Ht* Find(Ky* k) { return g_hit ? &g_h : 0; } }; static unsigned char g_sec[8];
struct X { unsigned char p[0x1f4]; Mp m; long Proc(void* tag, En* e) { Pl l(g_sec); if (e) { Ky k; k.f.Set(e->ow, e, e->tb.Lookup(tag)); Ht* h = m.Find(&k); k.Clear(); if (h) return h->st; } return -1; } };
static bool Check() { X* x = new X; g_h.st = 7; int t, o; En e; e.ow = &o; if (x->Proc(&t, 0) != -1 || g_key) return false; if (x->Proc(&t, &e) != 7 || g_key != 1 || g_set != 1 || g_so != &o || g_res != &t || g_clr != 1 || g_kd != 1) return false; g_hit = false; bool ok = x->Proc(&t, &e) == -1 && g_kd == 2 && g_lk == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c119d0_TEST FAIL\n"); return 1; }
    std::printf("00c119d0_TEST PASS\n");
    return 0;
}