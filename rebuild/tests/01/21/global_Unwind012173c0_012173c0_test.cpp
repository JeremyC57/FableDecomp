#include <cstdio>
#include <new>
static int g_enter = 0, g_leave = 0; static void Enter(void* s) { ++g_enter; } static void Leave(void* s) { ++g_leave; } struct Lk { void* p; Lk(void* s) : p(s) { Enter(p); } ~Lk() { Leave(p); } };
#include <cstdlib>
static int g_wait = 0, g_del = 0; static void* g_waited = 0; static unsigned long Wait(void* h, unsigned long ms) { ++g_wait; g_waited = h; return 0; } struct Rc { long a, b, c, d; }; static int g_pops = 0; static Rc* g_next = 0; static int g_nullpops = 2;
struct Q { unsigned char b[0x18]; Rc* Pop() { ++g_pops; if (g_nullpops-- > 0) return 0; Rc* r = (Rc*)std::malloc(sizeof(Rc)); r->a = 1; r->b = 2; r->c = 3; r->d = 4; return r; } };
struct X { unsigned char p[0xc]; Q q; unsigned char sec[0x18]; void* ev; long waiters; void PopQ(Rc* out) { Rc* r; for (;;) { { Lk l(sec); r = q.Pop(); if (r) break; ++waiters; } Wait(ev, 0xFFFFFFFF); } *out = *r; std::free(r); ++g_del; } };
static bool Check() { int e; X* x = new X; x->ev = &e; x->waiters = 0; Rc out; out.d = 0; x->PopQ(&out); bool ok = out.a == 1 && out.d == 4 && g_pops == 3 && g_wait == 2 && g_waited == &e && x->waiters == 2 && g_enter == 3 && g_leave == 3 && g_del == 1; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00cb1030_TEST FAIL\n"); return 1; }
    std::printf("00cb1030_TEST PASS\n");
    return 0;
}