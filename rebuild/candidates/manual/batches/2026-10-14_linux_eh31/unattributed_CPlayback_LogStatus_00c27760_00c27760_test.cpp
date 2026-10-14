#include <cstdio>
#include <new>
#include <cstdlib>
#include <cstring>
static char g_nil[1] = { 0 };
struct Cs { char* p; Cs() : p(g_nil) {} Cs(const char* t) : p(g_nil) { Set(t); } Cs(const Cs& o) : p(g_nil) { Set(o.p); } ~Cs() { if (p != g_nil) std::free(p); } void Set(const char* t) { char* n = (char*)std::malloc(std::strlen(t) + 1); std::strcpy(n, t); if (p != g_nil) std::free(p); p = n; } const Cs& operator=(const char* t) { Set(t); return *this; } const Cs& operator=(const Cs& o) { Set(o.p); return *this; } Cs* Append(const char* t) { char* n = (char*)std::malloc(std::strlen(p) + std::strlen(t) + 1); std::strcpy(n, p); std::strcat(n, t); if (p != g_nil) std::free(p); p = n; return this; } Cs* AppendNumber(unsigned long v) { char n[16]; std::sprintf(n, "%lu", v); return Append(n); } };
static int g_lk = 0; static unsigned char g_sec[8]; struct Pl { void* s; unsigned long t; Pl(void* x) : s(x), t(0) { ++g_lk; } ~Pl() { --g_lk; } };
static long g_playing = 1; static unsigned long g_pos = 5;
static long IsPlaying(void* s) { return g_playing; } static unsigned long GetPos(void* s) { return g_pos; }
struct Pb { long m0, m4, m8; void* src; unsigned char pad[0x10]; unsigned long len; Cs LogStatus() { Pl l(g_sec); if (IsPlaying(src)) { Cs line; line = "CODA reports source IS playing"; unsigned long position = GetPos(src); line.Append("(")->AppendNumber(position)->Append("/")->AppendNumber(len)->Append(")"); return line; } return Cs("CODA reports source is NOT playing"); } };
static bool Check() { Pb p; p.src = &p; p.len = 10; Cs a = p.LogStatus(); if (std::strcmp(a.p, "CODA reports source IS playing(5/10)") || g_lk) return false; g_playing = 0; Cs b = p.LogStatus(); return std::strcmp(b.p, "CODA reports source is NOT playing") == 0 && g_lk == 0; }

int main()
{
    if (!Check()) { std::printf("00c27760_TEST FAIL\n"); return 1; }
    std::printf("00c27760_TEST PASS\n");
    return 0;
}
