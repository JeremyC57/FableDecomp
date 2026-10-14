#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
#include <cstdlib>
static int g_base = 0; static bool g_fail = false; static void* g_owner = 0;
struct Sv { virtual ~Sv() {} void* owner; Sv(void* o) : owner(o) { ++g_base; g_owner = o; } };
struct Ag : Sv { unsigned long n; void* c; void* d; unsigned char* buf; void* e; static void* operator new(size_t s) { return std::malloc(s); } static void operator delete(void* p) { std::free(p); }
    Ag(void* o, unsigned long count, void* a, void* b, void* f) : Sv(o), n(count), c(a), d(b), buf(0), e(f) { if (!(count > 0)) WARN(".\\PKRASlaveObject_Local.cpp", 15, "Expecting to be reading some data!"); buf = g_fail ? 0 : (unsigned char*)std::malloc(count ? count : 1); if (!buf) WARN(".\\PKRASlaveObject_Local.cpp", 17, "EMEM"); } ~Ag() { std::free(buf); } };
static bool Check() { int o, a, b, f; Ag* g = new Ag(&o, 8, &a, &b, &f); if (g->owner != &o || g_owner != &o || g->n != 8 || g->c != &a || g->d != &b || g->e != &f || !g->buf || g_lines) return false; delete g; Ag* h = new Ag(&o, 0, &a, &b, &f); if (g_lines != 1 || g_trace != 1 || !std::strstr(g_buf, "(15) : Expecting to be reading some data!")) return false; delete h; g_fail = true; Ag* k = new Ag(&o, 4, &a, &b, &f); bool ok = k->buf == 0 && g_lines == 2 && g_trace == 2 && std::strstr(g_buf, "(17) : EMEM") != 0 && g_base == 3; delete k; return ok; }

int main()
{
    if (!Check()) { std::printf("00c62c70_TEST FAIL\n"); return 1; }
    std::printf("00c62c70_TEST PASS\n");
    return 0;
}