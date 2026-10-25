// Behaviour test for 0x00469012 vector<CRGBColour>::_M_fill_insert: both in-place paths (tail longer / shorter than n)
// and the overflow hand-off, checked against a reference insert.
#include <cstdio>
#include <cstdlib>
#include <new>

struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() : B(0), G(0), R(0), A(0) {}
    CRGBColour(const CRGBColour& other) { R = other.R; G = other.G; B = other.B; A = other.A; }
};

static int g_overflow_calls = 0;

static void _Construct(CRGBColour* p, const CRGBColour& value) { new (p) CRGBColour(value); }
static CRGBColour* uninitialized_fill_n(CRGBColour* first, unsigned int n, const CRGBColour& x)
{
    for (; n > 0; --n, ++first) _Construct(first, x);
    return first;
}
struct __false_type { };

template <class _Tp> _Tp* __uninitialized_copy(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    _Tp* __cur = __result;
    for (; __first != __last; ++__first, ++__cur) _Construct(__cur, *__first);
    return __cur;
}
template <class _Tp> _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    for (int __n = int(__last - __first); __n > 0; --__n) *--__result = *--__last;
    return __result;
}
template <class _Tp> void fill(_Tp* __first, _Tp* __last, const _Tp& __val) { for (; __first != __last; ++__first) *__first = __val; }

template <class _Tp> struct vector {
    _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage;
    void _M_insert_overflow(_Tp* pos, const _Tp& x, const __false_type&, unsigned int n, bool atend = false)
    {
        ++g_overflow_calls;
        unsigned int old = _M_finish - _M_start, len = old + (old > n ? old : n);
        _Tp* ns = (_Tp*)std::malloc(len * sizeof(_Tp));
        _Tp* nf = __uninitialized_copy(_M_start, pos, ns, __false_type());
        nf = uninitialized_fill_n(nf, n, x);
        if (!atend) nf = __uninitialized_copy(pos, _M_finish, nf, __false_type());
        std::free(_M_start); _M_start = ns; _M_finish = nf; _M_end_of_storage = ns + len;
    }
    void _M_fill_insert(_Tp* __position, unsigned int __n, const _Tp& __x)
    {
        if (__n != 0) {
            if ((unsigned int)(_M_end_of_storage - _M_finish) >= __n) {
                _Tp __x_copy = __x;
                const unsigned int __elems_after = _M_finish - __position;
                _Tp* __old_finish = _M_finish;
                if (__elems_after > __n) {
                    __uninitialized_copy(_M_finish - __n, _M_finish, _M_finish, __false_type());
                    _M_finish += __n;
                    __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                    fill(__position, __position + __n, __x_copy);
                } else {
                    uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy);
                    _M_finish += __n - __elems_after;
                    __uninitialized_copy(__position, __old_finish, _M_finish, __false_type());
                    _M_finish += __elems_after;
                    fill(__position, __old_finish, __x_copy);
                }
            } else
                _M_insert_overflow(__position, __x, __false_type(), __n);
        }
    }
};

static CRGBColour C(int v) { CRGBColour c; c.R = (unsigned char)v; c.G = (unsigned char)(v + 1); c.B = (unsigned char)(v + 2); c.A = 0xff; return c; }

static int Run(int size, int cap, int pos, int n)
{
    vector<CRGBColour> v;
    v._M_start = (CRGBColour*)std::malloc(cap * sizeof(CRGBColour));
    for (int i = 0; i < size; ++i) new (v._M_start + i) CRGBColour(C(i * 10));
    v._M_finish = v._M_start + size; v._M_end_of_storage = v._M_start + cap;
    CRGBColour x = C(200);
    v._M_fill_insert(v._M_start + pos, n, x);
    int ok = (v._M_finish - v._M_start) == size + n;
    for (int i = 0; ok && i < size + n; ++i) {
        int expect = i < pos ? i * 10 : (i < pos + n ? 200 : (i - n) * 10);
        CRGBColour e = C(expect);
        if (v._M_start[i].R != e.R || v._M_start[i].G != e.G || v._M_start[i].B != e.B || v._M_start[i].A != e.A) ok = 0;
    }
    std::free(v._M_start);
    return ok;
}

int main()
{
    int ok = 1;
    ok &= Run(5, 10, 1, 2);    // elems_after 4 > n 2
    ok &= Run(5, 10, 3, 4);    // elems_after 2 <= n 4
    ok &= Run(5, 10, 5, 3);    // append
    ok &= Run(5, 10, 2, 0);    // n == 0: no change
    int before = g_overflow_calls;
    ok &= Run(5, 6, 1, 3);     // no room: overflow
    if (g_overflow_calls != before + 1) ok = 0;
    if (!ok) { std::printf("00469012_TEST FAIL\n"); return 1; }
    std::printf("00469012_TEST PASS\n");
    return 0;
}