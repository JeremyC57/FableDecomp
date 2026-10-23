#include <cstdio>
#include <new>
#include <cstring>
#include <cstdlib>
static bool g_flag = false; static int g_live = 0; static long Slen(const char* t) { return (long)std::strlen(t); }
struct Cs { char* p; Cs(const char* t) { p = (char*)std::malloc(std::strlen(t) + 1); std::strcpy(p, t); ++g_live; } Cs(const Cs& o) { p = (char*)std::malloc(std::strlen(o.p) + 1); std::strcpy(p, o.p); ++g_live; } ~Cs() { std::free(p); --g_live; } Cs Right(int n) const { int l = (int)std::strlen(p); return Cs(p + (l > n ? l - n : 0)); } int Compare(const char* t) const { return std::strcmp(p, t); }
  long Classify() const { if (g_flag) return 2; if (Slen(p) < 4) return 3; Cs tail = Right(4); if (tail.Compare(".ogg") == 0) return 1; if (tail.Compare(".wma") == 0) return 0; return 3; } };
static bool Check() { { Cs a("song.ogg"), b("x.wma"), c("abc"), d("tune.mp3"); if (a.Classify() != 1 || b.Classify() != 0 || c.Classify() != 3 || d.Classify() != 3) return false; g_flag = true; if (a.Classify() != 2) return false; } return g_live == 0; }

int main()
{
    if (!Check()) { std::printf("00c73a00_TEST FAIL\n"); return 1; }
    std::printf("01216750_TEST PASS\n");
    return 0;
}