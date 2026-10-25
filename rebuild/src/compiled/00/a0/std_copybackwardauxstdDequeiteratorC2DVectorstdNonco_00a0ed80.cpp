// std::__copy_backward_aux(_Deque_iterator<C2DVector> first, _Deque_iterator<C2DVector> last, _Deque_iterator<C2DVector> result,
//   const __false_type&) -- STLport copy_backward() step for deque iterators: forward to the random-access __copy_backward
//   with the iterator category and a null distance pointer
// Leaf: ??$__copy_backward_aux@U?$_Deque_iterator@VC2DVector@@U?$_Nonconst_traits@VC2DVector@@@std@@@std@@U12@@std@@YI?AU?$_Deque_iterator@VC2DVector@@U?$_Nonconst_traits@VC2DVector@@@std@@@0@U10@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0ED80, 118 bytes (+ int3 padding) (`ret 30h`). First landed as std::__copy_aux over an
//   unidentified 8-byte "SPair8" with its callee given as 0x00A0E860; the catalog label CNavigatorAStar::
//   MoveToNavLineOfSightProximity is propagated.
// Evidence: the call target is 0x00A0E850, STLport's random-access __copy_backward over deque<C2DVector> iterators (FINDINGS
//   chunk 415, graded there: `for (__n = __last - __first; __n > 0; --__n) *--__result = *--__last;`), and the only caller is
//   copy_backward 0x00A0EF90; the deque is deque<C2DVector> (Ego_r.pdb CJoystickPositionTracker::RecentPositions, 8-byte
//   elements, 0x80-byte nodes), the instantiation of the eighteen STLport deque rows 0x00A0E2A0..0x00A0FC80 around it.
// Shape: the TU's free functions are `__fastcall` (/Gr): ecx = the hidden result iterator (returned in eax), edx = the unused
//   `const __false_type&`, three 16-byte iterators by value (0x30 bytes). The empty tag temporary lives in the dead slot of
//   `__result` and goes in edx by reference only when __copy_backward (the const-ref taker of the tag) is a same-TU noinline
//   body; extern, VC7.1 opens a `push ecx` frame word for it.
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

// STLport 4.6 __copy_backward @ 0x00A0E850 (its own body, same TU and noinline: the empty tag is then homed in the dead
// `__result` argument slot as retail does; extern, VC7.1 opens a `push ecx` frame word for it).
#pragma auto_inline(off)
template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
__declspec(noinline) _BidirectionalIter2 __fastcall __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result,
                                                                     const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
#pragma auto_inline(on)

template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 __fastcall __copy_backward_aux(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result,
                                                   const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

typedef _Deque_iterator<C2DVector, _Nonconst_traits<C2DVector> > _C2DVectorDequeIt;
template _C2DVectorDequeIt __fastcall __copy_backward_aux(_C2DVectorDequeIt, _C2DVectorDequeIt, _C2DVectorDequeIt, const __false_type&);
}
