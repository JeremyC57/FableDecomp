// std::_Deque_iterator<C2DVector, std::_Nonconst_traits<C2DVector> >::operator+(int n) const -- STLport deque iterator plus
// offset: copy the iterator, advance the copy (operator+= inline, i.e. _M_advance) and return it by value
// Leaf: ??H?$_Deque_iterator@VC2DVector@@U?$_Nonconst_traits@VC2DVector@@@std@@@std@@QBE?AU01@H@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0E980, 81 bytes (+15 int3 pad). First landed as CDequeIterator8::operator+
//   over a placeholder 8-byte element, with its callee called operator+=; the manifest label
//   `_Vb_iterator<std::allocator<bool> >::operator+` is propagated over a byte-identical body.
// Callee: _Deque_iterator_base<C2DVector>::_M_advance(int) @ 0x00A0E2A0 (FINDINGS chunk 415: STLport `_M_advance`, void; the
//   inline `operator+=` is `{ this->_M_advance(__n); return *this; }`, so the reference it returns is never read -- retail
//   copies the result from the local).
// Evidence: its callers are the deque<C2DVector> members of FINDINGS chunk 415 (resize 0x00A0FC80 calls it out of line on
//   `begin() + __new_size`, _M_reserve_elements_at_front 0x00A0F430, the insert helpers 0x00A0F040 / 0x00A0F4B0 / 0x00A0F6EA /
//   0x00A0F98B); Ego_r.pdb CJoystickPositionTracker::RecentPositions is deque<C2DVector> (8-byte element, 0x80-byte nodes).
// Retail bytes:  sub esp,0x10 ; copy the four iterator words [ecx..ecx+0xc] into the frame (interleaved with the `n` load) ;
//   push n ; lea ecx,[tmp] ; call _M_advance ; mov eax,[hidden result] ; copy the four frame words to [eax] through ecx ;
//   add esp,0x10 ; ret 8 (result pointer + n).
class C2DVector {                                           // 8 bytes
public:
    float X, Y;
    C2DVector() { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};

namespace std {
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };

template <class _Tp> struct _Deque_iterator_base {          // STLport 4.6, 16 bytes
    typedef int difference_type;
    typedef _Tp** _Map_pointer;
    _Tp*         _M_cur;                                    // +0x00
    _Tp*         _M_first;                                  // +0x04
    _Tp*         _M_last;                                   // +0x08
    _Map_pointer _M_node;                                   // +0x0c
    _Deque_iterator_base() : _M_cur(0), _M_first(0), _M_last(0), _M_node(0) { }
    void _M_advance(difference_type __n);                   // 0x00A0E2A0
};

template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    typedef int difference_type;
    typedef _Deque_iterator<_Tp, _Traits> _Self;
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > _Nonconst_self;
    _Deque_iterator() { }
    _Deque_iterator(const _Nonconst_self& __x) : _Deque_iterator_base<_Tp>(__x) { }
    _Self& operator+=(difference_type __n) { this->_M_advance(__n); return *this; }
    _Self operator+(difference_type __n) const
    {
        _Self __tmp = *this;
        return __tmp += __n;
    }
};

template _Deque_iterator<C2DVector, _Nonconst_traits<C2DVector> >
    _Deque_iterator<C2DVector, _Nonconst_traits<C2DVector> >::operator+(int) const;
}
