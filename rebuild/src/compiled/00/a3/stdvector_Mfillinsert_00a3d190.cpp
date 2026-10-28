// std::vector::_M_fill_insert(pointer position, size_type n, const value_type& x) -- (vector<C3DAnimationStateAlpha>) STLport 4.6 insert of n copies of a polymorphic 20-byte animation state: shift the tail inside the spare capacity, else grow through the __false_type _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VC3DAnimationStateAlpha@@V?$allocator@VC3DAnimationStateAlpha@@@std@@@std@@QAEXPAVC3DAnimationStateAlpha@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3D190, 325 bytes, thiscall `ret 0Ch`.
// Identity (proven): STLport 4.6 vector<_Tp>::_M_fill_insert (public QAE); its only caller is the landed
//   vector<C3DAnimationStateAlpha>::resize(n, x) 0x00A3D500 (0x00A3D559), its overflow callee 0x00A3D060 (this chunk).
//   Catalog label "Vector_Insert_CDisplayModeDesc" is propagated.
// Element (proven): C3DAnimationStateAlpha -- the copy stores the three vtables 0x012354D0 (CIVCountedPointeeBase) /
//   0x01249F50 (C3DAnimationState) / 0x01265128 (C3DAnimationStateAlpha) and copies IVRefCount +4, AnimationIndex +8,
//   AnimationTime +0xC, AlphaWeight +0x10 (Ego_r.pdb layout, 20 bytes; 66666667h / sar 3 stride); the vector is
//   C3DAnimationBlendState::CSharedData's combination-blend vector (landed push_back 0x00A3D380, resize 0x00A3D500
//   bound `vector<C3DAnimationStateAlpha>::resize`). The base destructor is a user-written inline virtual: the ??_G in
//   slot 0 (0x0070FE30) stores only 0x012354D0, and retail destroys elements through `p->~T()` (push 0 ; call [eax]).
// Callees: __uninitialized_copy 0x00A3CB90, __copy_backward<.., int> 0x00A3CD00 (random-access tag, (int*)0),
//   __uninitialized_fill_n 0x00A3CBE0, fill<C3DAnimationStateAlpha*, C3DAnimationStateAlpha> 0x00A3CEE0 (all landed,
//   same retail TU), _M_insert_overflow 0x00A3D060.
// Shape: STLport 4.6 vector.c verbatim; x_copy is built inline (one vtable store) and destroyed by the inline empty
//   destructor chain (no call); the four helpers get same-TU noinline bodies (retail holds them in this TU), which
//   lets VC7.1 tail-duplicate fill + epilogue into both branches as retail. At landing the retail COMDATs win.
inline void* operator new(unsigned int, void* p) { return p; }

class CIVCountedPointeeBase {                               // vtable 0x012354D0
public:
    virtual ~CIVCountedPointeeBase() { }                    // slot 0 (inline: ??_G stores only this vtable)
    unsigned long IVRefCount;                               // +0x04
};

class C3DAnimationState : public CIVCountedPointeeBase {    // vtable 0x01249F50
public:
    virtual long GetType() const;
    virtual void Validate();
    long  AnimationIndex;                                   // +0x08
    float AnimationTime;                                    // +0x0C
};

class C3DAnimationStateAlpha : public C3DAnimationState {   // vtable 0x01265128, sizeof 0x14
public:
    virtual long GetType() const;                           // 0x0070FE20
    virtual void Validate();                                // 0x00A3BE20
    float AlphaWeight;                                      // +0x10
};

namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                   // 0x00A3CB90
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                   // 0x00A3CBE0
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                   // 0x00A3CD00
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                   // 0x00A3CEE0
    for (; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_backward_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
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
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                               // 0x00A3D060
public:
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
} // namespace std

template void std::vector<C3DAnimationStateAlpha>::_M_fill_insert(C3DAnimationStateAlpha*, unsigned int, const C3DAnimationStateAlpha&);