#include <cstdio>
#include <new>
static int g_lk = 0, g_pa = 0, g_pb = 0, g_bind = 0, g_res = 0; static void* g_tgt = 0; static void* g_att = 0; static long g_flags = -1; static bool g_hit = true; static unsigned char g_sec[8];
struct Pl { void* s; unsigned long t; Pl(void* x) : s(x), t(0) { ++g_lk; } ~Pl() { --g_lk; } }; struct Pa { unsigned char b[0x1c]; Pa() { ++g_pa; } }; struct Pb { unsigned char b[0x10]; Pb() { ++g_pb; } };
struct Q { Pa a; long m1c, m20; float m24, m28; long m2c; Pb b; float m40; short m44; float m48, m4c; Q() : a(), m1c(0), m20(0), m24(1.0f), m28(1.0f), m2c(0), b(), m40(0.0f), m44(0), m48(0.0f), m4c(0.0f) {} void Bind(void* t) { ++g_bind; g_tgt = t; } }; struct Ht { unsigned char p[0xc]; long v; }; static Ht g_h;
struct X { Ht* Resolve(void* a, void* c, Q* q, long f) { ++g_res; g_att = a; g_flags = f; return (g_hit && q->m24 == 1.0f && q->m2c == 0) ? &g_h : 0; } long RB(void* a, void* c, void* t) { Pl l(g_sec); if (!a) return -1; Q q; q.Bind(t); Ht* h = Resolve(a, c, &q, 0); long r; if (h) r = h->v; else r = -1; return r; } };
static bool Check() { X x; int a, t; g_h.v = 42; if (x.RB(0, 0, &t) != -1 || g_res) return false; if (x.RB(&a, 0, &t) != 42 || g_pa != 1 || g_pb != 1 || g_bind != 1 || g_tgt != &t || g_att != &a || g_flags != 0) return false; g_hit = false; return x.RB(&a, 0, &t) == -1 && g_lk == 0; }

int main()
{
    if (!Check()) { std::printf("00c12d50_TEST FAIL\n"); return 1; }
    std::printf("00c12d50_TEST PASS\n");
    return 0;
}