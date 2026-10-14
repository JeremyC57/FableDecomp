#include <cstdio>
#include <new>
#include <cstring>
static char g_nil[1] = { 0 }; static char* g_pchNil = g_nil; static char g_buf[256]; static int g_lines = 0, g_trace = 0; static const char* g_sunk = 0;
struct Ln { char* p; Ln() : p(g_pchNil) { g_buf[0] = 0; ++g_lines; } ~Ln() {} Ln* Append(const char* t) { std::strcat(g_buf, t); p = g_buf; return this; } Ln* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat(g_buf, n); p = g_buf; return this; } }; typedef void (*Sink)(const char*); static void Sk(const char* t) { g_sunk = t; } static Sink g_sink = Sk; struct Tr { Tr() { ++g_trace; } };
#define WARN(FILE, LINE, TEXT) { { Ln line; line.Append(FILE)->Append("(")->AppendNumber((long)(LINE))->Append(") : ")->Append(TEXT)->Append("\n"); if (g_sink) g_sink(line.p); } Tr(); }
static int g_refill = 0;
struct R { unsigned long m0, m4, bs, mc, lb; unsigned char pad[0xc]; unsigned long size, first, m28, cnt, pos; void Refill() { ++g_refill; }
    bool Seek(unsigned long off) { if (off > size) return false; unsigned long block = off / bs; unsigned long res = off - block * bs; if (block >= first && block < first + cnt) { if (!(off >= first * bs)) WARN(".\\PKCBufferedReader.cpp", 414, "Error: Byte offset out of scope???|!!???"); pos = off - first * bs; } else { unsigned long pull = lb; m28 = 0; cnt = 0; first = block; pos = res; if (!(pull <= block)) pull = block; if (pull != 0) { first = block - pull; pos = res + pull * bs; } } Refill(); return true; } };
static bool Check() { R r; r.bs = 16; r.lb = 2; r.size = 1000; r.first = 10; r.m28 = 5; r.cnt = 4; r.pos = 0; if (r.Seek(2000) || g_refill) return false; if (!r.Seek(165) || r.pos != 5 || r.first != 10 || g_refill != 1) return false; if (!r.Seek(400) || r.m28 != 0 || r.cnt != 0 || r.first != 23 || r.pos != 32 || g_refill != 2) return false; r.first = 1; r.cnt = 3; if (!r.Seek(20) || r.pos != 4) return false; if (!r.Seek(5) || r.first != 0 || r.pos != 5 || r.cnt != 0) return false; return g_lines == 0 && g_refill == 4; }

int main()
{
    if (!Check()) { std::printf("00c4aa40_TEST FAIL\n"); return 1; }
    std::printf("00c4aa40_TEST PASS\n");
    return 0;
}
