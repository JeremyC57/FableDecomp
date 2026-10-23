#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber(LINE)->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
struct Lt { unsigned char b[0x18]; }; struct X { long count; unsigned char p[0xc]; Lt lt; bool fin; void OnFin() { L l(&lt); if (fin) WARN(".\\PKRACMasterObject.cpp", 71, "Dependancy finalised... this could be a dead object..."); ++count; } };
static bool Check() { X x; x.count = 0; x.fin = false; x.OnFin(); if (x.count != 1 || g_lines) return false; x.fin = true; x.OnFin(); return x.count == 2 && g_lines == 1 && g_trace == 1 && g_l == 0 && std::strstr(g_buf, "(71) : Dependancy") != 0; }

int main()
{
    if (!Check()) { std::printf("00c4c6c0_TEST FAIL\n"); return 1; }
    std::printf("01215f8f_TEST PASS\n");
    return 0;
}