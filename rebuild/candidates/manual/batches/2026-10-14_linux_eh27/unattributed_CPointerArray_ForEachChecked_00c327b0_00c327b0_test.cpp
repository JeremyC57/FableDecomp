#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } Ln* AppendNumber(unsigned long v) { char n[16]; std::sprintf(n, "%lu", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
static int g_vis = 0; static void** g_last = 0; struct Vs { virtual void Visit(void** s) { ++g_vis; g_last = s; } }; struct Ar { long m0; void** d; long m8; unsigned long n; void ForEach(Vs* v) { for (unsigned long i = 0; i < n; ++i) { if (d[i] == 0) WARN(".\\PKCTRArray.cpp", 220, "Error: Null item"); v->Visit(&d[i]); } } };
static bool Check() { int a; void* d[3] = { &a, 0, &a }; Ar r; r.d = d; r.n = 3; Vs v; r.ForEach(&v); if (g_vis != 3 || g_last != &d[2] || g_lines != 1 || g_trace != 1 || std::strstr(g_buf, "(220) : Error: Null item") == 0) return false; r.n = 0; r.ForEach(&v); return g_vis == 3; }

int main()
{
    if (!Check()) { std::printf("00c327b0_TEST FAIL\n"); return 1; }
    std::printf("00c327b0_TEST PASS\n");
    return 0;
}
