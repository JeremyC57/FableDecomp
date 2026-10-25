// std::uninitialized_fill(std::_Deque_iterator<C2DVector, _Nonconst_traits<C2DVector> > first, std::_Deque_iterator<C2DVector, _Nonconst_traits<C2DVector> > last, const C2DVector& x) -- copy-construct one 2D vector into every slot of a deque range (forwards to the out-of-line non-POD loop)
// Leaf: ??$uninitialized_fill@U?$_Deque_iterator@VC2DVector@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0EAF0, 78 bytes, `__fastcall` (the value reference in ecx, the two
//   16-byte iterators on the stack), `ret 20h`.
// Identity: the callee 0x00A0E740 is STLport `__uninitialized_fill(first, last, const T& x, const __false_type&)`
//   over a deque of 8-byte elements (`add eax,8`, 0x80-byte blocks): `for (; cur != last; ++cur) _Construct(&*cur,
//   x)` with the placement-new null test and a two-dword copy. The same deque<C2DVector> as 0x0048D490 (the
//   CInputProcessJumpingAndRolling / CJoystickPositionTracker::RecentPositions family); the element type C2DVector
//   is inferred (any 8-byte non-POD class gives the same bytes). The manifest's "for_each<_Vector_iterator<C3DVector>,
//   CGetBestIciclePointForTarget>" is propagated. Callers 0x00A0FB38, 0x00A0FBE6.
// Callees: __uninitialized_fill<CDequeIter, C2DVector>(CDequeIter, CDequeIter, const C2DVector&, const __false_type&)
//   @ 0x00A0E740 -- in this same retail TU, so its real body is kept here as a same-TU `__declspec(noinline)` definition
//   under `auto_inline(off)`; with a declaration only, VC7.1 gives the tag temporary a fresh frame slot.
// Shape: STLport 4.6 `uninitialized_fill(first, last, x)` -> `__uninitialized_fill(first, last, x,
//   _IS_POD_ITER(first, _Tp))`: x stays in ecx, the POD tag (by const reference) goes in edx, the iterators are
//   copied right to left into the outgoing area through esi.
class C2DVector {                                        // 8 bytes
public:
    float X, Y;
};

namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };              // user constructor: `__false_type()` is not zero-stored
template <class _Tp> struct __type_traits {              // the unspecialised STLport traits: nothing is trivial
    typedef __false_type has_trivial_destructor;
    typedef __false_type is_POD_type;
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp> struct _Deque_iterator_base {       // 16 bytes
    typedef _Tp** _Map_pointer;
    _Tp*         _M_cur;                                 // +0x00
    _Tp*         _M_first;                               // +0x04
    _Tp*         _M_last;                                // +0x08
    _Map_pointer _M_node;                                // +0x0c
    static unsigned int _S_buffer_size() { return sizeof(_Tp) < 128 ? (128 / sizeof(_Tp)) : 1; }
    void _M_set_node(_Map_pointer __new_node)
    {
        _M_last = (_M_first = *(_M_node = __new_node)) + int(_S_buffer_size());
    }
    void _M_increment()
    {
        if (++_M_cur == _M_last) {
            _M_set_node(_M_node + 1);
            _M_cur = _M_first;
        }
    }
};
template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    typedef _Tp value_type;
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _Deque_iterator() { }
    _Deque_iterator(const iterator& __x) : _Deque_iterator_base<_Tp>(__x) { }   // the STLport user copy constructor
    _Tp& operator*() const { return *this->_M_cur; }
    _Deque_iterator& operator++() { this->_M_increment(); return *this; }
    bool operator!=(const _Deque_iterator& __y) const { return this->_M_cur != __y._M_cur; }
};
template <class _Tp, class _Traits> inline _Tp* value_type(const _Deque_iterator<_Tp, _Traits>&) { return (_Tp*)0; }
}
typedef std::_Deque_iterator<C2DVector, std::_Nonconst_traits<C2DVector> > CDequeIter;

inline void* operator new(unsigned int, void* __p) { return __p; }

namespace std {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

#pragma auto_inline(off)
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall __uninitialized_fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __x, const __false_type&)   // 0x00A0E740
{
    _ForwardIter __cur = __first;
    for ( ; __cur != __last; ++__cur)
        _Construct(&*__cur, __x);
}
#pragma auto_inline(on)

template <class _Tp> struct _IsPOD {                     // STLport 4.6 type_traits.h (no partial specialisation)
    typedef typename __type_traits<_Tp>::is_POD_type _Type;
    static _Type __fastcall _Answer() { return _Type(); }
};
template <class _Tp> inline _IsPOD<_Tp> __fastcall _Is_POD(_Tp*) { return _IsPOD<_Tp>(); }

template <class _ForwardIter, class _Tp>
void __fastcall uninitialized_fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __x)
{
    __uninitialized_fill(__first, __last, __x, _Is_POD(value_type(__first))._Answer());   // _IS_POD_ITER(__first, _Tp)
}
}

template void __fastcall std::uninitialized_fill<CDequeIter, C2DVector>(CDequeIter, CDequeIter, const C2DVector&);