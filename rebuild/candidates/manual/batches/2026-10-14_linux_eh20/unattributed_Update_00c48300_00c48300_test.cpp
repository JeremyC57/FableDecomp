#include <cstdio>
#include <new>
static int g_ip = 0; struct Ip { unsigned char b[8]; Ip() { ++g_ip; } ~Ip() { --g_ip; } }; struct Pr { void* a; void* b; }; struct Qd { void* a; void* b; void* c; void* d; };
struct Rq { long t; long m4; long m8, mc, m10; Ip th; long m1c; void* t20; Qd q24; Pr p34; void* t3c; Rq(long T) : t(T), m8(0), mc(0), m10(0), th() {} void*& T20() { return t20; } Qd& Q24() { return q24; } Pr& P34() { return p34; } void*& T3c() { return t3c; } };
static long g_type = 0; static Qd g_q;
struct X { void Issue(Rq* r) { g_type = r->t; g_q = r->q24; } void Update(Qd q) { Rq r(3); r.Q24() = q; Issue(&r); } };
static bool Check() { X x; int a, b, c, d; Qd q; q.a = &a; q.b = &b; q.c = &c; q.d = &d; x.Update(q); return g_type == 3 && g_q.a == &a && g_q.d == &d && g_ip == 0; }

int main()
{
    if (!Check()) { std::printf("00c48300_TEST FAIL\n"); return 1; }
    std::printf("00c48300_TEST PASS\n");
    return 0;
}
