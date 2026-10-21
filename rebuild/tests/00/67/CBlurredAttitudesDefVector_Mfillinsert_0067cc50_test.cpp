// Behaviour test for 0x0067CC50 (vector<CBlurredAttitudesDef>::_M_fill_insert): insert n copies at a position.
#include <cstdio>
#include <cstdlib>
#include <new>
// Host model: the element is an int with a live-object counter; the helpers are the STLport semantics (copy into raw
// storage, assign backwards, assign-fill, construct-fill); _M_insert_overflow regrows into a fresh block.
static int g_live = 0;
struct E { int v; E(int x) : v(x) { ++g_live; } E(const E& o) : v(o.v) { ++g_live; } ~E() { --g_live; } E& operator=(const E& o) { v = o.v; return *this; } };
static E* ucopy(E* f, E* l, E* r) { for (; f != l; ++f, ++r) new (r) E(*f); return r; }
static E* copy_back(E* f, E* l, E* r) { while (l != f) *--r = *--l; return r; }
static void fill(E* f, E* l, const E& x) { for (; f != l; ++f) *f = x; }
static E* ufill_n(E* f, unsigned long n, const E& x) { for (; n; --n, ++f) new (f) E(x); return f; }
struct Vec {
    E* _M_start; E* _M_finish; E* _M_end_of_storage; int overflows;
    Vec(int cap) : overflows(0) { _M_start = _M_finish = (E*)std::malloc(cap * sizeof(E)); _M_end_of_storage = _M_start + cap; }
    ~Vec() { for (E* p = _M_start; p != _M_finish; ++p) p->~E(); std::free(_M_start); }
    void push(int v) { new (_M_finish++) E(v); }
    void _M_insert_overflow(E* pos, const E& x, unsigned long n)
    {
        ++overflows;
        unsigned long sz = _M_finish - _M_start, len = sz + (sz > n ? sz : n);
        E* s = (E*)std::malloc(len * sizeof(E));
        E* f = ucopy(_M_start, pos, s); f = ufill_n(f, n, x); f = ucopy(pos, _M_finish, f);
        for (E* p = _M_start; p != _M_finish; ++p) p->~E();
        std::free(_M_start); _M_start = s; _M_finish = f; _M_end_of_storage = s + len;
    }
    void _M_fill_insert(E* position, unsigned long n, const E& x)
    {
        if (n != 0) {
            if ((unsigned long)(_M_end_of_storage - _M_finish) >= n) {
                E x_copy = x;
                const unsigned long elems_after = _M_finish - position;
                E* old_finish = _M_finish;
                if (elems_after > n) {
                    ucopy(_M_finish - n, _M_finish, _M_finish);
                    _M_finish += n;
                    copy_back(position, old_finish - n, old_finish);
                    fill(position, position + n, x_copy);
                } else {
                    ufill_n(_M_finish, n - elems_after, x_copy);
                    _M_finish += n - elems_after;
                    ucopy(position, old_finish, _M_finish);
                    _M_finish += elems_after;
                    fill(position, old_finish, x_copy);
                }
            } else
                _M_insert_overflow(position, x, n);
        }
    }
    bool is(const int* want, int n) const
    {
        if (_M_finish - _M_start != n) return false;
        for (int i = 0; i < n; ++i) if (_M_start[i].v != want[i]) return false;
        return true;
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    { Vec v(10); for (int i = 1; i <= 5; ++i) v.push(i); E x(9); v._M_fill_insert(v._M_start + 1, 2, x);   // tail longer than n
      int w[] = { 1, 9, 9, 2, 3, 4, 5 }; CHECK(v.is(w, 7) && v.overflows == 0); }
    { Vec v(10); for (int i = 1; i <= 5; ++i) v.push(i); E x(7); v._M_fill_insert(v._M_start + 4, 3, x);   // tail shorter than n
      int w[] = { 1, 2, 3, 4, 7, 7, 7, 5 }; CHECK(v.is(w, 8) && v.overflows == 0); }
    { Vec v(5); for (int i = 1; i <= 4; ++i) v.push(i); E x(6); v._M_fill_insert(v._M_start + 2, 3, x);    // regrow
      int w[] = { 1, 2, 6, 6, 6, 3, 4 }; CHECK(v.is(w, 7) && v.overflows == 1); }
    { Vec v(5); v.push(1); E x(3); v._M_fill_insert(v._M_finish, 0, x); int w[] = { 1 }; CHECK(v.is(w, 1) && v.overflows == 0); }
    CHECK(g_live == 0);
    if (g_fail) { std::printf("0067cc50_TEST FAIL\n"); return 1; }
    std::printf("0067cc50_TEST PASS\n");
    return 0;
}