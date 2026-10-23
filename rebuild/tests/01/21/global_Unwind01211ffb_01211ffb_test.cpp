#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
#include <cstdlib>
static bool g_fail = false; static int g_tree = 0, g_flat = 0; struct Nb {}; struct Tn : Nb { unsigned char b[0x28]; Tn(long m) { ++g_tree; } static void* operator new(size_t n) throw() { return g_fail ? 0 : std::malloc(n); } static void operator delete(void* p) { std::free(p); } }; struct Fn : Nb { unsigned char b[0x10]; Fn(long m) { ++g_flat; } static void* operator new(size_t n) throw() { return g_fail ? 0 : std::malloc(n); } static void operator delete(void* p) { std::free(p); } }; struct Bs { virtual void v0() {} Bs() {} ~Bs() {} };
struct Mn : Bs { Nb* n; Mn(long mode) : n(0) { n = mode ? (Nb*)new Tn(0) : (Nb*)new Fn(0); if (n == 0) WARN(".\\PKCTRMorphable.cpp", 12, "Error: EMEM"); } };
static bool Check() { Mn a(1); if (a.n == 0 || g_tree != 1 || g_flat != 0) return false; Mn b(0); if (b.n == 0 || g_flat != 1 || g_lines) return false; std::free(a.n); std::free(b.n); g_fail = true; Mn c(1); return c.n == 0 && g_lines == 1 && g_trace == 1 && std::strstr(g_buf, "(12) : Error: EMEM") != 0; }

int main()
{
    if (!Check()) { std::printf("00c185b0_TEST FAIL\n"); return 1; }
    std::printf("01211ffb_TEST PASS\n");
    return 0;
}