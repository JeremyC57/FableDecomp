#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
static int g_lk = 0; static unsigned char g_sec[8]; struct Pl { void* s; unsigned long t; Pl(void* x) : s(x), t(0) { ++g_lk; } ~Pl() { --g_lk; } };
static int g_wait = 0, g_handled = 0; static long g_msg = 0; static unsigned long g_count = 1;
struct Ev { void* h; void Wait() { ++g_wait; } }; struct H { virtual void Handle(long m) { ++g_handled; g_msg = m; } virtual void S4() {} virtual unsigned long Count() { return g_count; } };
struct M { unsigned char pad[0x18]; H* h; Ev ev; long Dispatch(long m) { ev.Wait(); Pl l(this); if (!(h->Count() > 0)) WARN("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCMailbox.h", 63, "the queue is out of step with the mailbox handler"); h->Handle(m); return m; } };
static bool Check() { M m; H h; m.h = &h; if (m.Dispatch(42) != 42 || g_wait != 1 || g_handled != 1 || g_msg != 42 || g_lines || g_lk != 0) return false; g_count = 0; if (m.Dispatch(7) != 7) return false; return g_lines == 1 && g_trace == 1 && std::strstr(g_buf, "(63) : the queue is out of step") != 0 && g_msg == 7 && g_lk == 0; }

int main()
{
    if (!Check()) { std::printf("00c1da30_TEST FAIL\n"); return 1; }
    std::printf("00c1da30_TEST PASS\n");
    return 0;
}