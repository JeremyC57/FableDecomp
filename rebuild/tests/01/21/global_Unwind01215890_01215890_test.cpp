#include <cstdio>
static int g_l = 0, g_r = 0, g_p = 0; struct Q;
struct L { Q* q; long s; L(Q* x) : q(x), s(0) { ++g_l; } ~L() { --g_l; } }; struct Sem { long v; void R() { ++g_r; } };
struct M { virtual void a() {} virtual void Push(void*) { ++g_p; } virtual void b() {} virtual void* Peek() { return g_p ? (void*)1 : 0; } };
struct Q { unsigned char p[0x18]; M* m; Sem s; long P(void* x) { L l(this); if (m->Peek()) return 1; s.R(); m->Push(x); return 0; } };
int main()
{
    M m; Q q; q.m = &m; if (q.P(0) != 0 || g_r != 1 || g_p != 1 || g_l != 0 || q.P(0) != 1) { std::printf("00c48aa0_TEST FAIL\n"); return 1; }

    std::printf("00c48aa0_TEST PASS\n");
    return 0;
}