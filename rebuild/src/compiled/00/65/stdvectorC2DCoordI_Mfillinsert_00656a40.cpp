// std::vector<C2DCoordI>::_M_fill_insert(pointer position, unsigned int n, const C2DCoordI& x) -- STLport insert of n copies of x before position (in place when capacity allows, else the growth path)
// Leaf: _M_fill_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x00656A40, 219 bytes, thiscall `ret 0Ch`. Callers: vector<C2DCoordI>::resize(n, x)
// @ 0x00656FAB (jmp 0x00656EED / call 0x00656FDF), which the landed CDrawQuickAccessIcons constructor 0x00652F0A calls on
// its EffectPositions member, so the element is C2DCoordI (Ego_r.pdb: long X, Y).
// Callees: __uninitialized_copy @ 0x00612830, __copy_backward(.., const random_access_iterator_tag&, int*) @ 0x00654EF4,
// fill @ 0x00654E67, __uninitialized_fill_n @ 0x00612860 (all `__fastcall`, shared with other 8-byte element types by
// identical-COMDAT folding), _M_insert_overflow @ 0x00618550 (growth path, atEnd defaulted false).
// Label correction: the catalog "std_vector_insert_CObjectFamilyEntry_range" is propagated; this is STLport 4.x
// `vector<C2DCoordI>::_M_fill_insert`.
// Shape: the same STLport body as the CIntelligentPointer instance 0x006568F2; the POD x_copy is two dword copies into
// the frame ([ebp-8], [ebp-4]), and VC7.1 tail-merges the two `fill(.., &x_copy)` calls into one block. The empty tag
// temporaries share the dead high byte of the position slot ([ebp+0Bh]): same-TU `auto_inline(off)` noinline template
// bodies of the tag-taking helpers with the tag parameter unnamed (GOTCHAS empty-tag rule).
// Opt: frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)

inline void* operator new(unsigned int, void* p) { return p; }

class C2DCoordI {
public:
    long X;                                          // +0x00
    long Y;                                          // +0x04
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> __forceinline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Copy_Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Copy_Construct(&*__cur, __x);
    return __cur;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)

template <class _Tp>
__forceinline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    pointer _M_start;                                                   // +0x00
    pointer _M_finish;                                                  // +0x04
    _STLP_alloc_proxy<pointer, _Tp, allocator<_Tp> > _M_end_of_storage; // +0x08
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x00618550
    void _M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x);
};

template <class _Tp>
void vector<_Tp>::_M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x)
{
    if (__n != 0) {
        if ((unsigned int)(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const unsigned int __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                __uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy, __false_type());
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

template void vector<C2DCoordI>::_M_fill_insert(C2DCoordI*, unsigned int, const C2DCoordI&);
}