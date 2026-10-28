// behaviour test for 0x00889b20 vector<CEnvironmentThemePriorityStackEntry>::_M_insert_overflow: STLport _M_insert_overflow -- grow to old + max(old, n), copy the prefix,
// construct one copy (n == 1) or fill n copies, copy the suffix unless atend, release the old block.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_live = 0;
struct Elem {                                  // stand-in for CEnvironmentThemePriorityStackEntry (copy-constructed, assigned and destroyed)
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

int main()
{
    bool ok = true;
    { const int a[] = { 1, 2, 3 }; Vec v; v.reserve_with(a, 3, 3);
      v._M_insert_overflow(v._M_start + 1, Elem(9), 1);
      const int r[] = { 1, 9, 2, 3 }; if (!v.is(r, 4) || v.capacity() != 6) ok = false; v._M_clear(); }
    { const int a[] = { 4, 5 }; Vec v; v.reserve_with(a, 2, 2);
      v._M_insert_overflow(v._M_finish, Elem(7), 3, true);
      const int r[] = { 4, 5, 7, 7, 7 }; if (!v.is(r, 5) || v.capacity() != 5) ok = false; v._M_clear(); }
    { Vec v; v._M_insert_overflow(v._M_finish, Elem(6), 1, true);
      const int r[] = { 6 }; if (!v.is(r, 1) || v.capacity() != 1) ok = false; v._M_clear(); }
    if (g_live != 0) ok = false;
    if (!ok) { std::printf("00889b20_TEST FAIL\n"); return 1; }
    std::printf("00889b20_TEST PASS\n");
    return 0;
}