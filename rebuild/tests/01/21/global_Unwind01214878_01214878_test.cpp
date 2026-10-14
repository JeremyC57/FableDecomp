#include <cstdio>
static int g_l = 0, g_a = 0, g_d = 0, g_n = 0, g_b = 0; static char g_buf[4]; static char* g_p = g_buf; struct Q;
struct L { Q* q; long s; L(Q* x) : q(x), s(0) { ++g_l; } ~L() { --g_l; } }; struct Sem { long v; void A() { ++g_a; } };
struct M { virtual void* D(void* m) { ++g_d; return 0; } virtual void b() {} virtual unsigned long C() { return (unsigned long)g_d; } };
struct D { char* p; D(char* b) : p(b) {} D& __fastcall operator<<(const char*) { ++g_n; return *this; } D& __fastcall operator<<(long) { ++g_n; return *this; } ~D() {} };
struct B { B() { ++g_b; } }; static void (__fastcall* g_f)(const char*) = 0;
struct Q { unsigned char p[0x18]; M* m; Sem s; void* P(void* x) { s.A(); L l(this); if (!(m->C() > 0)) { { D d(g_p); d << "a" << "(" << 3 << ")"; if (g_f) g_f(d.p); } B(); } m->D(x); return x; } };
int main()
{
    M m; Q q; q.m = &m; int v; if (q.P(&v) != &v || g_a != 1 || g_d != 1 || g_b != 1 || g_n != 4 || g_l != 0) { std::printf("00c392f0_TEST FAIL\n"); return 1; }
    if (q.P(&v) != &v || g_b != 1) { std::printf("00c392f0_TEST FAIL\n"); return 1; }

    std::printf("00c392f0_TEST PASS\n");
    return 0;
}