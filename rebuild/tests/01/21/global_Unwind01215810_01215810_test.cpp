#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
static int g_acq = 0, g_lk = 0, g_pop = 0, g_md = 0; static unsigned long g_count = 2; struct Pl { void* s; unsigned long t; Pl(void* x) : s(x), t(0) { ++g_lk; } ~Pl() { --g_lk; } }; struct Ms { void* p; ~Ms() { ++g_md; } };
struct Iq { virtual Ms Pop() { Ms m; m.p = (void*)7; ++g_pop; return m; } virtual void v1() {} virtual unsigned long Count() { return g_count; } }; struct Sm { void Acquire() { ++g_acq; } };
struct X { unsigned char p[0x18]; Iq* q; Sm s; Ms Take() { s.Acquire(); Pl l(this); if (!(q->Count() > 0)) WARN("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCMailbox.h", 63, "the queue is out of step with the mailbox handler"); return q->Pop(); } };
static bool Check() { Iq q; X x; x.q = &q; { Ms m = x.Take(); if (m.p != (void*)7 || g_acq != 1 || g_pop != 1 || g_lines || g_lk != 0) return false; } g_count = 0; { Ms m = x.Take(); if (m.p != (void*)7 || g_lines != 1 || g_trace != 1 || std::strstr(g_buf, "(63) : the queue") == 0) return false; } return g_lk == 0; }

int main()
{
    if (!Check()) { std::printf("00c48860_TEST FAIL\n"); return 1; }
    std::printf("01215810_TEST PASS\n");
    return 0;
}