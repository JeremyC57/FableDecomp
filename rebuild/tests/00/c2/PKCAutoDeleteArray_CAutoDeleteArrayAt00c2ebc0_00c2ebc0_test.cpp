#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
struct A { long* p; unsigned long n; long* At(unsigned long i) { if (!(i < n)) { { Ln line; line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDeleteArray.h")->Append("(")->AppendNumber(77L)->Append(") : ")->Append("Index ")->AppendNumber((long)i)->Append(" is out of range (")->AppendNumber((long)n)->Append(")")->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); } return &p[i]; } };
static bool Check() { long d[3] = { 1, 2, 3 }; A a; a.p = d; a.n = 3; if (a.At(1) != &d[1] || g_lines) return false; long* q = a.At(5); return q == d + 5 && g_lines == 1 && g_trace == 1 && std::strstr(g_buf, "(77) : Index 5 is out of range (3)") != 0; }

int main()
{
    if (!Check()) { std::printf("00c2ebc0_TEST FAIL\n"); return 1; }
    std::printf("00c2ebc0_TEST PASS\n");
    return 0;
}