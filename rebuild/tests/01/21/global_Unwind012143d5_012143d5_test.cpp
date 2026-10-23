#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
static int g_run = 0, g_rel = 0; struct Ow { virtual void Run() { ++g_run; } }; struct Sm { long a; void Release() { ++g_rel; } }; struct Tm { Ow* o; long m4; Sm s; };
struct X { long P(Tm* m) { if (!m) { { Ln line; line.Append(".\\PKCThreadManager.cpp")->Append("(")->AppendNumber(6)->Append(") : ")->Append("Null thread manager")->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); } m->o->Run(); m->s.Release(); return 0; } };
static bool Check() { Ow o; Tm m; m.o = &o; X x; return x.P(&m) == 0 && g_run == 1 && g_rel == 1 && g_lines == 0 && g_trace == 0; }

int main()
{
    if (!Check()) { std::printf("00c35510_TEST FAIL\n"); return 1; }
    std::printf("012143d5_TEST PASS\n");
    return 0;
}