// _STL::_Destroy(_STL::_Deque_iterator<C2DVector, _Nonconst_traits<C2DVector> > first, _STL::_Deque_iterator<C2DVector, _Nonconst_traits<C2DVector> > last) -- destroy a range of a deque of 2D vectors (forwards to the out-of-line non-trivial-destructor loop)
// Leaf: ??$_Destroy@U?$_Deque_iterator@VC2DVector@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048D490, 76 bytes, `__fastcall` with no register arguments (the two
//   16-byte iterators on the stack), `ret 20h`.
// Identity: the callee 0x0048D050 is STLport `__destroy_aux(first, last, const __false_type&)` over a deque of
//   8-byte elements (`add eax,8`, 0x80-byte blocks) whose per-element destroy is empty -- an unspecialised
//   __type_traits class with a trivial destructor. This TU is CInputProcessJumpingAndRolling's (vtable 0x01238050,
//   constructor 0x0048D080), and Ego_r.pdb's CJoystickPositionTracker::RecentPositions is deque<C2DVector>;
//   the twin 0x00A0EAF0 (uninitialized_fill over the same iterators) copies 8-byte C2DVectors. Element type is
//   therefore C2DVector (any trivially destructible 8-byte class gives the same bytes). The manifest's
//   "for_each<_Vector_iterator<C3DVector>, CGetBestIciclePointForTarget>" is propagated. Callers 0x00A0F1C2,
//   0x00A0F290 (COMDAT shared across TUs).
// Callees: __destroy_aux<CDequeIter>(CDequeIter, CDequeIter, const __false_type&) @ 0x0048D050 -- in this same retail
//   TU, so its real body is kept here as a same-TU `__declspec(noinline)` definition under `auto_inline(off)`: that is
//   what lets VC7.1 home the tag temporary in the dead `last` slot (with a declaration only it takes a fresh frame slot).
// Shape: STLport 4.6 `_Destroy(first, last)` -> inline `__destroy(first, last, value_type(first))` ->
//   `__destroy_aux(first, last, _Trivial_destructor())`; the iterators are copied right to left into the outgoing
//   area and the tag temporary (passed by const reference, so its address goes in ecx) is homed in `last`'s
//   dead argument slot.
class C2DVector {                                        // 8 bytes
public:
    float X, Y;
};

namespace _STL {
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
typedef _STL::_Deque_iterator<C2DVector, _STL::_Nonconst_traits<C2DVector> > CDequeIter;

namespace _STL {
#pragma auto_inline(off)
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)   // 0x0048D050
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _ForwardIterator, class _Tp>
inline void __fastcall __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}

template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, value_type(__first));
}
}

template void __fastcall _STL::_Destroy<CDequeIter>(CDequeIter, CDequeIter);