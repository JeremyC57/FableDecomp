#include <cstdio>
static int g_b = 0, g_h = 0; struct Th { int id; };
struct SD { const char* p; unsigned int n; };
struct Str { SD* d; Str() : d(0) {} Str(const char* t, long = -1) { d = new SD; d->p = t; d->n = 0; while (t[d->n]) ++d->n; } ~Str() { delete d; }
    Str& operator=(const Str& o) { if (o.d) { if (!d) d = new SD; *d = *o.d; } return *this; } };
static unsigned int Calc(unsigned int, const void*, unsigned int n) { ++g_h; return n * 7; }
struct Filter { virtual void v00() {} Str s; unsigned int h;
    Filter(const Str& nm) : s() { s = nm; const char* p; unsigned int n; if (nm.d == 0) { n = 0; p = ""; } else { n = nm.d->n; p = nm.d->p; } h = Calc(0, p, n); } };
struct B { virtual void v00() {} unsigned char body[8]; B(Th*) { ++g_b; } };
struct D : B { unsigned int gap; Filter m; Th* owner; virtual void v00() {} D(Th* t) : B(t), m("EXHAUSTED"), owner(t) {} };
int main()
{
    Th t; D d(&t);
    if (g_b != 1 || g_h != 1 || d.m.h != 9 * 7 || !d.m.s.d || d.owner != &t) { std::printf("008d82b0_TEST FAIL\n"); return 1; }
    std::printf("008d82b0_TEST PASS\n");
    return 0;
}
