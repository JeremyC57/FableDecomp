#include <cstdio>
#include <new>
static int g_enter = 0, g_leave = 0; static void Enter(void* s) { ++g_enter; } static void Leave(void* s) { ++g_leave; } struct Lk { void* p; Lk(void* s) : p(s) { Enter(p); } ~Lk() { Leave(p); } };
struct Pr { void* a; void* b; }; static long g_hr = 0; static int g_calls = 0; struct Isi { virtual long v0() { return 0; } virtual long v1() { return 0; } virtual long v2() { return 0; } virtual long v3() { return 0; } virtual long v4() { return 0; } virtual long GetInfo(Pr* f, Pr* s) { ++g_calls; f->a = (void*)1; f->b = (void*)2; s->a = (void*)3; s->b = (void*)4; return g_hr; } };
struct X { unsigned char p[0x1c]; unsigned char sec[0x18]; long m34; void* fa; void* fb; void* sa; void* sb; long m48; long Init(Isi* src) { Lk l(sec); Pr f; Pr s; long r = src->GetInfo(&f, &s); if (r < 0) return r; fa = f.a; fb = f.b; sa = s.a; sb = s.b; m48 = 0; return 0; } };
static bool Check() { Isi src; X* x = new X; x->fa = 0; x->m48 = 7; g_hr = -5; if (x->Init(&src) != -5 || x->fa != 0 || x->m48 != 7) return false; g_hr = 1; bool ok = x->Init(&src) == 0 && x->fa == (void*)1 && x->fb == (void*)2 && x->sa == (void*)3 && x->sb == (void*)4 && x->m48 == 0 && g_calls == 2 && g_enter == 2 && g_leave == 2; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00cadc10_TEST FAIL\n"); return 1; }
    std::printf("00cadc10_TEST PASS\n");
    return 0;
}
