#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber(LINE)->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
#include <cstdlib>
static int g_free = 0; static void Free(void* p) { ++g_free; std::free(p); } struct Bs { virtual void v0() {} ~Bs() {} };
struct X : Bs { void* data; long m8; long count; ~X() { if (count) WARN(".\\PKCTRArray.cpp", 59, "Error: this array collection still has items in it!"); if (data) { Free(data); data = 0; } } };
static bool Check() { X* x = new X; x->count = 0; x->data = std::malloc(4); delete x; if (g_lines || g_free != 1) return false; X* y = new X; y->count = 2; y->data = 0; delete y; return g_lines == 1 && g_trace == 1 && g_free == 1 && std::strstr(g_buf, "(59) : Error: this array") != 0; }

int main()
{
    if (!Check()) { std::printf("00c32bf0_TEST FAIL\n"); return 1; }
    std::printf("01214288_TEST PASS\n");
    return 0;
}