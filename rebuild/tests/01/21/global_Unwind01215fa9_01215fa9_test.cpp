#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
static int g_lk = 0, g_done = 0; struct Pl { void* s; unsigned long t; Pl(void* x) : s(x), t(0) { ++g_lk; } ~Pl() { --g_lk; } };
struct X { unsigned long exp, rec; long ontime; long mc; unsigned char sec[0x18]; bool fin; void Complete() { ++g_done; } void Recv(bool late) { Pl l(sec); if (!(rec < exp)) WARN(".\\PKRACMasterObject.cpp", 79, "No room for this received object... sniff sniff"); if (!late) ++ontime; if (exp == ++rec) { if (fin) Complete(); } } };
static bool Check() { X x; x.exp = 2; x.rec = 0; x.ontime = 0; x.fin = false; x.Recv(false); if (x.rec != 1 || x.ontime != 1 || g_lines || g_done) return false; x.fin = true; x.Recv(true); if (x.rec != 2 || x.ontime != 1 || g_done != 1 || g_lines) return false; x.Recv(false); return x.rec == 3 && g_lines == 1 && g_trace == 1 && g_done == 1 && g_lk == 0 && std::strstr(g_buf, "(79) : No room") != 0; }

int main()
{
    if (!Check()) { std::printf("00c4c7a0_TEST FAIL\n"); return 1; }
    std::printf("00c4c7a0_TEST PASS\n");
    return 0;
}