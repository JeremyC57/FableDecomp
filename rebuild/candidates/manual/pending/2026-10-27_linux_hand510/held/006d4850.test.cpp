// behaviour test for 0x006d4850 vector<pair<CCharString,long> >::_M_fill_insert: STLport _M_fill_insert -- n == 0 is a no-op; with room, shift the tail by n
// (elems_after > n: copy the last n up, move the rest backward, fill; else fill past the end, copy the tail, fill the
// gap); without room, _M_insert_overflow.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_live = 0;
struct Elem {                                  // stand-in for pair<CCharString,long> (copy-constructed, assigned and destroyed)
    int v;
    Elem() : v(0) { ++g_live; }
    Elem(int x) : v(x) { ++g_live; }
    Elem(const Elem& o) : v(o.v) { ++g_live; }
    Elem& operator=(const Elem& o) { v = o.v; return *this; }
    ~Elem() { --g_live; }
};
struct Vec {
    Elem* _M_start; Elem* _M_finish; Elem* _M_eos;
    Vec() : _M_start(0), _M_finish(0), _M_eos(0) { }
    unsigned size() const { return (unsigned)(_M_finish - _M_start); }
    unsigned capacity() const { return (unsigned)(_M_eos - _M_start); }
    static Elem* allocate(unsigned n) { return n ? (Elem*)std::malloc(n * sizeof(Elem)) : 0; }
    static void construct(Elem* p, const Elem& x) { new (p) Elem(x); }
    void _M_clear() { for (Elem* p = _M_start; p != _M_finish; ++p) p->~Elem(); std::free(_M_start); }
    void reserve_with(const int* vals, unsigned n, unsigned cap)
    {
        _M_start = allocate(cap); _M_finish = _M_start; _M_eos = _M_start + cap;
        for (unsigned i = 0; i < n; ++i) construct(_M_finish++, Elem(vals[i]));
    }
    void _M_insert_overflow(Elem* pos, const Elem& x, unsigned fill_len, bool atend = false)
    {
        const unsigned old_size = size();
        const unsigned len = old_size + (old_size > fill_len ? old_size : fill_len);
        Elem* new_start = allocate(len);
        Elem* new_finish = new_start;
        for (Elem* p = _M_start; p != pos; ++p) construct(new_finish++, *p);
        if (fill_len == 1) { construct(new_finish, x); ++new_finish; }
        else for (unsigned i = 0; i < fill_len; ++i) construct(new_finish++, x);
        if (!atend) for (Elem* p = pos; p != _M_finish; ++p) construct(new_finish++, *p);
        _M_clear();
        _M_start = new_start; _M_finish = new_finish; _M_eos = new_start + len;
    }
    bool is(const int* vals, unsigned n) const
    {
        if (size() != n) return false;
        for (unsigned i = 0; i < n; ++i) if (_M_start[i].v != vals[i]) return false;
        return true;
    }
};

static void fill_insert(Vec& v, Elem* pos, unsigned n, const Elem& x)
{
    if (n != 0) {
        if (unsigned(v._M_eos - v._M_finish) >= n) {
            Elem x_copy = x;
            const unsigned elems_after = v._M_finish - pos;
            Elem* old_finish = v._M_finish;
            if (elems_after > n) {
                for (Elem *s = v._M_finish - n, *d = v._M_finish; s != old_finish; ++s, ++d) Vec::construct(d, *s);
                v._M_finish += n;
                for (Elem *s = old_finish - n, *d = old_finish; s != pos; ) *--d = *--s;
                for (Elem* p = pos; p != pos + n; ++p) *p = x_copy;
            } else {
                for (unsigned i = 0; i < n - elems_after; ++i) Vec::construct(v._M_finish + i, x_copy);
                v._M_finish += n - elems_after;
                for (Elem *s = pos, *d = v._M_finish; s != old_finish; ++s, ++d) Vec::construct(d, *s);
                v._M_finish += elems_after;
                for (Elem* p = pos; p != old_finish; ++p) *p = x_copy;
            }
        } else
            v._M_insert_overflow(pos, x, n);
    }
}
int main()
{
    bool ok = true;
    { const int a[] = { 1, 2, 3, 4 }; Vec v; v.reserve_with(a, 4, 8);
      fill_insert(v, v._M_start + 1, 0, Elem(9)); if (!v.is(a, 4)) ok = false;
      fill_insert(v, v._M_start + 1, 2, Elem(9));
      const int r[] = { 1, 9, 9, 2, 3, 4 }; if (!v.is(r, 6)) ok = false; v._M_clear(); }
    { const int a[] = { 1, 2 }; Vec v; v.reserve_with(a, 2, 8);
      fill_insert(v, v._M_start + 1, 3, Elem(5));
      const int r[] = { 1, 5, 5, 5, 2 }; if (!v.is(r, 5)) ok = false; v._M_clear(); }
    { const int a[] = { 1, 2 }; Vec v; v.reserve_with(a, 2, 2);
      fill_insert(v, v._M_start, 2, Elem(8));
      const int r[] = { 8, 8, 1, 2 }; if (!v.is(r, 4) || v.capacity() != 4) ok = false; v._M_clear(); }
    if (g_live != 0) ok = false;
    if (!ok) { std::printf("006d4850_TEST FAIL\n"); return 1; }
    std::printf("006d4850_TEST PASS\n");
    return 0;
}
