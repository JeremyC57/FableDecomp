// Behaviour test for 0x0066E370: std::vector::_M_insert_overflow(C3DVector* position, const C3DVector& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport non-POD vector growth for 3D points: allocate old + max(old, n), copy the head, construct the n new copies, copy the tail, free the old block
// A copy of the source model and logic (compiled separately from src/0066e370.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);   // 0x0066DDF0 (landed)
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy_head(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> class allocator {
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

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);
    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy_head(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<C3DVector>::_M_insert_overflow(C3DVector*, const C3DVector&, const __false_type&, unsigned int, bool);
}

// ---- test stand-ins ----
static int g_copyCalls = 0;
namespace std {
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                         // STLport body of the landed 0x0066DDF0
    ++g_copyCalls;
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
}

struct TestVector : public std::vector<C3DVector> {
    C3DVector* Begin() { return this->_M_start; }
    unsigned int Size() { return (unsigned int)(this->_M_finish - this->_M_start); }
    unsigned int Capacity() { return (unsigned int)(this->_M_end_of_storage._M_data - this->_M_start); }
    void Init(C3DVector* s, unsigned int n, unsigned int cap) { this->_M_set(s, s + n, s + cap); }
};

static int Fail() { std::printf("0066e370_TEST FAIL\n"); return 1; }
static bool Is(const C3DVector& v, float k) { return v.X == k && v.Y == 2.0f * k && v.Z == -k; }
static C3DVector Point(float k) { return C3DVector(k, 2.0f * k, -k); }

int main()
{
    C3DVector* s = (C3DVector*)std::malloc(3 * sizeof(C3DVector));
    for (int k = 0; k < 3; ++k) new (&s[k]) C3DVector(Point((float)k));
    TestVector v; v.Init(s, 3, 3);
    C3DVector x = Point(9.0f);
    v._M_insert_overflow(v.Begin() + 1, x, std::__false_type(), 1);         // 3 -> 6, x between 0 and 1
    if (v.Size() != 4 || v.Capacity() != 6 || g_copyCalls != 1) return Fail();                   // head inline, tail out of line
    if (!Is(v.Begin()[0], 0.0f) || !Is(v.Begin()[1], 9.0f) || !Is(v.Begin()[2], 1.0f) || !Is(v.Begin()[3], 2.0f)) return Fail();
    int calls = g_copyCalls;
    x = Point(7.0f);
    v._M_insert_overflow(v.Begin() + 4, x, std::__false_type(), 5, true);   // at the end: 4 -> 9, no tail copy
    if (v.Size() != 9 || v.Capacity() != 9 || g_copyCalls != calls) return Fail();                  // atend: no tail copy call
    for (int i = 4; i < 9; ++i) if (!Is(v.Begin()[i], 7.0f)) return Fail();
    if (!Is(v.Begin()[3], 2.0f)) return Fail();
    v._M_insert_overflow(v.Begin(), x, std::__false_type(), 2);             // at the front: 9 -> 18
    if (v.Size() != 11 || v.Capacity() != 18 || !Is(v.Begin()[0], 7.0f) || !Is(v.Begin()[1], 7.0f) || !Is(v.Begin()[2], 0.0f) ||
        !Is(v.Begin()[3], 9.0f) || !Is(v.Begin()[10], 7.0f)) return Fail();
    TestVector e; e.Init(0, 0, 0);                                           // an empty vector grows to exactly n
    e._M_insert_overflow(0, x, std::__false_type(), 3);
    if (e.Size() != 3 || e.Capacity() != 3 || !Is(e.Begin()[2], 7.0f)) return Fail();
    v._M_clear(); e._M_clear();
    std::printf("0066e370_TEST PASS\n");
    return 0;
}