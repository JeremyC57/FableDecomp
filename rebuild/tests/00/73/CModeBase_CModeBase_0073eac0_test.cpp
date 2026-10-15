#include <cstdio>
static int g_b = 0, g_h = 0; struct Th { int id; };
struct SD { const char* p; unsigned int n; };
struct Str { SD* d; Str() : d(0) {} Str(const char* t, long = -1) { d = new SD; d->p = t; d->n = 0; while (t[d->n]) ++d->n; } ~Str() { delete d; }
    Str(const Str& o) : d(0) { *this = o; }
    Str& operator=(const Str& o) { if (o.d) { if (!d) d = new SD; *d = *o.d; } return *this; }
    unsigned int GetLength() const { return d == 0 ? 0 : d->n; }
    const char* GetPString() const { return d == 0 ? "" : d->p; } };
static unsigned int Calc(unsigned int, const void*, unsigned int n) { ++g_h; return n * 7; }
struct Filter { virtual void v00() {} Str s; unsigned int h;
    Filter(const Str& nm) : s() { s = nm; h = Calc(0, nm.GetPString(), nm.GetLength()); } };
struct B { virtual void v00() {} unsigned char body[8]; B(Th*) { ++g_b; } };
struct IP { virtual void v00() {} void* item; IP() : item(0) {} void Set(void* p) { item = p; } };
struct D { virtual void v00() {} Th* parent; Filter f; long frames; bool active; D(Th* t) : parent(t), f("WEAPON"), frames(-1), active(false) {} };
int main()
{
    Th t; D d(&t); Str empty; Filter e(empty);
    if (g_h != 2 || d.parent != &t || d.f.h != 42 || d.frames != -1 || d.active || e.h != 0)
    { std::printf("0073eac0_TEST FAIL\n"); return 1; }
    std::printf("0073eac0_TEST PASS\n");
    return 0;
}