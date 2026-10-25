// Behaviour test for 0x00889F00 std::vector<NEnvironmentInternal::CBlendedEnvironmentTheme>::operator=(const vector&)
// The model mirrors the source: STLport operator= over an element with a counted copy constructor, destructor and
// assignment (stand-ins for 0x008894A0 / the inlined destructor / 0x006B5DF0).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
static int g_assigns = 0;

struct Theme {
    float TimeOfDay;
    long  Tag;
    Theme(float t, long g) : TimeOfDay(t), Tag(g) { ++g_live; }
    Theme(const Theme& o) : TimeOfDay(o.TimeOfDay), Tag(o.Tag) { ++g_live; }
    ~Theme() { --g_live; }
    Theme& operator=(const Theme& o) { TimeOfDay = o.TimeOfDay; Tag = o.Tag; ++g_assigns; return *this; }
};

namespace tv {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    for (; __first != __last; ++__first, ++__result) new (__result) Theme(*__first);
    return __result;
}
template <class _ForwardIter>
void __destroy_aux(_ForwardIter __first, _ForwardIter __last, const __false_type&)
{
    for (; __first != __last; ++__first) __first->~Theme();
}
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = (_Distance)(__last - __first); __n > 0; --__n) { *__result = *__first; ++__first; ++__result; }
    return __result;
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

class vec {
public:
    typedef Theme* pointer;
    typedef const Theme* const_pointer;
    typedef unsigned int size_type;
    Theme* _M_start;
    Theme* _M_finish;
    Theme* _M_end_of_storage;
    vec(size_type n, size_type cap, long seed)
    {
        _M_start = cap ? (Theme*)std::malloc(cap * sizeof(Theme)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + cap;
        for (size_type i = 0; i < n; ++i) { new (_M_finish) Theme((float)i * 0.5f, seed + (long)i); ++_M_finish; }
    }
    ~vec() { _M_clear(); }
    size_type size() const { return size_type(_M_finish - _M_start); }
    size_type capacity() const { return size_type(_M_end_of_storage - _M_start); }
    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = __n != 0 ? (pointer)std::malloc(__n * sizeof(Theme)) : 0;
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start) std::free(_M_start);
    }
    vec& operator=(const vec& __x)
    {
        if (&__x != this) {
            const size_type __xlen = __x.size();
            if (__xlen > capacity()) {
                pointer __tmp = _M_allocate_and_copy(__xlen, (const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0);
                _M_clear();
                _M_start = __tmp;
                _M_end_of_storage = _M_start + __xlen;
            }
            else if (size() >= __xlen) {
                pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, _M_start, __false_type());
                _Destroy(__i, _M_finish);
            }
            else {
                __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), _M_start, __false_type());
                __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, _M_finish, __false_type());
            }
            _M_finish = _M_start + __xlen;
        }
        return *this;
    }
};
}

static bool same(const tv::vec& a, const tv::vec& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned int i = 0; i < a.size(); ++i)
        if (a._M_start[i].Tag != b._M_start[i].Tag || a._M_start[i].TimeOfDay != b._M_start[i].TimeOfDay) return false;
    return true;
}

int main()
{
    int fail = 0;
    {
        tv::vec src(5, 5, 100), dst(2, 2, 7);           // grow: reallocate and copy-construct
        tv::vec& r = (dst = src);
        if (&r != &dst || !same(dst, src) || dst.capacity() != 5 || g_live != 10) fail |= 1;
    }
    if (g_live != 0) fail |= 2;
    {
        tv::vec src(2, 2, 200), dst(4, 6, 9);           // shrink: assign 2, destroy the surplus 2
        g_assigns = 0;
        dst = src;
        if (!same(dst, src) || dst.capacity() != 6 || g_assigns != 2 || g_live != 4) fail |= 4;
    }
    if (g_live != 0) fail |= 8;
    {
        tv::vec src(5, 5, 300), dst(2, 8, 11);          // fits: assign 2, copy-construct 3
        g_assigns = 0;
        dst = src;
        if (!same(dst, src) || dst.capacity() != 8 || g_assigns != 2 || g_live != 10) fail |= 16;
        dst = dst;                                      // self-assignment is a no-op
        if (!same(dst, src) || g_live != 10) fail |= 32;
    }
    if (g_live != 0) fail |= 64;
    if (fail) { std::printf("00889f00_TEST FAIL\n"); return 1; }
    std::printf("00889f00_TEST PASS\n");
    return 0;
}