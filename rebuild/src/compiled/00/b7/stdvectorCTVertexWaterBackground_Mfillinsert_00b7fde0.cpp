// std::vector<CTVertexWaterBackground>::_M_fill_insert(CTVertexWaterBackground* __position, unsigned int __n, const CTVertexWaterBackground& __x) -- STLport insert of n copies of a 56-byte water background vertex: inside the spare capacity slide the tail (uninitialised copy, copy_backward, fill) or fill past the end first, otherwise grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCTVertexWaterBackground@@V?$allocator@VCTVertexWaterBackground@@@std@@@std@@QAEXPAVCTVertexWaterBackground@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B7FDE0, 469 bytes, thiscall `ret 0Ch`; speed TU.
// Identity (proven): the sole caller is vector<CTVertexWaterBackground>::resize(n, x) 0x00B7FD00 (`insert(end(), n - size(),
//   x)` at 0x00B7FD5D; Ego_r.pdb lists ?resize@?$vector@VCTVertexWaterBackground@@...@QAEXI@Z / insert / _Insert_n for the type),
//   and the growth path calls 0x00B7FFC0, the landed vector<CTVertexWaterBackground>::_M_insert_overflow(pos, x, const
//   __false_type&, n, false) (`push 0 ; push n ; lea tag ; push x ; push pos`); the 56-byte stride (`imul 92492493h` = /56,
//   14-dword `rep movsd`) is Ego_r CTVertexWaterBackground. The body is STLport 4.6 _vector.c `_M_fill_insert` verbatim.
//   The catalog label "Vector_Insert_14Elements" and the landed resize's binding `?Insert@CRetailVector_elem56@@...` are
//   stand-ins (NAME_FIXES.tsv). Mangled spelling as the landed _M_fill_insert rows (public QAE, `IABV3@`).
// Callees: _M_insert_overflow 0x00B7FFC0 (protected, out of line; landed). Everything else is inline.
// Shape: the landed _M_insert_overflow TU model (same retail TU: CTVertexWaterBackground, _Vector_base/_STLP_alloc_proxy,
//   __forceinline STLport helpers -- with plain `inline` VC7.1 leaves __uninitialized_copy / __uninitialized_fill_n /
//   __copy_backward / fill out of line, 317 bytes, probe log); uninitialized_fill_n is inlined here (the 0x0046C022 size-TU
//   copy calls it out of line); the `__false_type()` tag of the overflow call is zero-stored in a dead argument slot.
#include "fable_compiler.h"
inline void* operator new(unsigned int, void* p) { return p; }

class CTVertexWaterBackground {                    // Ego_r.pdb, 56 bytes (CTVertexWaterBackgroundBase layout)
public:
    short X;                                       // +0x00
    short Y;                                       // +0x02
    float Z;                                       // +0x04
    float ShoreLookUpArray[12];                    // +0x08
};

namespace std {

struct __false_type { };
struct random_access_iterator_tag { };

template <class _T1, class _T2> FABLE_FORCEINLINE void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
FABLE_FORCEINLINE _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
FABLE_FORCEINLINE _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
FABLE_FORCEINLINE _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
FABLE_FORCEINLINE _BidirectionalIter2 __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                           _BidirectionalIter2 __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _Tp>
FABLE_FORCEINLINE _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _ForwardIter, class _Tp>
FABLE_FORCEINLINE void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _Tp> class allocator {
public:
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                  // 0x00B7FFC0
public:
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}

template void vector<CTVertexWaterBackground>::_M_fill_insert(CTVertexWaterBackground*, unsigned int, const CTVertexWaterBackground&);

} // namespace std