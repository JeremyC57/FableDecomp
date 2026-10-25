#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// Model of _STL::vector<long>::_M_initialize_aux<int>(n, val, __true_type) @ 0x00451C3E: allocate n longs and fill
// them with val (the integral fill-constructor path).

namespace _STL {
struct __true_type { };

template <class _Tp> class allocator {
public:
    allocator() { }
    __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _OutputIter, class _Size, class _Tp>
__forceinline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return fill_n(__first, __n, __x);                       // __uninitialized_fill_n(..., __true_type): POD
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    template <class _Integer>
    void _M_initialize_aux(_Integer __n, _Integer __val, const __true_type&)
    {
        this->_M_start = this->_M_end_of_storage.allocate(__n);
        this->_M_end_of_storage._M_data = this->_M_start + __n;
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, __val);
    }
};

}
int main()
{
    bool ok = true;
    _STL::vector<long> v;
    v._M_initialize_aux(5, 7, _STL::__true_type());
    if (v._M_finish - v._M_start != 5 || v._M_end_of_storage._M_data != v._M_start + 5)
        ok = false;
    for (int i = 0; i < 5; ++i)
        if (v._M_start[i] != 7)
            ok = false;
    _STL::vector<long> w;
    w._M_initialize_aux(0, 3, _STL::__true_type());
    if (w._M_start != 0 || w._M_finish != 0 || w._M_end_of_storage._M_data != 0)
        ok = false;
    if (!ok) { printf("00451c3e_TEST FAIL\n"); return 1; }
    printf("00451c3e_TEST PASS\n");
    return 0;
}