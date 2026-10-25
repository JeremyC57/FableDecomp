// Behaviour test for 0x00A0EAF0 _STL::uninitialized_fill over a deque<C2DVector> range: one copy of x is
// constructed in every slot from first up to last, stepping across the buffer boundary through the map
#include <stdio.h>
#include <stdlib.h>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __declspec(x)
#endif
static int gCopies = 0;
class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { ++gCopies; }   // test-only: count constructions
};
namespace _STL {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits {
    typedef __false_type has_trivial_destructor;
    typedef __false_type is_POD_type;
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> struct _Deque_iterator_base {
    typedef _Tp** _Map_pointer;
    _Tp*         _M_cur;
    _Tp*         _M_first;
    _Tp*         _M_last;
    _Map_pointer _M_node;
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
    _Deque_iterator(const iterator& __x) : _Deque_iterator_base<_Tp>(__x) { }
    _Tp& operator*() const { return *this->_M_cur; }
    _Deque_iterator& operator++() { this->_M_increment(); return *this; }
    bool operator!=(const _Deque_iterator& __y) const { return this->_M_cur != __y._M_cur; }
};
template <class _Tp, class _Traits> inline _Tp* value_type(const _Deque_iterator<_Tp, _Traits>&) { return (_Tp*)0; }
}
typedef _STL::_Deque_iterator<C2DVector, _STL::_Nonconst_traits<C2DVector> > CDequeIter;
namespace _STL {
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall __uninitialized_fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __cur != __last; ++__cur)
        _Construct(&*__cur, __x);
}
template <class _Tp> struct _IsPOD {
    typedef typename __type_traits<_Tp>::is_POD_type _Type;
    static _Type __fastcall _Answer() { return _Type(); }
};
template <class _Tp> inline _IsPOD<_Tp> __fastcall _Is_POD(_Tp*) { return _IsPOD<_Tp>(); }
template <class _ForwardIter, class _Tp>
void __fastcall uninitialized_fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __x)
{
    __uninitialized_fill(__first, __last, __x, _Is_POD(value_type(__first))._Answer());
}
}
static C2DVector* gMap[3];
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    unsigned int bs = CDequeIter::_S_buffer_size();
    C2DVector* b0 = (C2DVector*)malloc(bs * sizeof(C2DVector));
    C2DVector* b1 = (C2DVector*)malloc(bs * sizeof(C2DVector));
    unsigned int i;
    for (i = 0; i < bs; ++i) { b0[i].X = -1.0f; b0[i].Y = -1.0f; b1[i].X = -1.0f; b1[i].Y = -1.0f; }
    gMap[0] = b0; gMap[1] = b1; gMap[2] = 0;
    CDequeIter first, last;
    first._M_set_node(gMap);     first._M_cur = b0 + (bs - 2);
    last._M_set_node(gMap + 1);  last._M_cur = b1 + 3;
    C2DVector x; x.X = 1.5f; x.Y = -2.25f;
    _STL::uninitialized_fill(first, last, x);
    bool ok = bs == 16 && gCopies == 5;
    ok = ok && Near(b0[bs - 2].X, 1.5f) && Near(b0[bs - 1].Y, -2.25f);
    ok = ok && Near(b1[0].X, 1.5f) && Near(b1[1].X, 1.5f) && Near(b1[2].Y, -2.25f);
    ok = ok && Near(b0[bs - 3].X, -1.0f) && Near(b1[3].X, -1.0f);      // outside the range: untouched
    _STL::uninitialized_fill(last, last, x);
    ok = ok && gCopies == 5;
    free(b0); free(b1);
    if (ok) { printf("00a0eaf0_TEST PASS\n"); return 0; }
    printf("00a0eaf0_TEST FAIL\n");
    return 1;
}