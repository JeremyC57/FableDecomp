// Behaviour test for 0x00A0EF90 std::copy_backward over deque<C2DVector> iterators (links the source object, whose
// __copy_backward_aux is STLport's own body); __copy_backward 0x00A0E850 is the STLport body, specialised here.
#include <cstdio>
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

template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
_BidirectionalIter2 __fastcall __copy_backward(_BidirectionalIter1, _BidirectionalIter1, _BidirectionalIter2, const random_access_iterator_tag&, _Distance*);
template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 __fastcall copy_backward(_BidirectionalIter1, _BidirectionalIter1, _BidirectionalIter2);
typedef _Deque_iterator<C2DVector, _Nonconst_traits<C2DVector> > _C2DVectorDequeIt;
static int g_inner = 0;
template <> _C2DVectorDequeIt __fastcall __copy_backward<_C2DVectorDequeIt, _C2DVectorDequeIt, int>(_C2DVectorDequeIt __first, _C2DVectorDequeIt __last,
    _C2DVectorDequeIt __result, const random_access_iterator_tag&, int* __d)
{
    ++g_inner;
    if (__d != 0) g_inner += 100;
    for (int __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
}
using std::_C2DVectorDequeIt;
using std::g_inner;
#define CALL(a, b, c) std::copy_backward(a, b, c)

static C2DVector g_buf[64];                                  // four nodes of 16 elements
static C2DVector* g_map[4] = { g_buf, g_buf + 16, g_buf + 32, g_buf + 48 };
static _C2DVectorDequeIt mk(int index)
{
    _C2DVectorDequeIt it;
    it._M_node = &g_map[index / 16]; it._M_first = *it._M_node; it._M_last = it._M_first + 16; it._M_cur = it._M_first + index % 16;
    return it;
}
int main()
{
    for (int i = 0; i < 64; ++i) { g_buf[i].X = (float)i; g_buf[i].Y = (float)(-i); }
    // copy elements [10, 20) to end at 26 (overlapping, crossing the node boundary at 16): backward copy keeps the values
    _C2DVectorDequeIt r = CALL(mk(10), mk(20), mk(26));
    int ok = g_inner == 1 && r._M_cur == g_buf + 16 && r._M_node == &g_map[1] && r._M_first == g_buf + 16;
    for (int i = 16; i < 26; ++i) ok = ok && g_buf[i].X == (float)(i - 6) && g_buf[i].Y == (float)(6 - i);
    for (int i = 10; i < 16; ++i) ok = ok && g_buf[i].X == (float)i;
    ok = ok && g_buf[26].X == 26.0f;
    if (!ok) { std::printf("00a0ef90_TEST FAIL\n"); return 1; }
    std::printf("00a0ef90_TEST PASS\n");
    return 0;
}
