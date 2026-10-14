#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
static int g_ht = 0, g_disp = 0, g_root = 0, g_node = 0, g_ev = 0, g_ct = 0, g_sleep = 0; static bool g_fail = false; static void* g_param = 0; static unsigned long g_stack = 0; static void* g_sinkp = 0; static void* g_rootp = 0; static unsigned long g_ms = 0;
static unsigned long Proc(void* p) { return 0; } static void* Ct(void* a, unsigned long s, unsigned long (*f)(void*), void* p, unsigned long fl, unsigned long* id) { ++g_ct; g_stack = s; g_param = p; return g_fail ? 0 : (void*)0x66; } static void Sl(unsigned long ms) { ++g_sleep; g_ms = ms; }
struct Ht { unsigned char b[0x18]; Ht() { ++g_ht; } ~Ht() {} }; struct Snk { virtual void v0() {} }; struct Dp { unsigned char b[0x24]; Dp(Snk* s) { ++g_disp; g_sinkp = s; } }; struct Rt { unsigned char b[0x14]; Rt(long c) { ++g_root; } ~Rt() {} }; struct Nd { unsigned char b[0x20]; Nd(Rt* r, long c) { ++g_node; g_rootp = r; } }; struct Qu { Rt root; Nd node; Qu() : root(0x28), node(&root, 0x28) {} ~Qu() {} }; struct Ev { void* h; Ev(long m, long i) { ++g_ev; } ~Ev() {} };
struct Bs { virtual ~Bs() {} }; struct Fm : Bs { Ht ht; Snk sk; Dp dp; Qu q; Ev ev; void* thr; long m80; void* owner; Fm(void* o) : ht(), sk(), dp(&sk), q(), ev(0, 1), m80(0), owner(o) { thr = Ct(0, 0x3000, Proc, this, 0, 0); if (!thr) WARN(".\\PKCDefaultFileManager.cpp", 234, "Could not create thread"); Sl(20); } };
static bool Check() { int o; Fm* f = new Fm(&o); if (f->thr != (void*)0x66 || f->owner != &o || f->m80 != 0 || g_ht != 1 || g_disp != 1 || g_sinkp != &f->sk || g_root != 1 || g_node != 1 || g_rootp != &f->q.root || g_ev != 1 || g_ct != 1 || g_stack != 0x3000 || g_param != f || g_sleep != 1 || g_ms != 20 || g_lines) return false; delete f; g_fail = true; Fm* g = new Fm(&o); bool ok = g->thr == 0 && g_lines == 1 && g_trace == 1 && std::strstr(g_buf, "(234) : Could not create thread") != 0 && g_sleep == 2; delete g; return ok; }

int main()
{
    if (!Check()) { std::printf("00c1ac40_TEST FAIL\n"); return 1; }
    std::printf("00c1ac40_TEST PASS\n");
    return 0;
}