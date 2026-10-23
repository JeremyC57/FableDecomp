#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
static int g_lk = 0; static unsigned char g_sec[8]; struct Pl { void* s; unsigned long t; Pl(void* x) : s(x), t(0) { ++g_lk; } ~Pl() { --g_lk; } };
#include <cstdlib>
static int g_stop = 0, g_rel = 0, g_des = 0, g_pop = 0, g_fr = 0, g_d8 = 0, g_dr = 0, g_de = 0, g_d48 = 0; static long g_playing = 1; static int g_null = 1;
static long IsPlaying(void* s) { return g_playing; } static void Stop(void* s, long a, long b) { ++g_stop; } static void Rel(void* s) { ++g_rel; } static void Des(void* s) { ++g_des; }
struct Fr { ~Fr() { ++g_fr; } }; struct P8 { unsigned char b[0x14]; ~P8() { ++g_d8; } }; struct Ht { unsigned char b[0x1c]; ~Ht() { ++g_dr; } }; struct Ev { void* h; ~Ev() { ++g_de; } }; struct Qu { Ht root; Ev ev; Fr* Pop() { ++g_pop; return (g_pop == g_null) ? 0 : new Fr; } ~Qu() {} }; struct P48 { unsigned char b[4]; ~P48() { ++g_d48; } };
struct Bs { virtual ~Bs() {} }; struct Cs2 : Bs { void* src; P8 p8; Qu q; unsigned long frames; long m40, m44; P48 p48; ~Cs2() { { Pl l(g_sec); if (IsPlaying(src)) Stop(src, 0, 0); Rel(src); Des(src); src = 0; } for (unsigned long i = 0; i < frames; ++i) { Fr* f = q.Pop(); if (!f) WARN(".\\CCodaFrameStreamer.cpp", 84, "Null frame") else delete f; } } };
static bool Check() { Cs2* c = new Cs2; c->src = &c; c->frames = 3; delete c; return g_stop == 1 && g_rel == 1 && g_des == 1 && g_lk == 0 && g_pop == 3 && g_fr == 2 && g_lines == 1 && g_trace == 1 && std::strstr(g_buf, "(84) : Null frame") != 0 && g_d8 == 1 && g_dr == 1 && g_de == 1 && g_d48 == 1; }

int main()
{
    if (!Check()) { std::printf("00c49650_TEST FAIL\n"); return 1; }
    std::printf("01215b9a_TEST PASS\n");
    return 0;
}