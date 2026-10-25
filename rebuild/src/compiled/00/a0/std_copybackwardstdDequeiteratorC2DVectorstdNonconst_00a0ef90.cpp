// std::copy_backward(_Deque_iterator<C2DVector> first, _Deque_iterator<C2DVector> last, _Deque_iterator<C2DVector> result) --
//   STLport copy_backward() over deque iterators: forward to __copy_backward_aux with the not-both-pointers __false_type tag
// Leaf: ??$copy_backward@U?$_Deque_iterator@VC2DVector@@U?$_Nonconst_traits@VC2DVector@@@std@@@std@@U12@@std@@YI?AU?$_Deque_iterator@VC2DVector@@U?$_Nonconst_traits@VC2DVector@@@std@@@0@U10@00@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0EF90, length 116 bytes (+ int3 padding), `ret 30h`. First landed as
//   std::copy over an unidentified 8-byte "SPair8"; the catalog label CEnginePrimitiveRenderer::AddStaticMesh is propagated.
// Evidence: its only callee is 0x00A0ED80 __copy_backward_aux, which calls __copy_backward 0x00A0E850 (FINDINGS chunk 415);
//   its callers 0x00A0F173 / 0x00A0F94F sit in the deque<C2DVector> insert code of the same TU (Ego_r.pdb
//   CJoystickPositionTracker::RecentPositions is deque<C2DVector>).
// Shape: the TU is compiled with /Gr, so the function is `__fastcall`: the 16-byte iterator result comes back through the
//   hidden pointer in ecx (kept in esi and returned in eax), the three iterators arrive by value on the stack (`ret 30h`).
//   Each iterator has a user constructor, so every by-value argument of the forwarded call is built in its reserved slot;
//   the empty `__false_type` tag (user constructor: no zero store) is passed by const reference in edx and homed in the dead
//   `__result` argument slot. That sharing needs __copy_backward_aux as a same-TU noinline body (here STLport's own);
//   extern, VC7.1 opens a `push ecx` frame word for it.
class C2DVector {                                           // 8 bytes (Ego_r.pdb: deque<C2DVector> CJoystickPositionTracker::RecentPositions)
public:
    float X, Y;
    C2DVector() { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };

template <class _Tp> struct _Deque_iterator_base {          // STLport 4.6, 16 bytes
    enum _Constants { _blocksize = 128, __buffer_size = (sizeof(_Tp) < (unsigned int)_blocksize ? ((unsigned int)_blocksize / sizeof(_Tp)) : 1) };
    typedef int difference_type;
    typedef _Tp** _Map_pointer;
    typedef _Deque_iterator_base<_Tp> _Self;
    _Tp*         _M_cur;                                    // +0x00
    _Tp*         _M_first;                                  // +0x04
    _Tp*         _M_last;                                   // +0x08
    _Map_pointer _M_node;                                   // +0x0c
    _Deque_iterator_base() : _M_cur(0), _M_first(0), _M_last(0), _M_node(0) { }
    difference_type _M_subtract(const _Self& __x) const
    {
        return difference_type(__buffer_size) * (_M_node - __x._M_node - 1) + (_M_cur - _M_first) + (__x._M_last - __x._M_cur);
    }
    void _M_decrement()
    {
        if (_M_cur == _M_first) {
            _M_set_node(_M_node - 1);
            _M_cur = _M_last;
        }
        --_M_cur;
    }
    void _M_set_node(_Map_pointer __new_node)
    {
        _M_last = (_M_first = *(_M_node = __new_node)) + difference_type(__buffer_size);
    }
};

template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    typedef typename _Traits::reference reference;
    typedef int difference_type;
    typedef _Deque_iterator<_Tp, _Traits> _Self;
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > _Nonconst_self;
    _Deque_iterator() { }
    _Deque_iterator(const _Nonconst_self& __x) : _Deque_iterator_base<_Tp>(__x) { }
    reference operator*() const { return *this->_M_cur; }
    difference_type operator-(const _Self& __x) const { return this->_M_subtract(__x); }
    _Self& operator--() { this->_M_decrement(); return *this; }
};

// STLport 4.6 __copy_backward_aux @ 0x00A0ED80 (same TU and noinline for the same tag placement; __copy_backward 0x00A0E850 extern).
template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
_BidirectionalIter2 __fastcall __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result,
                                               const random_access_iterator_tag&, _Distance*);
#pragma auto_inline(off)
template <class _BidirectionalIter1, class _BidirectionalIter2>
__declspec(noinline) _BidirectionalIter2 __fastcall __copy_backward_aux(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result,
                                                                         const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
#pragma auto_inline(on)

template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 __fastcall copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward_aux(__first, __last, __result, __false_type());
}

typedef _Deque_iterator<C2DVector, _Nonconst_traits<C2DVector> > _C2DVectorDequeIt;
template _C2DVectorDequeIt __fastcall copy_backward(_C2DVectorDequeIt, _C2DVectorDequeIt, _C2DVectorDequeIt);
}
