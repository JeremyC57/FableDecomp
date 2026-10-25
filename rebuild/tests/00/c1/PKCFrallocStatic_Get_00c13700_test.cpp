// Behaviour test for 00c13700: PKCFrallocStatic<CPlaybackSlot>::Get -- in-range access returns the 0x2c-byte slot; an overflow logs the PKCFrallocStatic.h(31) warning, breaks, and still returns the slot
#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber(LINE)->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
struct Sl { unsigned char b[0x2c]; }; struct Fa { long m0; unsigned long count; long m8, mc; Sl* slots; Sl* Get(unsigned long i) { if (i >= count) WARN("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h", 31, "Error: Fralloc overflow"); return &slots[i]; } };
static bool Check() { Sl s[3]; Fa f; f.slots = s; f.count = 2; if (f.Get(1) != &s[1] || g_lines) return false; return f.Get(2) == &s[2] && g_lines == 1 && g_trace == 1 && std::strstr(g_buf, "(31) : Error: Fralloc overflow") != 0; }

int main()
{
    if (!Check()) { std::printf("00c13700_TEST FAIL\n"); return 1; }
    std::printf("00c13700_TEST PASS\n");
    return 0;
}