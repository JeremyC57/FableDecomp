#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
static int g_rel = 0; static long g_flags = 0; struct Fo { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void Release(long f) { ++g_rel; g_flags = f; } };
static void F(Fo* f) { if (!f) { { Ln line; line.Append(".\\PKCDefaultFileManager.cpp")->Append("(")->AppendNumber(91)->Append(") : ")->Append("NULL File Object")->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); return; } f->Release(1); }
static bool Check() { Fo f; F(&f); if (g_rel != 1 || g_flags != 1 || g_lines) return false; F(0); return g_lines == 1 && g_trace == 1 && g_rel == 1 && std::strstr(g_buf, "(91) : NULL File Object") != 0; }

int main()
{
    if (!Check()) { std::printf("00c19fa0_TEST FAIL\n"); return 1; }
    std::printf("00c19fa0_TEST PASS\n");
    return 0;
}