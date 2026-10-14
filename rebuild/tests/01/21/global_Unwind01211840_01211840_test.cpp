#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
struct En { unsigned char b[0xb4]; }; static En g_d[3];
struct Ar { long m0; unsigned long count; long m8, mc; En* data; En* Get(unsigned long i) { if (i >= count) { { Ln line; line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h")->Append("(")->AppendNumber(31)->Append(") : ")->Append("Error: Fralloc overflow")->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); } return &data[i]; } };
static bool Check() { Ar a; a.count = 2; a.data = g_d; if (a.Get(1) != &g_d[1] || g_lines) return false; En* e = a.Get(2); return e == &g_d[2] && g_lines == 1 && g_trace == 1 && g_sunk == g_buf && std::strstr(g_buf, "(31) : Error: Fralloc overflow") != 0; }

int main()
{
    if (!Check()) { std::printf("00c13620_TEST FAIL\n"); return 1; }
    std::printf("00c13620_TEST PASS\n");
    return 0;
}