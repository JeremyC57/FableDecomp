#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber(LINE)->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
#include <cstdlib>
static int g_free = 0; static void Free(void* p) { ++g_free; std::free(p); } struct Bs { virtual void v0() {} ~Bs() {} };
struct Nd : Bs { long m4; void* blk; unsigned char sent[0x14]; unsigned char* head; long m24; ~Nd() { if (head != sent) WARN(".\\PKCTRRedblackTree.cpp", 79, "Error: There are still items in this map"); if (blk) { Free(blk); blk = 0; } } };
static bool Check() { Nd* n = new Nd; n->head = n->sent; n->blk = std::malloc(8); delete n; if (g_lines || g_free != 1) return false; Nd* m = new Nd; m->head = 0; m->blk = 0; delete m; return g_lines == 1 && g_trace == 1 && g_free == 1 && std::strstr(g_buf, "(79) : Error: There are still items") != 0; }

int main()
{
    if (!Check()) { std::printf("00c19250_TEST FAIL\n"); return 1; }
    std::printf("00c19250_TEST PASS\n");
    return 0;
}
