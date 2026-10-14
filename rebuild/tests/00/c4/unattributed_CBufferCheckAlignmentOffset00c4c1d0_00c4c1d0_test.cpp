#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } Ln* AppendNumber(unsigned long v) { char n[16]; std::sprintf(n, "%lu", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
struct Bf { long a, b, c; unsigned long al; unsigned long Chk(unsigned long n) { unsigned long a2 = al; if (n % a2 != 0) WARN("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKRACBuffer.cpp", 81, "your byte request ("); return n / al; } };
static bool Check() { Bf b; b.al = 16; if (b.Chk(64) != 4 || g_lines) return false; return b.Chk(70) == 4 && g_lines == 1 && g_trace == 1 && std::strstr(g_buf, "(81) : your byte") != 0; }

int main()
{
    if (!Check()) { std::printf("00c4c1d0_TEST FAIL\n"); return 1; }
    std::printf("00c4c1d0_TEST PASS\n");
    return 0;
}