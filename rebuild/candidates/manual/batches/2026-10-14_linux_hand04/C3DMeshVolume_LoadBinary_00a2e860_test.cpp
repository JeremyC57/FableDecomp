#include <cstdio>
static int g_dtor = 0, g_ctor = 0, g_load = 0;
struct Str { const char* p; Str() : p(0) {} Str(const char* s) : p(s) {} ~Str() { ++g_dtor; } Str& operator=(const Str& o) { p = o.p; return *this; } };
struct Pool;
struct In { virtual void v00() {} virtual void v04() {} virtual void v08() {} virtual void v0c() {} virtual void v10() {} virtual void v14() {} virtual Str Read() { return Str("vol"); } };
struct Vol { unsigned char b[16]; Vol() { ++g_ctor; } bool Load(In*, Pool* p) { ++g_load; return p == 0; } };
struct CP { Vol* o; void* c; void Reset(Vol* v) { delete o; o = v; } };
struct MV { Str name; CP vol; bool Load(In* in, Pool* p) { name = in->Read(); vol.Reset(new Vol); return vol.o->Load(in, p); } };
int main()
{
    In in; MV mv; mv.vol.o = 0; mv.vol.c = 0;
    bool r = mv.Load(&in, 0);
    if (!r || g_ctor != 1 || g_load != 1 || g_dtor < 1 || mv.name.p[0] != 'v' || !mv.vol.o) { std::printf("00a2e860_TEST FAIL\n"); return 1; }
    delete mv.vol.o; mv.vol.o = 0;
    std::printf("00a2e860_TEST PASS\n");
    return 0;
}
