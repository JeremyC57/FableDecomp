// Behaviour test for 0x00A0E980 std::_Deque_iterator<C2DVector, _Nonconst_traits>::operator+ (links the source object);
// _M_advance 0x00A0E2A0 is the STLport body, specialised here; the original iterator is left untouched.
#include <cstdio>
class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};
namespace std {
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Deque_iterator_base {
    typedef int difference_type;
    typedef _Tp** _Map_pointer;
    enum { __buffer_size = 16 };
    _Tp* _M_cur; _Tp* _M_first; _Tp* _M_last; _Map_pointer _M_node;
    _Deque_iterator_base() : _M_cur(0), _M_first(0), _M_last(0), _M_node(0) { }
    void _M_advance(difference_type __n);
    void _M_set_node(_Map_pointer __new_node) { _M_last = (_M_first = *(_M_node = __new_node)) + difference_type(__buffer_size); }
};
template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    typedef int difference_type;
    typedef _Deque_iterator<_Tp, _Traits> _Self;
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > _Nonconst_self;
    _Deque_iterator() { }
    _Deque_iterator(const _Nonconst_self& __x) : _Deque_iterator_base<_Tp>(__x) { }
    _Self operator+(difference_type __n) const;             // defined in the source object
};
template <> void _Deque_iterator_base<C2DVector>::_M_advance(int __n)
{
    int __offset = __n + (int)(_M_cur - _M_first);
    if (__offset >= 0 && __offset < int(__buffer_size))
        _M_cur += __n;
    else {
        int __node_offset = __offset > 0 ? __offset / int(__buffer_size) : -((-__offset - 1) / int(__buffer_size)) - 1;
        _M_set_node(_M_node + __node_offset);
        _M_cur = _M_first + (__offset - __node_offset * int(__buffer_size));
    }
}
}
typedef std::_Deque_iterator<C2DVector, std::_Nonconst_traits<C2DVector> > It;
static int Fail() { std::printf("00a0e980_TEST FAIL\n"); return 1; }
int main()
{
    static C2DVector nodes[3][16];
    C2DVector* map[3] = { nodes[0], nodes[1], nodes[2] };
    for (int n = 0; n < 3; ++n)
        for (int i = 0; i < 16; ++i) { nodes[n][i].X = (float)(n * 16 + i); nodes[n][i].Y = (float)-(n * 16 + i); }
    It it;
    it._M_set_node(&map[1]);
    it._M_cur = it._M_first + 5;                  // logical index 21
    It a = it + 3, b = it + 20, c = it + (-10), d = it + (-5), e = it + 11;
    if (it._M_cur != nodes[1] + 5 || it._M_node != &map[1]) return Fail();
    if (a._M_cur->X != 24.0f || a._M_node != &map[1]) return Fail();
    if (b._M_cur->X != 41.0f || b._M_node != &map[2] || b._M_first != nodes[2] || b._M_last != nodes[2] + 16) return Fail();
    if (c._M_cur->X != 11.0f || c._M_node != &map[0] || c._M_first != nodes[0]) return Fail();
    if (d._M_cur->X != 16.0f || d._M_node != &map[1] || e._M_cur->X != 32.0f || e._M_node != &map[2]) return Fail();
    std::printf("00a0e980_TEST PASS\n");
    return 0;
}
