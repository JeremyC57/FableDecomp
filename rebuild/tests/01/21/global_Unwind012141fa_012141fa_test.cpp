#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber(LINE)->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
struct Ar { long m0; void** data; long m8; unsigned long count; void* Get(unsigned long i) { if (i >= count) return 0; if (data[i] == 0) WARN(".\\PKCTRArray.cpp", 190, "Error: NULL item in this array"); return data[i]; } };
static bool Check() { int a; void* d[2] = { &a, 0 }; Ar r; r.data = d; r.count = 2; if (r.Get(2) != 0 || r.Get(0) != &a || g_lines) return false; return r.Get(1) == 0 && g_lines == 1 && g_trace == 1 && std::strstr(g_buf, "(190) : ") != 0; }

int main()
{
    if (!Check()) { std::printf("00c324f0_TEST FAIL\n"); return 1; }
    std::printf("012141fa_TEST PASS\n");
    return 0;
}