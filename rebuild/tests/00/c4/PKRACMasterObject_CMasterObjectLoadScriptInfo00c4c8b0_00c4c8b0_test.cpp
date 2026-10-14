#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
static int g_lk = 0; static unsigned char g_sec[8]; struct Pl { void* s; unsigned long t; Pl(void* x) : s(x), t(0) { ++g_lk; } ~Pl() { --g_lk; } };
#include <cstdlib>
static int g_ag = 0, g_des = 0, g_load = 0, g_reset = 0, g_commit = 0, g_cnt = 0; static bool g_fail = false, g_loadr = true; static unsigned long g_count = 4; static void* g_owner = 0; static unsigned long g_n = 0; static void* g_a1 = 0; static void* g_a2 = 0; static void* g_a3 = 0; static void* g_la = 0; static unsigned long g_ls = 0; static void* g_lend = 0; static void* g_lbuf = 0; static void* g_lag = 0; static void* g_lcls = 0; static int g_desf = 0;
struct Mo; struct Cls { void* vt; }; static Cls g_cls;
struct Ag { virtual void Destroy(int f) { ++g_des; g_desf = f; delete this; } unsigned char buf[0x18]; Ag(Mo* o, unsigned long n, void* a, void* b, void* c) { ++g_ag; g_owner = o; g_n = n; g_a1 = a; g_a2 = b; g_a3 = c; } virtual ~Ag() {} void* GetBuffer() { return buf; } static void* operator new(size_t n) throw() { return g_fail ? 0 : std::malloc(n); } static void operator delete(void* p) { std::free(p); } };
struct Rd { virtual void v0() {} virtual bool Load(void* a, unsigned long s, void* e, void* b, Ag* g, Cls* k) { ++g_load; g_la = a; g_ls = s; g_lend = e; g_lbuf = b; g_lag = g; g_lcls = k; if (g_lk != 1) return false; return g_loadr; } virtual unsigned long GetCount() { ++g_cnt; return g_count; } };
static void* Buf(Ag* g) { return g ? g->GetBuffer() : 0; }
struct Mo { unsigned char p0[0x10]; unsigned char sec[0x18]; bool fin; void Reset() { ++g_reset; } void Commit() { ++g_commit; }
    void LoadScriptInfo(Rd* reader, void* a, unsigned long stride, char* b, void* c, unsigned long d) { Pl lock(sec); if (fin) WARN(".\\PKRACMasterObject.cpp", 98, "Dependancy already finalised - this could be a dead object!"); unsigned long count = reader->GetCount(); Ag* agents = new Ag(this, count * stride, (void*)d, b, c); if (!agents) WARN(".\\PKRACMasterObject.cpp", 105, "EMEM"); if (!reader->Load(a, stride, b + d, Buf(agents), agents, &g_cls)) { if (agents) agents->Destroy(1); Reset(); } else { Commit(); } } };
static bool Check() { Mo* m = (Mo*)std::calloc(1, sizeof(Mo)); Rd rd; int a, c; char b[64];
    m->LoadScriptInfo(&rd, &a, 3, b, &c, 7); if (g_lines || g_cnt != 1 || g_ag != 1 || g_owner != m || g_n != 12 || g_a1 != (void*)7 || g_a2 != b || g_a3 != &c || g_load != 1 || g_la != &a || g_ls != 3 || g_lend != b + 7 || g_lag == 0 || g_lbuf != ((Ag*)g_lag)->buf || g_lcls != &g_cls || g_commit != 1 || g_reset || g_des || g_lk) return false; ((Ag*)g_lag)->Destroy(0); g_des = 0;
    g_loadr = false; m->LoadScriptInfo(&rd, &a, 3, b, &c, 7); if (g_ag != 2 || g_load != 2 || g_des != 1 || g_desf != 1 || g_reset != 1 || g_commit != 1 || g_lines) return false;
    m->fin = true; m->LoadScriptInfo(&rd, &a, 2, b, &c, 1); if (g_lines != 1 || g_trace != 1 || !std::strstr(g_buf, ".\\PKRACMasterObject.cpp(98) : Dependancy already finalised - this could be a dead object!\n") || g_n != 8 || g_reset != 2 || g_des != 2) return false;
    m->fin = false; g_fail = true; m->LoadScriptInfo(&rd, &a, 3, b, &c, 7); bool ok = g_ag == 3 && g_lines == 2 && g_trace == 2 && std::strstr(g_buf, ".\\PKRACMasterObject.cpp(105) : EMEM\n") != 0 && g_load == 4 && g_lag == 0 && g_lbuf == 0 && g_des == 2 && g_reset == 3 && g_commit == 1 && g_lk == 0; std::free(m); return ok; }

int main()
{
    if (!Check()) { std::printf("00c4c8b0_TEST FAIL\n"); return 1; }
    std::printf("00c4c8b0_TEST PASS\n");
    return 0;
}