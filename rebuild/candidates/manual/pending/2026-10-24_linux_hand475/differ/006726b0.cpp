// std::vector::_M_fill_insert(CRGBColour* position, unsigned int n, const CRGBColour& x) -- STLport insert of n copies of a colour: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Grade: DIFFER(311v307) -- helpers inline only under __forceinline (plain inline and implicit instantiation keep cdecl calls: 258v307); then ecx/edx swap in the inlined __uninitialized_copy cursor, the if-branch fill iterates ebp instead of a copy in eax, and the else-branch fill reloads x_copy inside the loop where retail hoists it
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006726B0, 320 bytes, thiscall `ret 0Ch`. Caller: vector<CRGBColour>::resize(n, x)
//   0x00672660 (`call 0x006726B0` at 0x006726A2), itself called only by GFSerialiseVectorBinaryIn<CRGBColour> 0x00672530,
//   so the 4-byte element is CRGBColour (the catalog's "ulong" is the propagated vector<unsigned int>::resize label).
// Callees: uninitialized_fill_n(first, n, x) 0x006727F0 (`__fastcall`, `ret 4`, null-tested placement copy loop),
//   _M_insert_overflow(position, x, const __false_type&, fill_len, atend) 0x00672820.
// Label correction: the catalog "Vector_InsertN_ulong" -- this is STLport 4.6 `vector<CRGBColour>::_M_fill_insert`.
// Shape: STLport 4.6 `_M_fill_insert` verbatim for a non-POD element (no __type_traits specialisation, so `_IsPODType` and
//   `_TrivialAss` are __false_type): speed TU, so __uninitialized_copy (placement copies through the non-throwing
//   placement new: `test ecx,ecx ; je`), __copy_backward and both `fill` loops are inlined; only uninitialized_fill_n stays
//   out of line (same-TU noinline body so VC7.1 knows its clobbers). x_copy lives in the dead x slot, elems_after (later
//   n * 4) in the dead n slot; the overflow call's `__false_type()` has NO user constructor, so VC7.1 value-initialises it
//   with `mov byte ptr [n slot],0`.
// Model: CRGBColour as the landed B, G, R, A union over a packed dword (implicit copy = one dword move).
inline void* operator new(unsigned int, void* __p) throw() { return __p; }

struct CRGBColour {
    union {
        struct { unsigned char B, G, R, A; };
        unsigned long Colour;
    };
};

namespace std {
struct __false_type { };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__forceinline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                          const random_access_iterator_tag&, _Distance*)
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

#pragma auto_inline(off)
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{                                                                                 // 0x006727F0
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp> class allocator {
public:
    allocator() { }
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
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                               // 0x00672820
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            } else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        } else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}

template void vector<CRGBColour>::_M_fill_insert(CRGBColour*, unsigned int, const CRGBColour&);
}
