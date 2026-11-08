// std::vector<CTVertexLandscapeBackground>::_M_fill_insert(CTVertexLandscapeBackground* __position, unsigned int __n, const CTVertexLandscapeBackground& __x) -- STLport insert of n copies of a 24-byte landscape background vertex: inside the spare capacity slide the tail (uninitialised copy, copy_backward, fill) or fill past the end first, otherwise grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCTVertexLandscapeBackground@@V?$allocator@VCTVertexLandscapeBackground@@@std@@@std@@QAEXPAVCTVertexLandscapeBackground@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B6CE00, 674 bytes, thiscall `ret 0Ch`; speed TU.
// Identity (proven): (1) the sole caller is the landed vector<CTVertexLandscapeBackground>::resize(n, x) 0x00B6CD10
//   (`insert(end(), n - size(), x)` at 0x00B6CD69), which binds this address under exactly this spelling and whose own
//   identity rests on Ego_r.pdb CLandscapePatchTesselator::BridgeVertexBuffer (CArray<CTVertexLandscapeBackground>) and
//   the PDB name set's ?resize@?$vector@VCTVertexLandscapeBackground@@...; (2) the 24-byte stride (`imul 2AAAAAABh ; sar
//   edx,2` = /24, six-dword element copies) is Ego_r CTVertexLandscapeBackgroundBase (X, Y ushort; Z, NormalX/Y/Z float;
//   four UV bytes); (3) the growth path calls 0x00B6D0B0 with (pos, x, tag, n, false): STLport 4.6 _M_insert_overflow
//   (landed under the stand-in CRetailVector_SRecord24::InsertGrow -- NAME_FIXES.tsv); the body is _vector.c
//   _M_fill_insert verbatim, as the landed twin vector<CTVertexWaterBackground>::_M_fill_insert 0x00B7FDE0.
//   The catalog label "Vector_InsertFill_Vertices" is descriptive.
// Callees: _M_insert_overflow 0x00B6D0B0 (protected, out of line). Everything else is inline (placement-new null tests
//   on the uninitialised copies / fills, dword copies for the trivially copyable element).
// Shape: the 0x00B7FDE0 TU model: __forceinline STLport helpers (plain `inline` leaves them out of line), the
//   `__false_type()` tag of the overflow call zero-stored in a dead argument slot.
#include "fable_compiler.h"
inline void* operator new(unsigned int, void* p) { return p; }

class CTVertexLandscapeBackground {                // Ego_r.pdb, 24 bytes (CTVertexLandscapeBackgroundBase layout)
public:
    unsigned short X;                              // +0x00
    unsigned short Y;                              // +0x02
    float Z;                                       // +0x04
    float NormalX;                                 // +0x08
    float NormalY;                                 // +0x0C
    float NormalZ;                                 // +0x10
    unsigned char UVPad1;                          // +0x14
    unsigned char V1;                              // +0x15
    unsigned char U1;                              // +0x16
    unsigned char UVPad2;                          // +0x17
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
                            bool __atend = false);                                  // 0x00B6D0B0
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

template void vector<CTVertexLandscapeBackground>::_M_fill_insert(CTVertexLandscapeBackground*, unsigned int, const CTVertexLandscapeBackground&);

} // namespace std