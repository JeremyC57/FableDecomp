#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber(LINE)->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
static int g_wait = 0, g_pop = 0; static unsigned long g_count = 1; struct Q { virtual void* Pop() { ++g_pop; return (void*)&g_pop; } virtual void v1() {} virtual unsigned long GetCount() { return g_count; } }; struct Sm { long a; void Wait() { ++g_wait; } }; struct Lt {}; static int g_l = 0; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct X : Lt { unsigned char p[0x18]; Q* q; Sm sm; void* Get() { sm.Wait(); L l(this); if (!(q->GetCount() > 0)) WARN("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCMailbox.h", 63, "the queue is out of step with the mailbox handler"); return q->Pop(); } };
static bool Check() { Q q; X* x = new X; x->q = &q; if (x->Get() != &g_pop || g_lines || g_wait != 1 || g_pop != 1) return false; g_count = 0; void* r = x->Get(); bool ok = r == &g_pop && g_lines == 1 && g_trace == 1 && g_pop == 2 && g_l == 0 && std::strstr(g_buf, "(63) : the queue") != 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c4bc80_TEST FAIL\n"); return 1; }
    std::printf("01215e98_TEST PASS\n");
    return 0;
}