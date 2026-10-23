#include <cstdio>
#include <new>
struct X; static int g_n = 0, g_b = 0; static long g_m = 5; struct Fl { unsigned char b[0x10]; Fl(void* e, long m) { ++g_n; g_m = m; } void Bind(X* g) { ++g_b; } }; struct Tb { void** e; long m4; unsigned long c; }; struct Hd { unsigned char p[0x14]; Tb t; }; struct Ow { unsigned char p[8]; Hd* h; };
struct X { unsigned char p[4]; Ow* o; void Init() { Tb* t = &o->h->t; for (unsigned long i = 0; i < t->c; ++i) { Fl* f = new Fl(t->e[i], 0); f->Bind(this); } } };
static bool Check() { void* e[3] = { 0, 0, 0 }; Hd h; h.t.e = e; h.t.c = 3; Ow o; o.h = &h; X x; x.o = &o; x.Init(); return g_n == 3 && g_b == 3 && g_m == 0; }

int main()
{
    if (!Check()) { std::printf("00c44c50_TEST FAIL\n"); return 1; }
    std::printf("01215330_TEST PASS\n");
    return 0;
}