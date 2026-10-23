#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
static int g_mem = 0; struct Mb { unsigned char b[0x18]; ~Mb() { ++g_mem; } };
struct X { unsigned char p[0x10]; Mb m; bool fin; ~X() { if (!fin) { { Ln line; line.Append(".\\PKRACMasterObject.cpp")->Append("(")->AppendNumber(8)->Append(") : ")->Append("If it was not finalised, then how did I get here?")->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); } } };
static bool Check() { X* x = new X; x->fin = true; delete x; if (g_lines || g_mem != 1) return false; X* y = new X; y->fin = false; delete y; return g_lines == 1 && g_trace == 1 && g_mem == 2 && std::strstr(g_buf, "(8) : If it") != 0; }

int main()
{
    if (!Check()) { std::printf("00c4c370_TEST FAIL\n"); return 1; }
    std::printf("01215f4b_TEST PASS\n");
    return 0;
}