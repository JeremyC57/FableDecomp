#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
#include <cstdlib>
static bool g_fail = false; static int g_allocs = 0, g_frees = 0;
static long* Alloc(unsigned long n) { ++g_allocs; return g_fail ? 0 : (long*)std::malloc(n * sizeof(long)); } static void Free(void* p) { ++g_frees; std::free(p); }
struct A { long* p; unsigned long n; A() : p(0), n(0) {}
    void Resize(unsigned long count) { if (count == n) return; if (count == 0) { if (n) { Free(p); p = 0; } n = 0; return; } long* b = Alloc(count); if (!b) WARN("PKCAutoDeleteArray.h", 57, "EMEM"); unsigned long keep = count; if (n < count) keep = n; if (keep) { for (unsigned long i = 0; i < keep; ++i) b[i] = p[i]; } if (p) Free(p); p = b; n = count; } };
static bool Check() { A a; a.Resize(3); if (a.n != 3 || !a.p || g_allocs != 1) return false; a.p[0] = 7; a.p[1] = 8; a.p[2] = 9; a.Resize(3); if (g_allocs != 1) return false; a.Resize(5); if (a.n != 5 || a.p[0] != 7 || a.p[2] != 9 || g_frees != 1) return false; a.Resize(2); if (a.n != 2 || a.p[1] != 8 || g_frees != 2) return false; a.Resize(0); if (a.n != 0 || a.p != 0 || g_frees != 3 || g_lines) return false; g_fail = true; a.Resize(2); return g_lines == 1 && g_trace == 1 && std::strstr(g_buf, "(57) : EMEM") != 0 && a.p == 0 && a.n == 2; }

int main()
{
    if (!Check()) { std::printf("00c31810_TEST FAIL\n"); return 1; }
    std::printf("00c31810_TEST PASS\n");
    return 0;
}
