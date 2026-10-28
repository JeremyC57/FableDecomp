// Behaviour test for 00a85cc0: STLport vector<CNavQuadTree::CNavigationLayer>::_M_fill_insert (model element with a counted copy).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static int g_live = 0;                                     // live element count (copy/dtor balance)
struct E {                                                 // non-POD element (the __false_type path)
    int v;
    explicit E(int a) : v(a) { ++g_live; }
    E(const E& o) : v(o.v) { ++g_live; }
    ~E() { --g_live; }
    E& operator=(const E& o) { v = o.v; return *this; }
};
template <class T> struct Vec {                            // STLport 4.6 vector model (start / finish / end_of_storage)
    T* s; T* f; T* e;
    Vec() : s(0), f(0), e(0) { }
    ~Vec() { clear(); }
    unsigned size() const { return (unsigned)(f - s); }
    unsigned cap() const { return (unsigned)(e - s); }
    static T* alloc(unsigned n) { return n ? (T*)std::malloc(n * sizeof(T)) : 0; }
    static T* ucopy(const T* a, const T* b, T* r) { for (; a != b; ++a, ++r) new (r) T(*a); return r; }
    static T* ufill(T* r, unsigned n, const T& x) { for (; n > 0; --n, ++r) new (r) T(x); return r; }
    static void destroy(T* a, T* b) { for (; a != b; ++a) a->~T(); }
    void clear() { destroy(s, f); std::free(s); s = f = e = 0; }
    void insert_overflow(T* pos, const T& x, unsigned fill_len, bool atend) {
        unsigned old = size(); unsigned len = old + (old < fill_len ? fill_len : old);
        T* ns = alloc(len); T* nf = ucopy(s, pos, ns);
        if (fill_len == 1) { new (nf) T(x); ++nf; } else nf = ufill(nf, fill_len, x);
        if (!atend) nf = ucopy(pos, f, nf);
        destroy(s, f); std::free(s); s = ns; f = nf; e = ns + len;
    }
    void push_back(const T& x) { if (f != e) { new (f) T(x); ++f; } else insert_overflow(f, x, 1, true); }
    void fill_insert(T* pos, unsigned n, const T& x) {
        if (n == 0) return;
        if ((unsigned)(e - f) >= n) {
            T xc = x; unsigned after = (unsigned)(f - pos); T* of = f;
            if (after > n) {
                ucopy(f - n, f, f); f += n;
                for (T* a = of - n, *r = of; a != pos; ) *--r = *--a;
                for (T* p = pos; p != pos + n; ++p) *p = xc;
            } else {
                ufill(f, n - after, xc); f += n - after;
                ucopy(pos, of, f); f += after;
                for (T* p = pos; p != of; ++p) *p = xc;
            }
        } else insert_overflow(pos, x, n, false);
    }
    void range_insert(T* pos, const T* first, const T* last) {
        if (first == last) return;
        unsigned n = (unsigned)(last - first);
        if ((unsigned)(e - f) >= n) {
            unsigned after = (unsigned)(f - pos); T* of = f;
            if (after > n) {
                ucopy(f - n, f, f); f += n;
                for (T* a = of - n, *r = of; a != pos; ) *--r = *--a;
                for (T* p = pos; first != last; ++first, ++p) *p = *first;
            } else {
                const T* mid = first + after;
                ucopy(mid, last, f); f += n - after;
                ucopy(pos, of, f); f += after;
                for (T* p = pos; first != mid; ++first, ++p) *p = *first;
            }
        } else {
            unsigned old = size(); unsigned len = old + (old < n ? n : old);
            T* ns = alloc(len); T* nf = ucopy(s, pos, ns); nf = ucopy(first, last, nf); nf = ucopy(pos, f, nf);
            destroy(s, f); std::free(s); s = ns; f = nf; e = ns + len;
        }
    }
    Vec& assign(const Vec& x) {
        if (&x != this) {
            unsigned xlen = x.size();
            if (xlen > cap()) {
                T* t = alloc(xlen); ucopy(x.s, x.f, t);
                destroy(s, f); std::free(s); s = t; e = s + xlen;
            } else if (size() >= xlen) {
                T* i = s; for (const T* a = x.s; a != x.f; ++a, ++i) *i = *a;
                destroy(i, f);
            } else {
                T* i = s; const T* a = x.s; for (; a != x.s + size(); ++a, ++i) *i = *a;
                ucopy(a, x.f, f);
            }
            f = s + xlen;
        }
        return *this;
    }
    void reserve_raw(unsigned n) { if (n > cap()) { T* ns = alloc(n); T* nf = ucopy(s, f, ns); destroy(s, f); std::free(s); s = ns; f = nf; e = ns + n; } }
};
static int g_fail = 0;
static void check(bool ok, const char* what) { if (!ok) { std::printf("check failed: %s\n", what); g_fail = 1; } }
template <class T> static bool same(const Vec<T>& v, const int* want, unsigned n) {
    if (v.size() != n) return false;
    for (unsigned i = 0; i < n; ++i) if (v.s[i].v != want[i]) return false;
    return true;
}
static void fill(Vec<E>& v, int n, int base) { for (int i = 0; i < n; ++i) v.push_back(E(base + i)); }
int main() {
    {
        Vec<E> v; fill(v, 5, 0); v.reserve_raw(16);       // 0 1 2 3 4, spare capacity 11
        v.fill_insert(v.s + 1, 2, E(9));                   // elems_after 4 > n 2: shift the tail, fill
        int w1[] = { 0, 9, 9, 1, 2, 3, 4 }; check(same(v, w1, 7), "in place, elems_after > n");
        v.fill_insert(v.s + 5, 4, E(8));                   // elems_after 2 <= n 4: fill past the end first
        int w2[] = { 0, 9, 9, 1, 2, 8, 8, 8, 8, 3, 4 }; check(same(v, w2, 11), "in place, elems_after <= n");
        v.fill_insert(v.s, 0, E(5));                       // n == 0: nothing
        check(v.size() == 11, "n == 0");
        const E& self = v.s[0];                            // x aliasing the vector: the local copy keeps it valid
        v.fill_insert(v.s, 3, self);
        int w3[] = { 0, 0, 0, 0, 9, 9, 1, 2, 8, 8, 8, 8, 3, 4 }; check(same(v, w3, 14), "aliased x");
        v.fill_insert(v.s + 2, 5, E(6));                   // overflow: _M_insert_overflow(pos, x, n)
        check(v.size() == 19 && v.s[2].v == 6 && v.s[6].v == 6 && v.s[7].v == 0 && v.s[18].v == 4, "overflow path");
    }
    check(g_live == 0, "live elements balance");
    if (g_fail) { std::printf("00a85cc0_TEST FAIL\n"); return 1; }
    std::printf("00a85cc0_TEST PASS\n");
    return 0;
}