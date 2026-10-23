#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
#include <cstdlib>
static int g_reg = 0, g_use = 0, g_create = 0, g_reset = 0, g_nm = 0; static long g_used = -1; static bool g_havenew = true; static void* g_req = 0;
struct Nm { unsigned char b[8]; Nm& operator=(const Nm& o) { ++g_nm; b[0] = o.b[0]; return *this; } }; struct Th { long m0; Nm nm; long c, d, e, f; };
struct Sl { unsigned char b[0x17f]; bool fr; bool IsFree() { return fr; } }; struct Pt { void Reset(long a, long b) { ++g_reset; } }; struct In { unsigned char pad[0x138]; Pt part; }; static In g_in;
struct Mg { unsigned char pad[0x18]; Sl slots[2]; long slot; long m31c; Th theme; unsigned char pending; void Register(void* r) { ++g_reg; g_req = r; } In* Create() { ++g_create; return g_havenew ? &g_in : 0; } void Use(long s, const Th* t) { ++g_use; g_used = s; }
    void Assign(void* r, const Th* t) { Register(r); for (unsigned long s = 0; s < 2; ++s) { if (slots[s].IsFree()) { Use(s, t); slot = s; return; } } In* i = Create(); if (!i) WARN(".\\CInstanceManager.cpp", 90, "Cannot be!"); i->part.Reset(0, 0); pending = 1; theme = *t; } };
static bool Check() { Mg* m = (Mg*)std::calloc(1, sizeof(Mg)); Th t; t.m0 = 4; t.nm.b[0] = 9; t.c = 1; t.d = 2; t.e = 3; t.f = 5; int r; m->slots[1].fr = true; m->Assign(&r, &t); if (g_reg != 1 || g_req != &r || g_use != 1 || g_used != 1 || m->slot != 1 || g_create) return false; m->slots[1].fr = false; m->Assign(&r, &t); if (g_create != 1 || g_reset != 1 || m->pending != 1 || m->theme.m0 != 4 || m->theme.nm.b[0] != 9 || m->theme.f != 5 || g_nm != 1 || g_lines) return false; bool ok = g_lines == 0 && g_trace == 0; std::free(m); return ok; }

int main()
{
    if (!Check()) { std::printf("00c2d5e0_TEST FAIL\n"); return 1; }
    std::printf("01213b22_TEST PASS\n");
    return 0;
}