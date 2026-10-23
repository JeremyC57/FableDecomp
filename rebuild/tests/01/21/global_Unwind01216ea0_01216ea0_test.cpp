#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_res = 0; static long g_fail = 0; struct Sd { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual long Resume() { ++g_res; return g_fail; } void* h; };
struct Ls { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual long Count() { return n; } virtual Sd* At(long i) { return s[i]; } long a, b; long n; Sd* s[3]; };
struct St { unsigned char p[8]; long state; unsigned char q[0x20]; void* sec; };
struct Bk : Ls, St { static long Resume(St* st) { L l(st->sec); if (st->state == 0) { Ls& ls = static_cast<Bk&>(*st); long n = ls.Count(); for (long i = 0; i < n; ++i) { Sd* s = ls.At(i); if (s->h) { long r = s->Resume(); if (r < 0) return r; } } } st->state = 1; return 0; } };
static bool Check() { Bk b; Sd s0, s1, s2; int h; s0.h = &h; s1.h = 0; s2.h = &h; b.n = 3; b.s[0] = &s0; b.s[1] = &s1; b.s[2] = &s2; b.sec = &g_g; b.state = 0; if (Bk::Resume(&b) != 0 || g_res != 2 || b.state != 1) return false; if (Bk::Resume(&b) != 0 || g_res != 2) return false; b.state = 0; g_fail = -3; return Bk::Resume(&b) == -3 && g_res == 3 && b.state == 0 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00caabe0_TEST FAIL\n"); return 1; }
    std::printf("01216ea0_TEST PASS\n");
    return 0;
}