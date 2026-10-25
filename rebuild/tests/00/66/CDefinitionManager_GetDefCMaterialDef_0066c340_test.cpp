#include <cstdio>
static int g_d = 0; struct R { long c; void Dec() { --c; } long G() const { return c; } }; struct V { virtual void a() {} virtual void X() { ++g_d; } };
struct D : V, R { void Rel() { if (--c == 0) X(); } void RelT() { Dec(); if (G() == 0) X(); } };
struct P { D* p; P() {} P(D* q) : p(q) { if (p) ++p->c; } ~P() { if (p) p->RelT(); } P& operator=(const P& o) { if (p != o.p) { if (p) { p->Rel(); p = 0; } p = o.p; if (p) ++p->c; } return *this; } };
static D g_def; struct M { P G(long k) { return P(k == 1 ? &g_def : 0); }
    bool GD(long k, P& o) { if (k <= 0) return false; P d = G(k); if (d.p) { o = d; return true; } return false; } };
int main()
{
    g_def.c = 1; M m; P o; o.p = 0; if (!m.GD(1, o) || o.p != &g_def || g_def.c != 2 || m.GD(2, o) || m.GD(0, o) || g_def.c != 2) { std::printf("0066c340_TEST FAIL\n"); return 1; }

    std::printf("0066c340_TEST PASS\n");
    return 0;
}