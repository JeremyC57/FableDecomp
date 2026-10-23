#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
#include <cstdlib>
static int g_ctor = 0, g_reg = 0; static bool g_fail = false; static void* g_key = 0; static void* g_regd = 0; struct En { unsigned char b[0x30]; En(void* k) { ++g_ctor; g_key = k; } static void* operator new(size_t n) throw() { return g_fail ? 0 : std::malloc(n); } static void operator delete(void* p) { std::free(p); } };
struct X { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void Reg(En* e) { ++g_reg; g_regd = e; } En* Create(void* const* k) { En* e = new En(*k); if (!e) WARN("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoMap.h", 40, "EMEM"); Reg(e); return e; } };
static bool Check() { X x; int k; void* kp = &k; En* e = x.Create(&kp); if (!e || g_ctor != 1 || g_key != &k || g_reg != 1 || g_regd != e || g_lines) return false; delete e; g_fail = true; return x.Create(&kp) == 0 && g_reg == 2 && g_regd == 0 && g_lines == 1 && g_trace == 1 && std::strstr(g_buf, "(40) : EMEM") != 0; }

int main()
{
    if (!Check()) { std::printf("00c31560_TEST FAIL\n"); return 1; }
    std::printf("012140b0_TEST PASS\n");
    return 0;
}