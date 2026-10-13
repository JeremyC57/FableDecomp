#include <cstdio>
struct N { long c; N* p; N* l; N* r; unsigned long k; }; struct V { unsigned long k; };
struct It { N* n; It(N* x) : n(x) {} It(const It& o) : n(o.n) {} It& operator--() { n = n->l; return *this; } bool operator==(const It& o) const { return n == o.n; } };
struct PIB { It f; bool s; PIB(const It& i, bool b) : f(i), s(b) {} };
static int g_i = 0; struct Tr { N* h; unsigned long sz; It* Ins(It* r, N*, N* y, const V& v, N*) { ++g_i; r->n = y; return r; } It begin() { return It(h->l); }
    PIB IU(const V& v) { N* y = h; N* x = h->p; bool less = true; while (x != 0) { y = x; less = v.k < x->k; x = less ? x->l : x->r; } It j(y); if (less) { if (j == begin()) return PIB(*Ins(&j, y, y, v, 0), true); --j; } if (j.n->k < v.k) return PIB(*Ins(&j, x, y, v, 0), true); return PIB(j, false); } };
int main()
{
    N hd, n1; hd.p = &n1; hd.l = &n1; n1.p = &hd; n1.l = 0; n1.r = 0; n1.k = 5; Tr t; t.h = &hd; t.sz = 1; V v; v.k = 5; PIB r = t.IU(v); if (r.s || r.f.n != &n1 || g_i != 0) { std::printf("0069d900_TEST FAIL\n"); return 1; }
    v.k = 7; PIB r2 = t.IU(v); if (!r2.s || g_i != 1) { std::printf("0069d900_TEST FAIL\n"); return 1; }

    std::printf("0069d900_TEST PASS\n");
    return 0;
}