// CDequeIterator8::operator+(int n) const -- STLport deque iterator plus offset: copy the iterator, advance the copy with operator+= and return it by value
// Leaf: ??HCDequeIterator8@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0E980, 81 bytes (+15 int3 pad). Callee:
// CDequeIterator8::operator+=(int) 0x00A0E2A0.
//
// Retail bytes:  sub esp,0x10 ; copy the four iterator words [ecx..ecx+0xc] into the frame
//   (interleaved with the `n` load) ; push n ; lea ecx,[tmp] ; call operator+= ;
//   mov eax,[hidden result] ; copy the four frame words to [eax] through ecx ; add esp,0x10 ; ret 8
// The manifest label (`_Vb_iterator<std::allocator<bool> >::operator+`) is propagated over a
// byte-identical body and is wrong here: the callee 0x00A0E2A0 is the STLport
// `_Deque_iterator::operator+=` (offset = n + (_M_cur - _M_first) / 8; inside the 16-slot buffer it
// bumps _M_cur, else `_M_set_node(_M_node + node_offset)` with a 0x80-byte chunk and re-bases
// _M_cur), so the 16-byte object is a deque iterator {_M_cur, _M_first, _M_last, _M_node} over an
// 8-byte element. Retail copies the result from the local, not from the reference operator+=
// returns, so the body is `tmp += n; return tmp;` (STLport's `_Self tmp = *this; tmp += n`), the
// iterator going back through the hidden result pointer (`ret 8` = result pointer + n).
struct SDequeElement8 {
    long m_a;
    long m_b;
};

struct CDequeIterator8 {
    SDequeElement8* _M_cur;                       // +0
    SDequeElement8* _M_first;                     // +4
    SDequeElement8* _M_last;                      // +8
    SDequeElement8** _M_node;                     // +0xc
    CDequeIterator8& operator+=(int n);           // 0x00A0E2A0
    CDequeIterator8 operator+(int n) const;
};

CDequeIterator8 CDequeIterator8::operator+(int n) const
{
    CDequeIterator8 tmp = *this;
    tmp += n;
    return tmp;
}