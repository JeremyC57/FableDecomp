#include <cstdio>
static int g_d = 0; struct R { long c; void Dec() { --c; } long G() const { return c; } }; struct V { virtual void a() {} virtual void X() { ++g_d; } };
struct D : V, R { void Rel() { if (--c == 0) X(); } void RelT() { Dec(); if (G() == 0) X(); } };
struct PB { D* p; PB() {} PB(D* q) : p(q) { if (p) ++p->c; } ~PB() { if (p) { p->RelT(); p = 0; } } };
struct P { D* p; P() {} P(const PB& o) : p(o.p) { if (p) ++p->c; } ~P() { if (p) p->Rel(); } P& operator=(const P& o) { if (p != o.p) { if (p) { p->Rel(); p = 0; } p = o.p; if (p) ++p->c; } return *this; } };
static D g_def; static long g_idx = 1; struct M { PB G(long k) { return PB(k == 1 ? &g_def : 0); } };
struct S { char x[8]; S(const char*, long) {} ~S() {} };
struct C { virtual long* I(const S&) { return g_idx > 0 ? &g_idx : 0; } M* m; bool GS(P& o) { long* pi; { S n("x", -1); pi = I(n); } if (pi) { P r(m->G(*pi)); o = r; return true; } return false; } };
int main()
{
    g_def.c = 1; M m; C c; c.m = &m; P o; o.p = 0; if (!c.GS(o) || o.p != &g_def || g_def.c != 2) { std::printf("0095c5c0_TEST FAIL\n"); return 1; }
    g_idx = 0; if (c.GS(o) || g_def.c != 2) { std::printf("0095c5c0_TEST FAIL\n"); return 1; }

    std::printf("0095c5c0_TEST PASS\n");
    return 0;
}