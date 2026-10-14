#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
static int g_submit = 0, g_sleep = 0, g_wait = 0, g_close = 0, g_dh = 0, g_dt = 0, g_de = 0; static int g_busy = 2; static void* g_closed = 0; static void* g_key = 0;
static void Sl(unsigned long ms) { ++g_sleep; } static int Ch(void* h) { ++g_close; g_closed = h; return 1; }
struct Rq { void* h; void* key; long a, b, c, d, e; }; struct Ht { unsigned char b[0x40]; ~Ht() { ++g_dh; } }; struct Tb { unsigned char b[0x14]; ~Tb() { ++g_dt; } }; struct Qu { unsigned char b[0x20]; long Submit(const Rq* r) { ++g_submit; g_key = r->key; return g_busy-- > 0 ? 1 : 0; } }; struct Ev { void* h; void Wait() { ++g_wait; } ~Ev() { ++g_de; } };
struct Bs { virtual ~Bs() {} }; struct Fm : Bs { Ht ht; Tb tb; Qu q; Ev ev; void* thr; ~Fm() { Rq quit; quit.key = (void*)-1; while (q.Submit(&quit) == 1) { Sl(100); { Ln line; line.Append(".\\PKCDefaultFileManager.cpp")->Append("(")->AppendNumber(247L)->Append(") : ")->Append("Wait to send quit command...")->Append("\n"); if (g_sink) g_sink(line.p); } } ev.Wait(); Ch(thr); } };
static bool Check() { Fm* f = new Fm; f->thr = (void*)0x55; delete f; return g_submit == 3 && g_sleep == 2 && g_lines == 2 && g_trace == 0 && std::strstr(g_buf, "(247) : Wait to send quit command...") != 0 && g_key == (void*)-1 && g_wait == 1 && g_close == 1 && g_closed == (void*)0x55 && g_dh == 1 && g_dt == 1 && g_de == 1; }

int main()
{
    if (!Check()) { std::printf("00c1a230_TEST FAIL\n"); return 1; }
    std::printf("00c1a230_TEST PASS\n");
    return 0;
}