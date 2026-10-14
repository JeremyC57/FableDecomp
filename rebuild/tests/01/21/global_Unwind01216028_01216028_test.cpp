#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber(LINE)->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
#include <cstdlib>
static int g_ctor = 0; static bool g_fail = false; static void* g_n = 0; static void* g_v = 0; struct Cv { unsigned char b[0x30]; Cv(void* n, void* v) { ++g_ctor; g_n = n; g_v = v; } static void* operator new(size_t n) throw() { return g_fail ? 0 : std::malloc(n); } static void operator delete(void* p) { std::free(p); } };
static Cv* Create(void* n, void* v) { Cv* c = new Cv(n, v); if (!c) WARN(".\\PKRACMasterObject.cpp", 146, "EMEM"); return c; }
static bool Check() { int n, v; Cv* c = Create(&n, &v); if (!c || g_ctor != 1 || g_n != &n || g_v != &v || g_lines) return false; delete c; g_fail = true; return Create(&n, &v) == 0 && g_lines == 1 && g_trace == 1 && g_ctor == 1 && std::strstr(g_buf, "(146) : EMEM") != 0; }

int main()
{
    if (!Check()) { std::printf("00c4cc90_TEST FAIL\n"); return 1; }
    std::printf("00c4cc90_TEST PASS\n");
    return 0;
}