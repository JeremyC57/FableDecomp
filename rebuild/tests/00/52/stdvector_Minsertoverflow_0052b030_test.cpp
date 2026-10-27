// Behaviour test for 0x0052B030 std::vector<CUnprovenElement_005269b0>::_M_insert_overflow: the source body verbatim (comments and CRT declarations dropped), mocks for its callees.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#define std tstd
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif

class CUnprovenElement_005269b0 {                          // 16 bytes (descriptive: element type not proven)
public:
    unsigned long Word0;
    unsigned long Word1;
    unsigned long Word2;
    unsigned long Word3;
};
struct CUnprovenElement16_005269f0 {                       // the same element under 0x005269F0's bound spelling
    unsigned long Word0;
    unsigned long Word1;
    unsigned long Word2;
    unsigned long Word3;
};

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                          // 0x005269B0 (same-TU body)
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{                                                          // 0x005269F0 (same-TU body)
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear()
    {
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
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = (pointer)__uninitialized_fill_n((CUnprovenElement16_005269f0*)__new_finish, __fill_len,
                                                       (const CUnprovenElement16_005269f0&)__x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
typedef CUnprovenElement_005269b0 _Elem;
template void vector<_Elem>::_M_insert_overflow(_Elem*, const _Elem&, const __false_type&, unsigned int, bool);
} // namespace std
typedef std::vector<CUnprovenElement_005269b0> TVec;
struct TestVec : TVec {
    void Call(CUnprovenElement_005269b0* pos, const CUnprovenElement_005269b0& x, unsigned int n, bool atend)
    {
        _M_insert_overflow(pos, x, std::__false_type(), n, atend);
    }
};
static CUnprovenElement_005269b0 E(unsigned long v) { CUnprovenElement_005269b0 e; e.Word0 = v; e.Word1 = v + 1; e.Word2 = v + 2; e.Word3 = v + 3; return e; }
int main()
{
    TestVec v;
    v._M_start = (CUnprovenElement_005269b0*)malloc(3 * sizeof(CUnprovenElement_005269b0));
    v._M_start[0] = E(10); v._M_start[1] = E(20); v._M_start[2] = E(30);
    v._M_finish = v._M_start + 3; v._M_end_of_storage._M_data = v._M_finish;
    int ok = 1;
    v.Call(v._M_start + 1, E(99), 1, false);                 // insert one in the middle: capacity 3 + max(3, 1)
    ok &= v.size() == 4 && v._M_start[0].Word0 == 10 && v._M_start[1].Word3 == 102 && v._M_start[2].Word0 == 20 && v._M_start[3].Word2 == 32;
    ok &= v._M_end_of_storage._M_data - v._M_start == 6;
    v.Call(v._M_finish, E(50), 5, true);                     // fill five at the end: 4 + max(4, 5)
    ok &= v.size() == 9 && v._M_start[3].Word0 == 30 && v._M_start[4].Word0 == 50 && v._M_start[8].Word1 == 51;
    ok &= v._M_end_of_storage._M_data - v._M_start == 9;
    free(v._M_start);
    if (ok) printf("0052b030_TEST PASS\n");
    return ok ? 0 : 1;
}