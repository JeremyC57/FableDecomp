#include <cstdio>
#include <new>
#include <cstring>
#include <cstdlib>
static int g_live = 0; struct Cs { char* p; Cs(const char* t) { p = (char*)std::malloc(std::strlen(t) + 1); std::strcpy(p, t); ++g_live; } Cs(const Cs& o) { p = (char*)std::malloc(std::strlen(o.p) + 1); std::strcpy(p, o.p); ++g_live; } ~Cs() { std::free(p); --g_live; } const Cs& operator=(const Cs& o) { char* n = (char*)std::malloc(std::strlen(o.p) + 1); std::strcpy(n, o.p); std::free(p); p = n; return *this; } int ReverseFind(char c) const { const char* f = std::strrchr(p, c); return f ? (int)(f - p) : -1; } void TrimLeft(char c) { char* s = p; while (*s == c) ++s; if (s != p) std::memmove(p, s, std::strlen(s) + 1); } void Remove(char c) { char* w = p; for (char* r = p; *r; ++r) if (*r != c) *w++ = *r; *w = 0; } Cs Left(int n) const { Cs r(p); if (n < (int)std::strlen(p)) r.p[n] = 0; return r; }
  Cs Stem() const { int pos = ReverseFind(0x5c); if (pos <= 0) pos = 0; Cs name(p + pos); name.TrimLeft(0x5c); int dot = name.ReverseFind(0x2e); if (dot >= 0) { name = name.Left(dot); name.Remove(0x2e); } return name; } };
static bool Check() { { Cs a("c:\\dir\\file.v2.txt"); Cs s = a.Stem(); if (std::strcmp(s.p, "filev2") != 0) return false; Cs b("plain"); Cs t = b.Stem(); if (std::strcmp(t.p, "plain") != 0) return false; Cs c("x.y"); Cs u = c.Stem(); if (std::strcmp(u.p, "x") != 0) return false; } return g_live == 0; }

int main()
{
    if (!Check()) { std::printf("00c0e360_TEST FAIL\n"); return 1; }
    std::printf("01210fb1_TEST PASS\n");
    return 0;
}