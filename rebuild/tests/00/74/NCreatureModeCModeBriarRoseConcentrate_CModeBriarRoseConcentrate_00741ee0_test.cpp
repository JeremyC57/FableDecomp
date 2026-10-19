#include <cstdio>
#include <cstring>
static int g_b = 0, g_h = 0, g_ip = 0; struct Th { int id; };
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
struct IP { virtual void v00() {} void* item; IP() : item((void*)1) { ++g_ip; item = 0; } IP(void* p) : item(0) { ++g_ip; Set(p); } void Set(void* p) { item = p; } };
template <class T> struct Vec { T* a; T* b; T* c; Vec() : a(0), b(0), c(0) {} };
struct D : B { Filter m; long Timer; IP target; Vec<IP> imp; bool Imposter; void* PDef; IP body, sword;
    virtual void v00() {}
    D(Th* t) : B(t), m("POWER_UP"), Timer(0), Imposter(false), PDef(0) {} };
int main()
{
    Th t; D d(&t);
    if (g_b != 1 || g_h != 1 || d.m.h != 56 || std::strcmp(d.m.s.GetPString(), "POWER_UP") != 0 || d.Timer != 0 || d.target.item != 0
        || d.imp.a != 0 || d.imp.b != 0 || d.imp.c != 0 || d.Imposter || d.PDef != 0 || g_ip != 3 || d.body.item != 0)
    { std::printf("00741ee0_TEST FAIL\n"); return 1; }
    std::printf("00741ee0_TEST PASS\n");
    return 0;
}