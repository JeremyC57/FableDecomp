#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
static int g_ev = 0, g_ct = 0, g_pr = 0; static bool g_fail = false; static void* g_param = 0; static unsigned long g_stack = 0; static long g_prio = 0; static void* g_h = 0;
static void* Ct(void* a, unsigned long s, unsigned long (*f)(void*), void* p, unsigned long fl, unsigned long* id) { ++g_ct; g_stack = s; g_param = p; return g_fail ? 0 : (void*)&g_ct; } static int Sp(void* h, long p) { ++g_pr; g_h = h; g_prio = p; return 1; } static unsigned long Proc(void* p) { return 0; }
struct Sp2 { unsigned long stack; long prio; }; struct Ep { unsigned char b[4]; Ep(long a, long c) { ++g_ev; } ~Ep() {} };
struct W { void* o; void* h; Ep e; W(void* O, const Sp2* p) : o(O), h(0), e(0, 1) { unsigned long s = 0; long pr = 0; if (p) { s = p->stack; pr = p->prio; } h = Ct(0, s, Proc, this, 0, 0); if (h == 0) WARN(".\\PKCThreadManager.cpp", 23, "Problems starting thread"); if (pr) Sp(h, pr); } };
static bool Check() { int o; Sp2 p = { 0x3000, 2 }; W* w = new W(&o, &p); if (w->o != &o || w->h == 0 || g_ev != 1 || g_ct != 1 || g_stack != 0x3000 || g_param != w || g_pr != 1 || g_prio != 2 || g_h != w->h || g_lines) return false; delete w; W* v = new W(&o, 0); if (g_stack != 0 || g_pr != 1) return false; delete v; g_fail = true; W* u = new W(&o, &p); bool ok = u->h == 0 && g_lines == 1 && g_trace == 1 && std::strstr(g_buf, "(23) : Problems starting thread") != 0 && g_pr == 2; delete u; return ok; }

int main()
{
    if (!Check()) { std::printf("00c355e0_TEST FAIL\n"); return 1; }
    std::printf("00c355e0_TEST PASS\n");
    return 0;
}
