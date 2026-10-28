// Behaviour test for 00624a30: STLport vector::operator= on CTestQuest.
#include <cstdio>
#include <new>
#include <cstdlib>

// Model of the STLport 4.6 vector the source instantiates; element CTestQuest modelled as a 0x1c-byte record
// with a value and a live-object counter (a copy constructor, an assignment and a destructor, as the element has).
static int g_live = 0;
struct E {
    int v;
    unsigned char pad[0x1c - sizeof(int)];
    E() : v(0) { ++g_live; }
    E(int x) : v(x) { ++g_live; }
    E(const E& o) : v(o.v) { ++g_live; }
    E& operator=(const E& o) { v = o.v; return *this; }
    ~E() { --g_live; }
};
struct V {
    E* _M_start; E* _M_finish; E* _M_end_of_storage;
    V() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    unsigned size() const { return (unsigned)(_M_finish - _M_start); }
    unsigned capacity() const { return (unsigned)(_M_end_of_storage - _M_start); }
    static E* allocate(unsigned n) { return n ? (E*)std::malloc(n * sizeof(E)) : 0; }
    static E* ucopy(const E* f, const E* l, E* r) { for (; f != l; ++f, ++r) new (r) E(*f); return r; }
    static E* ufill(E* f, unsigned n, const E& x) { for (; n > 0; --n, ++f) new (f) E(x); return f; }
    static void destroy(E* f, E* l) { for (; f != l; ++f) f->~E(); }
    void _M_clear() { destroy(_M_start, _M_finish); std::free(_M_start); }
    void reserve_with(int n, int cap) { _M_start = allocate(cap); _M_finish = _M_start; for (int i = 0; i < n; ++i) new (_M_finish++) E(i + 1); _M_end_of_storage = _M_start + cap; }
    // STLport _M_insert_overflow (the growth path)
    void _M_insert_overflow(E* pos, const E& x, unsigned fill_len, bool atend)
    {
        const unsigned old_size = size();
        const unsigned len = old_size + (old_size > fill_len ? old_size : fill_len);
        E* ns = allocate(len);
        E* nf = ucopy(_M_start, pos, ns);
        if (fill_len == 1) { new (nf) E(x); ++nf; }
        else nf = ufill(nf, fill_len, x);
        if (!atend) nf = ucopy(pos, _M_finish, nf);
        _M_clear();
        _M_start = ns; _M_finish = nf; _M_end_of_storage = ns + len;
    }
    // STLport _M_fill_insert
    void _M_fill_insert(E* pos, unsigned n, const E& x)
    {
        if (n != 0) {
            if ((unsigned)(_M_end_of_storage - _M_finish) >= n) {
                E x_copy = x;
                const unsigned elems_after = (unsigned)(_M_finish - pos);
                E* old_finish = _M_finish;
                if (elems_after > n) {
                    ucopy(_M_finish - n, _M_finish, _M_finish);
                    _M_finish += n;
                    { E* l = old_finish - n; E* r = old_finish; for (int k = (int)(l - pos); k > 0; --k) *--r = *--l; }
                    for (E* p = pos; p != pos + n; ++p) *p = x_copy;
                } else {
                    ufill(_M_finish, n - elems_after, x_copy);
                    _M_finish += n - elems_after;
                    ucopy(pos, old_finish, _M_finish);
                    _M_finish += elems_after;
                    for (E* p = pos; p != old_finish; ++p) *p = x_copy;
                }
            } else
                _M_insert_overflow(pos, x, n, false);
        }
    }
    // STLport operator=
    V& operator=(const V& x)
    {
        if (&x != this) {
            const unsigned xlen = x.size();
            if (xlen > capacity()) {
                E* tmp = allocate(xlen);
                ucopy(x._M_start, x._M_finish, tmp);
                _M_clear();
                _M_start = tmp;
                _M_end_of_storage = _M_start + xlen;
            } else if (size() >= xlen) {
                E* i = _M_start;
                for (const E* f = x._M_start; f != x._M_finish; ++f, ++i) *i = *f;
                destroy(i, _M_finish);
            } else {
                E* d = _M_start;
                const E* f = x._M_start;
                for (; f != x._M_start + size(); ++f, ++d) *d = *f;
                ucopy(f, x._M_finish, _M_finish);
            }
            _M_finish = _M_start + xlen;
        }
        return *this;
    }
};
static bool same(const V& v, const int* want, unsigned n)
{
    if (v.size() != n) return false;
    for (unsigned i = 0; i < n; ++i) if (v._M_start[i].v != want[i]) return false;
    return true;
}

int main()
{
    bool ok = true;
    {   // source outgrows the capacity: reallocate exactly xlen
        V a; a.reserve_with(1, 1);
        V b; b.reserve_with(4, 4);
        a = b;
        int want[] = { 1, 2, 3, 4 };
        ok = ok && same(a, want, 4) && a.capacity() == 4;
        a._M_clear(); b._M_clear();
    }
    {   // shrinking: assign over the live ones, destroy the surplus, keep the capacity
        V a; a.reserve_with(5, 8);
        V b; b.reserve_with(2, 2); b._M_start[0].v = 7; b._M_start[1].v = 8;
        a = b;
        int want[] = { 7, 8 };
        ok = ok && same(a, want, 2) && a.capacity() == 8;
        a._M_clear(); b._M_clear();
    }
    {   // growing within capacity: assign the live ones, copy-construct the rest
        V a; a.reserve_with(2, 8); a._M_start[0].v = 0; a._M_start[1].v = 0;
        V b; b.reserve_with(5, 5);
        a = b;
        int want[] = { 1, 2, 3, 4, 5 };
        ok = ok && same(a, want, 5) && a.capacity() == 8;
        a._M_clear(); b._M_clear();
    }
    {   // self-assignment is a no-op
        V a; a.reserve_with(3, 3);
        a = a;
        int want[] = { 1, 2, 3 };
        ok = ok && same(a, want, 3);
        a._M_clear();
    }
    ok = ok && g_live == 0;
    if (!ok) { std::printf("00624a30_TEST FAIL\n"); return 1; }
    std::printf("00624a30_TEST PASS\n");
    return 0;
}