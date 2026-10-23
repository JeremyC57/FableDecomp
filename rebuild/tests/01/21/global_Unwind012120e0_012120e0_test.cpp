#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
#include <cstdlib>
static bool g_fail = false; struct Sn { virtual void v0() {} static void* operator new(size_t n) throw() { return g_fail ? 0 : std::malloc(n); } static void operator delete(void* p) { std::free(p); } }; struct Lk { Lk* next; }; struct Bs { virtual void v0() {} Bs() {} ~Bs() {} };
struct Nd : Bs { Sn* root; Sn* owned; Lk link; Lk* head; long m14, m18, m1c; Lk* tail; long m24; Nd(Sn* shared) : root(shared), owned(0), tail(&link), m24(0) { if (shared == 0) { owned = new Sn; if (owned == 0) WARN(".\\PKCTRRedblackTree.cpp", 35, "Error: EMEM"); root = owned; } head = &link; link.next = &link; m14 = 0; m18 = 0; m1c = 0; } };
static bool Check() { Sn sh; Nd* a = new Nd(&sh); if (a->root != &sh || a->owned != 0 || a->tail != &a->link || a->head != &a->link || a->link.next != &a->link || a->m24 != 0 || a->m1c != 0 || g_lines) return false; Nd* b = new Nd(0); if (b->owned == 0 || b->root != b->owned || g_lines) return false; delete b->owned; g_fail = true; Nd* c = new Nd(0); bool ok = c->owned == 0 && c->root == 0 && g_lines == 1 && g_trace == 1 && std::strstr(g_buf, "(35) : Error: EMEM") != 0; delete a; delete b; delete c; return ok; }

int main()
{
    if (!Check()) { std::printf("00c194f0_TEST FAIL\n"); return 1; }
    std::printf("012120e0_TEST PASS\n");
    return 0;
}