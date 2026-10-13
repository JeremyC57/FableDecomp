#include <cstdio>
static int g_b = 0, g_h = 0; struct Th;
struct SD { const char* p; unsigned long n; }; struct Str { SD* d; Str() : d(0) {} Str(const char* t, long = -1) { d = new SD; d->p = t; d->n = 3; } ~Str() { delete d; } Str& operator=(const Str& o) { if (o.d) { if (!d) d = new SD; *d = *o.d; } return *this; } };
static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long n) { ++g_h; return n * 7; }
struct HN { virtual void v00() {} Str s; unsigned long h; HN(const Str& nm) : s() { s = nm; const char* p; unsigned long n; if (nm.d == 0) { n = 0; p = ""; } else { n = nm.d->n; p = nm.d->p; } h = Calc(0, p, n); } };
struct B { virtual void v00() {} unsigned char body[8]; B(Th*) { ++g_b; } };
struct D : B { HN m; virtual void v00() {} D(Th* t) : B(t), m("ABC") {} };
int main()
{
    D d(0); if (g_b != 1 || g_h != 1 || d.m.h != 21 || !d.m.s.d) { std::printf("00740f70_TEST FAIL\n"); return 1; }

    std::printf("00740f70_TEST PASS\n");
    return 0;
}