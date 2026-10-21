// Behaviour test for vector<CActionInputControl>::operator= (0x00719540): the three assignment arms.
#include <cstdio>
#include <cstdlib>
struct E { long a; unsigned long d[6]; };
struct Vec {
    E* s; E* f; E* e;
    unsigned size() const { return unsigned(f - s); }
    unsigned capacity() const { return unsigned(e - s); }
    Vec& operator=(const Vec& x)
    {
        if (&x != this) {
            const unsigned xlen = x.size();
            if (xlen > capacity()) {
                E* tmp = xlen ? (E*)std::malloc(xlen * sizeof(E)) : 0;
                for (const E* p = x.s; p != x.f; ++p) tmp[p - x.s] = *p;
                if (s) std::free(s);
                s = tmp; e = s + xlen;
            } else if (size() >= xlen) {
                for (int i = 0; i < (int)xlen; ++i) s[i] = x.s[i];
            } else {
                unsigned n = size();
                for (unsigned i = 0; i < n; ++i) s[i] = x.s[i];
                for (unsigned i = n; i < xlen; ++i) s[i] = x.s[i];
            }
            f = s + xlen;
        }
        return *this;
    }
};
static Vec make(int n, int cap, long base)
{
    Vec v; v.s = cap ? (E*)std::malloc(cap * sizeof(E)) : 0; v.f = v.s + n; v.e = v.s + cap;
    for (int i = 0; i < n; ++i) { v.s[i].a = base + i; for (int k = 0; k < 6; ++k) v.s[i].d[k] = base + i + k; }
    return v;
}
static bool same(const Vec& a, const Vec& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned i = 0; i < a.size(); ++i) if (a.s[i].a != b.s[i].a || a.s[i].d[5] != b.s[i].d[5]) return false;
    return true;
}
int main()
{
    Vec src = make(5, 5, 100);
    Vec a = make(2, 2, 0);  a = src;           // grow: reallocate
    Vec b = make(7, 8, 0);  b = src;           // shrink within size
    Vec c = make(3, 6, 0);  c = src;           // assign overlap + construct rest
    Vec d = make(1, 1, 0);  d = d;             // self assignment
    if (!same(a, src) || a.capacity() != 5 || !same(b, src) || b.capacity() != 8 || !same(c, src) || c.capacity() != 6
        || d.size() != 1 || d.s[0].a != 0) { std::printf("00719540_TEST FAIL\n"); return 1; }
    std::printf("00719540_TEST PASS\n");
    return 0;
}