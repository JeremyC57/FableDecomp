#include <cstdio>
#include <new>
#include <cstring>
#include <cstdlib>
static char g_nilbuf[16] = { 0 }; static char* g_pchNil = g_nilbuf + 12; static int g_tr = 0, g_live = 0; struct Tr { Tr() { ++g_tr; } };
struct Sd { long r, len, al; }; struct Cs { char* p; Cs() : p(g_pchNil) {} Cs(const Cs& o) { Make(o.p, (int)std::strlen(o.p)); } Cs(const char* t) { Make(t, (int)std::strlen(t)); } ~Cs() { if (p != g_pchNil) { std::free((Sd*)p - 1); --g_live; } }
  void Make(const char* t, int n) { Sd* d = (Sd*)std::malloc(sizeof(Sd) + n + 1); d->r = 1; d->len = n; d->al = n; p = (char*)(d + 1); std::memcpy(p, t, n); p[n] = 0; ++g_live; } Sd* GetData() const { if (p == 0) Tr(); return (Sd*)p - 1; }
  void AllocCopy(Cs& d, long n, long i, long x) const { d.Make(p + i, (int)n); } Cs Right(long n) const { if (n < 0) n = 0; if (n >= GetData()->len) return *this; Cs d; AllocCopy(d, n, GetData()->len - n, 0); return d; } };
static bool Check() { bool ok; { Cs a("abcdef"); Cs r = a.Right(2); ok = std::strcmp(r.p, "ef") == 0; Cs w = a.Right(9); ok = ok && std::strcmp(w.p, "abcdef") == 0; Cs z = a.Right(-3); ok = ok && std::strcmp(z.p, "") == 0 && g_tr == 0; } return ok && g_live == 0; }

int main()
{
    if (!Check()) { std::printf("00c0e0f0_TEST FAIL\n"); return 1; }
    std::printf("01210f4b_TEST PASS\n");
    return 0;
}