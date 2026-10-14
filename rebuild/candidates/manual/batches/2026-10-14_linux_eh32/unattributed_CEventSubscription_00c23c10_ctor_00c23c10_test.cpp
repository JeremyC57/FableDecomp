#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
#include <cstdlib>
static int g_mc = 0, g_failat = 0, g_init = 0; static void* g_last = 0;
static void* Ml(size_t n) { ++g_mc; if (g_mc == g_failat) return 0; return std::malloc(n); }
struct Se { unsigned char b[0x14]; };
struct Es { virtual void v0() {} unsigned long n; long m8, mc; Se* ent; long* slots; void InitEntry(Se* e) { ++g_init; g_last = e; }
    Es(unsigned long count) : n(count), m8(0), mc(0) { if (count == 0) { ent = 0; slots = 0; return; } ent = (Se*)Ml(count * sizeof(Se)); if (!ent) WARN("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h", 63, "Memory allocation failure"); slots = (long*)Ml(n * sizeof(long)); if (!slots) WARN("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h", 65, "Memory allocation failure"); for (unsigned long i = 0; i < n; ++i) InitEntry(&ent[i]); }
    ~Es() { std::free(ent); std::free(slots); } };
static bool Check() { Es* z = new Es(0); if (z->n != 0 || z->m8 || z->mc || z->ent || z->slots || g_mc || g_init || g_lines) return false; delete z;
    Es* a = new Es(3); if (a->n != 3 || a->m8 || a->mc || !a->ent || !a->slots || g_mc != 2 || g_init != 3 || g_last != &a->ent[2] || (char*)g_last - (char*)a->ent != 0x28 || g_lines) return false; delete a;
    g_failat = 3; Es* b = new Es(2); if (b->ent != 0 || !b->slots || g_lines != 1 || g_trace != 1 || !std::strstr(g_buf, "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h(63) : Memory allocation failure\n") || g_sunk != g_buf || g_init != 5) return false; delete b;
    g_failat = 6; Es* c = new Es(2); bool ok = c->ent != 0 && c->slots == 0 && g_lines == 2 && g_trace == 2 && std::strstr(g_buf, "PKCFrallocStatic.h(65) : Memory allocation failure\n") != 0 && g_init == 7 && g_mc == 6; delete c; return ok; }

int main()
{
    if (!Check()) { std::printf("00c23c10_TEST FAIL\n"); return 1; }
    std::printf("00c23c10_TEST PASS\n");
    return 0;
}
