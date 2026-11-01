// std::vector::_M_fill_insert(CTVertexSeaBackground* position, unsigned int n, const CTVertexSeaBackground& x) -- STLport 4.6 vector<CTVertexSeaBackground> fill insert: with room, shift the tail by n (copy the last n into raw storage, copy_backward the rest, fill x) or fill the raw part first; without room, grow through _M_insert_overflow
// Grade: DIFFER(523v523) -- one scheduling residue: in the else-branch final fill(position, old_finish, x_copy) retail loads x_copy.X, Y, Z into edx/esi/edi in order; VC here hoists the Z load (edi) above X/Y (12 probes: fill/element/finish variants, none moved it). Next step: try the loop with the x_copy fields read through a const reference bound before the loop, or the landed 0x00B7FDE0 TU model compiled with its other members (resize 0x00B80120, _M_insert_overflow 0x00B80400) in the same TU for register-allocation facts.
// Leaf: ?_M_fill_insert@?$vector@VCTVertexSeaBackground@@V?$allocator@VCTVertexSeaBackground@@@std@@@std@@QAEXPAVCTVertexSeaBackground@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B801F0, 523 bytes, thiscall, `ret 0Ch`.
// Identity (proven): the one caller is 0x00B80177 inside the landed vector<CTVertexSeaBackground>::resize(n, x) 0x00B80120
//   (`insert(end(), new_size - size(), x)` forwarding to _M_fill_insert at this address); the grow callee at 0x00B803EE is the
//   landed vector<CTVertexSeaBackground>::_M_insert_overflow 0x00B80400 (position, x, __false_type, n, false: `push 0 ; push edi`
//   and the tag byte in the frame); 12-byte stride (0x2AAAAAAB reciprocal); the PDB name sets carry the
//   ?_M_fill_insert@?$vector@VCTVertexSeaBackground@@... spelling. T = CTVertexSeaBackground (Ego_r.pdb: float X, Y, Z,
//   implicit copy: the dword moves of x into __x_copy and of each element).
// Shape: STLport 4.6 _vector.c text verbatim, the landed vector<CTVertexWaterBackground>::_M_fill_insert 0x00B7FDE0 model
//   (same water-tesselator TU family) with the 12-byte element: __forceinline __uninitialized_copy / uninitialized_fill_n
//   placement-new loops with the null test on the destination, the counted random-access __copy_backward, plain fill.
// Label: the manifest "Vector_Insert_3Floats" is descriptive; the real name is the STLport vector member above.
inline void* operator new(unsigned int, void* p) { return p; }

class CTVertexSeaBackground {                      // Ego_r.pdb, 12 bytes (float X, Y, Z), implicit copy
public:
    float X, Y, Z;                                 // +0 / +4 / +8
};

namespace std {

struct __false_type { };
struct random_access_iterator_tag { };

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
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

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
__forceinline _BidirectionalIter2 __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                           _BidirectionalIter2 __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _Tp>
__forceinline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _ForwardIter, class _Tp>
__forceinline void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
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
                            bool __atend = false);                                  // 0x00B80400
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

template void vector<CTVertexSeaBackground>::_M_fill_insert(CTVertexSeaBackground*, unsigned int, const CTVertexSeaBackground&);

} // namespace std