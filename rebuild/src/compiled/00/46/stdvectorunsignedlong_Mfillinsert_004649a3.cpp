// std::vector<unsigned long>::_M_fill_insert(unsigned long* __position, unsigned int __n, const unsigned long& __x) -- STLport 4.6 insert of n copies of x: inside the spare capacity move the tail up and fill the gap, else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@KV?$allocator@K@std@@@std@@QAEXPAKIABK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004649A3, 264 bytes, thiscall `ret 0Ch`; size TU with a frame pointer (unaligned entry, `leave`).
// Identity (proven): std::vector<unsigned long>::_M_fill_insert(iterator, size_type, const unsigned long&); evidence: (1) the body
//   is STLport 4.6 _vector.c _M_fill_insert with the POD helpers on a 4-byte element (the `rep stosd` uninitialized_fill_n, dword
//   fill loops, memmove 0x00BFEAE6), the same TU shape as the landed vector<bool>::_M_fill_insert 0x004676EF; (2) its callees are
//   the landed std::__uninitialized_copy<unsigned long*, unsigned long*>(..., const __true_type&) 0x00464AAB and
//   vector<unsigned long>::_M_insert_overflow 0x00464ACE (bound with the STLport spelling); (3) family evidence (agent952 H056):
//   C3DMeshFileXAnimatedPrimitiveBlockChunk::ReadFromFile resizes VertexList (Ego_r CArray<unsigned long> : vector<unsigned long>)
//   through it, and its _M_insert_overflow also grows CVertexGroupBlock::Groups. _M_fill_insert is public in STLport 4.6 (QAE).
// Label: the catalog's "std::vector_insert_range_undefined4" is behavioural; the bindings ?_M_fill_insert@CULongVector@@ and
//   ?InsertN@?$CArray@K@@ are invented spellings for this address.
// Shape: the landed 0x004676EF recipe with the element swapped: x_copy is homed in a frame word ([ebp-4]); the two
//   __uninitialized_copy tags come from `_IsPOD<T>::_Answer()` (zero-stored then copied in place), the overflow tag is the plain
//   `_IsPODType()` temporary.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);   // 0x00BFEAE6

namespace std {

struct __true_type { };

template <class _Tp> struct _IsPOD {
    typedef __true_type _Type;
    static __forceinline _Type _Answer() { return _Type(); }
};

// STLport 4.6 `__uninitialized_copy(_InputIter, _InputIter, _ForwardIter, const __true_type&)`; called here with `pointer`
// arguments, so this TU's instance is <unsigned long*, unsigned long*> 0x00464AAB.
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __true_type&);                                              // 0x00464AAB

__forceinline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const int _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : __result;
}

template <class _Tp> __forceinline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial_backward(__first, __last, __result);
}

template <class _Tp> __forceinline void fill(_Tp* __first, _Tp* __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _Tp> __forceinline _Tp* fill_n(_Tp* __first, unsigned int __n, const _Tp& __val)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}

template <class _Tp> __forceinline _Tp* __uninitialized_fill_n(_Tp* __first, unsigned int __n, const _Tp& __x, const __true_type&)
{
    return fill_n(__first, __n, __x);
}

template <class _Tp> __forceinline _Tp* uninitialized_fill_n(_Tp* __first, unsigned int __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __true_type());
}

template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;
    typedef __true_type _IsPODType;
    typedef __true_type _TrivialAss;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;

protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, unsigned int __fill_len,
                            bool __atend = false);                                                    // 0x00464ACE (protected)
public:
    void _M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x)
{
    if (__n != 0) {
        if ((unsigned int)(this->_M_end_of_storage._M_data - _M_finish) >= __n) {
            _Tp __x_copy = __x;
            const unsigned int __elems_after = _M_finish - __position;
            pointer __old_finish = _M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(_M_finish - __n, _M_finish, _M_finish, _IsPOD<_Tp>::_Answer());
                _M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy);
                _M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, _M_finish, _IsPOD<_Tp>::_Answer());
                _M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}

template void vector<unsigned long, allocator<unsigned long> >::_M_fill_insert(unsigned long*, unsigned int, const unsigned long&);

}