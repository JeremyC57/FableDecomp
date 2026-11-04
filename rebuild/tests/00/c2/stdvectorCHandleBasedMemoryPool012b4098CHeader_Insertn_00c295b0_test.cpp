// Behaviour test for vector<CHeader*>::_Insert_n (0x00C295B0): insert n copies at a position through the three paths
// (reallocate, spill past the end, shift inside capacity); the sequence must read back in order.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct V { int* f; int* l; int* e; V() : f(0), l(0), e(0) {} ~V() { free(f); }
    unsigned size() const { return f ? (unsigned)(l - f) : 0; } unsigned cap() const { return f ? (unsigned)(e - f) : 0; }
    void insert_n(int* where, unsigned n, int v) { int t = v; if (n == 0) return; unsigned c = cap();
        if (c < size() + n) { c = c + c / 2; if (c < size() + n) c = size() + n; int* nv = (int*)malloc(c * sizeof(int)); unsigned off = (unsigned)(where - f);
            if (off) memmove(nv, f, off * sizeof(int)); for (unsigned i = 0; i < n; ++i) nv[off + i] = t;
            memmove(nv + off + n, where, (l - where) * sizeof(int)); unsigned s = size() + n; free(f); f = nv; l = nv + s; e = nv + c; }
        else if ((unsigned)(l - where) < n) { memmove(where + n, where, (l - where) * sizeof(int)); for (int* p = where; p < where + n; ++p) *p = t; l += n; }
        else { memmove(where + n, where, (l - where) * sizeof(int)); for (int* p = where; p < where + n; ++p) *p = t; l += n; } } };
int main()
{
    V v; v.insert_n(v.l, 3, 1); v.insert_n(v.f + 1, 2, 2); v.insert_n(v.l, 1, 3);
    int want[6] = { 1, 2, 2, 1, 1, 3 };
    bool ok = v.size() == 6;
    for (int i = 0; ok && i < 6; ++i) ok = v.f[i] == want[i];
    printf(ok ? "00c295b0_TEST PASS\n" : "00c295b0_TEST FAIL\n");
    return ok ? 0 : 1;
}