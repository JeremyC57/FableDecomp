// behaviour test for 0x0067d450 vector<CBlurredAttitudesDef>::operator=: STLport vector::operator= -- self-assignment is a no-op; a longer source
// reallocates and copies; a shorter one assigns and destroys the excess; otherwise assign the overlap and construct the rest.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_live = 0;
struct Elem {                                  // stand-in for CBlurredAttitudesDef (copy-constructed, assigned and destroyed)
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

static Vec& assign(Vec& self, const Vec& x)
{
    if (&x != &self) {
        const unsigned xlen = x.size();
        if (xlen > self.capacity()) {
            Elem* tmp = Vec::allocate(xlen);
            for (unsigned i = 0; i < xlen; ++i) Vec::construct(tmp + i, x._M_start[i]);
            self._M_clear();
            self._M_start = tmp;
            self._M_eos = self._M_start + xlen;
        } else if (self.size() >= xlen) {
            Elem* i = self._M_start;
            for (const Elem* s = x._M_start; s != x._M_finish; ++s, ++i) *i = *s;
            for (Elem* p = i; p != self._M_finish; ++p) p->~Elem();
        } else {
            for (unsigned k = 0; k < self.size(); ++k) self._M_start[k] = x._M_start[k];
            Elem* d = self._M_finish;
            for (const Elem* s = x._M_start + self.size(); s != x._M_finish; ++s, ++d) Vec::construct(d, *s);
        }
        self._M_finish = self._M_start + xlen;
    }
    return self;
}
int main()
{
    bool ok = true;
    const int a[] = { 1, 2, 3, 4 }; const int b[] = { 7, 8 };
    { Vec v, w; v.reserve_with(b, 2, 2); w.reserve_with(a, 4, 4);
      assign(v, w); if (!v.is(a, 4) || v.capacity() != 4) ok = false;
      assign(v, v); if (!v.is(a, 4)) ok = false; v._M_clear(); w._M_clear(); }
    { Vec v, w; v.reserve_with(a, 4, 4); w.reserve_with(b, 2, 2);
      assign(v, w); if (!v.is(b, 2) || v.capacity() != 4) ok = false; v._M_clear(); w._M_clear(); }
    { Vec v, w; v.reserve_with(b, 2, 6); w.reserve_with(a, 4, 4);
      assign(v, w); if (!v.is(a, 4) || v.capacity() != 6) ok = false; v._M_clear(); w._M_clear(); }
    if (g_live != 0) ok = false;
    if (!ok) { std::printf("0067d450_TEST FAIL\n"); return 1; }
    std::printf("0067d450_TEST PASS\n");
    return 0;
}