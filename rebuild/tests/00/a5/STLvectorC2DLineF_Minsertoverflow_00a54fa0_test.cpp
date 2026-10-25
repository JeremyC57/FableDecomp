// Behaviour test for 0x00A54FA0: std::vector<C2DLineF>::_M_insert_overflow (grow and insert 16-byte lines)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp>
class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
}

class C2DVector {
public:
    float X, Y;
};
class C2DLineF {
public:
    C2DVector Point1;
    C2DVector Point2;
};
static C2DLineF Line(float a)
{
    C2DLineF l;
    l.Point1.X = a; l.Point1.Y = a + 1.0f; l.Point2.X = a + 2.0f; l.Point2.Y = a + 3.0f;
    return l;
}
static bool IsLine(const C2DLineF& l, float a)
{
    return Near(l.Point1.X, a) && Near(l.Point1.Y, a + 1.0f) && Near(l.Point2.X, a + 2.0f) && Near(l.Point2.Y, a + 3.0f);
}

namespace std {
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    for ( ; __first != __last; ++__first, ++__result)
        _Construct(&*__result, *__first);
    return __result;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp>
inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp>
inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector() { this->_M_start = this->_M_finish = this->_M_end_of_storage._M_data = 0; }
    ~vector() { _M_clear(); }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, (unsigned int)(this->_M_end_of_storage._M_data - this->_M_start));
    }
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                             unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    _Tp* __new_start = this->_M_end_of_storage.allocate(__len);
    _Tp* __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
}

int main()
{
    std::vector<C2DLineF> v;
    C2DLineF a = Line(1.0f), b = Line(10.0f), c = Line(100.0f);
    v._M_insert_overflow(v._M_finish, a, std::__false_type(), 1u, true);
    Check(v.size() == 1 && IsLine(v._M_start[0], 1.0f));
    v._M_insert_overflow(v._M_finish, b, std::__false_type(), 1u, true);
    Check(v.size() == 2 && v._M_end_of_storage._M_data == v._M_start + 2);
    v._M_insert_overflow(v._M_start + 1, c, std::__false_type(), 2u, false);
    Check(v.size() == 4 && v._M_end_of_storage._M_data == v._M_start + 4);
    Check(IsLine(v._M_start[0], 1.0f) && IsLine(v._M_start[1], 100.0f) && IsLine(v._M_start[2], 100.0f) && IsLine(v._M_start[3], 10.0f));
    if (g_fail) {
        printf("00a54fa0_TEST FAIL\n");
        return 1;
    }
    printf("00a54fa0_TEST PASS\n");
    return 0;
}