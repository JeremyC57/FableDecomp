#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
static int g_lk = 0; static unsigned char g_sec[8]; struct Pl { void* s; unsigned long t; Pl(void* x) : s(x), t(0) { ++g_lk; } ~Pl() { --g_lk; } };
#include <cstdlib>
static int g_dtor = 0, g_comp = 0, g_del = 0;
struct M { long dep; long target; long m8, mc; unsigned char sec[0x18]; bool fin; ~M() { ++g_dtor; } static void* operator new(size_t n) { return std::malloc(n); } static void operator delete(void* p) { ++g_del; std::free(p); } void Complete() { ++g_comp; } bool Finalize() { Pl l(sec); if (fin) WARN(".\\PKRACMasterObject.cpp", 41, "Dependancy already finalised - this could be a dead object!"); fin = true; if (dep == 0) { delete this; return false; } if (dep == target) Complete(); return true; } };
static bool Check() { M* m = new M; m->dep = 2; m->target = 3; m->fin = false; if (!m->Finalize() || g_comp || !m->fin || g_lines) return false; if (!m->Finalize() || g_lines != 1 || g_trace != 1 || !std::strstr(g_buf, "(41) : Dependancy already")) return false; m->dep = 3; if (!m->Finalize() || g_comp != 1) return false; m->dep = 0; if (m->Finalize() || g_dtor != 1 || g_del != 1) return false; return g_lk == 0; }

int main()
{
    if (!Check()) { std::printf("00c4cd80_TEST FAIL\n"); return 1; }
    std::printf("00c4cd80_TEST PASS\n");
    return 0;
}