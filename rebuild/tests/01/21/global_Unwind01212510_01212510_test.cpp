#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
struct Rp { char f; void* m; }; static long g_post = 0; static char g_f = 0; static void* g_m = 0; struct Q { long Post(Rp* r) { g_f = r->f; g_m = r->m; return g_post; } };
struct X { long m0; Q* q; void Reply(char f, void* m) { Rp r; r.f = f; r.m = m; if (q->Post(&r)) { { Ln line; line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCASyncFileReader.h")->Append("(")->AppendNumber(41)->Append(") : ")->Append("Cannot reply to the user mailbox")->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); } } };
static bool Check() { Q q; X x; x.q = &q; int m; x.Reply(3, &m); if (g_f != 3 || g_m != &m || g_lines) return false; g_post = 1; x.Reply(4, 0); return g_f == 4 && g_lines == 1 && g_trace == 1 && std::strstr(g_buf, "(41) : Cannot reply") != 0; }

int main()
{
    if (!Check()) { std::printf("00c1db60_TEST FAIL\n"); return 1; }
    std::printf("00c1db60_TEST PASS\n");
    return 0;
}