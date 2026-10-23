#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
#include <cstdlib>
static int g_base = 0, g_th = 0; static bool g_fail = false; static long g_stack = 0, g_fl = -1; static void* g_own = 0; struct Sp { long stack; long flags; }; struct X; struct Th { unsigned char b[0xc]; Th(X* o, const Sp* p) { ++g_th; g_own = o; g_stack = p->stack; g_fl = p->flags; } static void* operator new(size_t n) throw() { return g_fail ? 0 : std::malloc(n); } static void operator delete(void* p) { std::free(p); } };
struct Bs { unsigned char b[0x38]; Bs() { ++g_base; } ~Bs() {} }; struct X : Bs { Th* th; X() : Bs(), th(0) { Sp p; p.flags = 0; p.stack = 0x3000; th = new Th(this, &p); if (th == 0) WARN(".\\CASyncDecoder.cpp", 193, "EMEM"); } };
static bool Check() { X* x = new X; if (x->th == 0 || g_base != 1 || g_th != 1 || g_own != x || g_stack != 0x3000 || g_fl != 0 || g_lines) return false; delete x->th; delete x; g_fail = true; X* y = new X; bool ok = y->th == 0 && g_lines == 1 && g_trace == 1 && std::strstr(g_buf, "(193) : EMEM") != 0; delete y; return ok; }

int main()
{
    if (!Check()) { std::printf("00c48640_TEST FAIL\n"); return 1; }
    std::printf("012159f4_TEST PASS\n");
    return 0;
}