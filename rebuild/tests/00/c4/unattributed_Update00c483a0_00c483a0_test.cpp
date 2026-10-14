#include <cstdio>
#include <new>
static int g_ip = 0; struct Ip { unsigned char b[8]; Ip() { ++g_ip; } ~Ip() { --g_ip; } }; struct Pr { void* a; void* b; };
struct Rq { long t; long m4; long m8, mc, m10; Ip th; long m1c; void* t20; void* m24; long m28, m2c, m30; Pr p34; void* t3c; Rq(long T) : t(T), m8(0), mc(0), m10(0), th() {} void*& T20() { return t20; } Pr& P34() { return p34; } void*& T3c() { return t3c; } };
static Rq* g_issued = 0; static long g_type = 0; static void* g_v = 0; static Pr g_p;
struct X { void Issue(Rq* r) { g_issued = r; g_type = r->t; g_p = r->p34; } void Update(Pr pr) { Rq r(4); r.P34() = pr; Issue(&r); } };
static bool Check() { X x; int a, b; Pr pr; pr.a = &a; pr.b = &b; x.Update(pr); return g_type == 4 && g_p.a == &a && g_p.b == &b && g_ip == 0; }

int main()
{
    if (!Check()) { std::printf("00c483a0_TEST FAIL\n"); return 1; }
    std::printf("00c483a0_TEST PASS\n");
    return 0;
}