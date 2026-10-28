// std::vector::_M_fill_insert(iterator position, size_type n, const value_type& x) -- (vector<std::pair<long, CRandomAppearanceMorph::CSkeletalMorph> >) STLport insert of n copies of a 16-byte skeletal-morph entry: slide the tail inside the spare capacity (uninitialized copy, copy_backward, fill from a copy of x), or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@U?$pair@JVCSkeletalMorph@CRandomAppearanceMorph@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086CD70, 287 bytes, thiscall, `ret 0Ch`.
// Identity (proven): the no-room arm calls the landed vector<pair<long, CRandomAppearanceMorph::CSkeletalMorph> >
//   ::_M_insert_overflow 0x0086CC50 (bound over that pair), and its helpers __uninitialized_copy 0x0086ABF0 /
//   __uninitialized_fill_n 0x0086AC40 are bound over the same pair; the element is 16 bytes (`sar 4`) of POD: long key +
//   Ego_r.pdb CRandomAppearanceMorph::CSkeletalMorph {long GroupID; CDefString Name; bool AllowVariation;}. Callers:
//   0x0086C2D8, the insert wrapper 0x0086D470 (jmp), 0x0086D85A, 0x0086DD06 (the CRandomAppearanceMorph skeletal-morph map).
//   The catalog's "C3DClothQuad" is a propagated label of a byte-identical 16-byte instantiation.
// Callees: __uninitialized_copy 0x0086ABF0 (twice), __copy_backward(.., const random_access_iterator_tag&, int*) 0x0086A4A0,
//   fill 0x0086A3F0, __uninitialized_fill_n 0x0086AC40 (through the inline tag-adding uninitialized_fill_n),
//   _M_insert_overflow 0x0086CC50 (`push 0`: atend defaulted).
// Model: the landed shape twin 0x0098D360 (vector<C4DVector>::_M_fill_insert), STLport 4.6 verbatim with same-TU noinline
//   helper bodies; the pair carries STLport's explicit copy constructor (as the landed 0x0086CC50), and the element is POD,
//   so x_copy is four dword moves and needs no destructor.
inline void* operator new(unsigned int, void* p) { return p; }

class CDefString {
public:
    long TablePos;                                   // +0x00
};

class CRandomAppearanceMorph {
public:
    class CSkeletalMorph {                           // Ego_r.pdb, 12 bytes of POD
    public:
        long       GroupID;                          // +0x00
        CDefString Name;                             // +0x04
        bool       AllowVariation;                   // +0x08
    };
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;                                       // +0x00
    _T2 second;                                      // +0x04
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit pair copy constructor
};
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                               // 0x0086ABF0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                               // 0x0086AC40
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                               // 0x0086A4A0
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                               // 0x0086A3F0
    for (; __first != __last; ++__first)
        *__first = __val;
}

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
    typedef _Tp* iterator;
    typedef __false_type _IsPODType;
    typedef __false_type _TrivialAss;
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                              // 0x0086CC50
    public:
    void _M_fill_insert(iterator __position, size_type __n, const _Tp& __x);
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

typedef pair<long, CRandomAppearanceMorph::CSkeletalMorph> TSkeletalMorphPair;
template void vector<TSkeletalMorphPair>::_M_fill_insert(TSkeletalMorphPair*, unsigned int, const TSkeletalMorphPair&);

}