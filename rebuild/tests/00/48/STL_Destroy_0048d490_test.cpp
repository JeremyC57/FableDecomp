// Behaviour test for 0x0048D490 std::_Destroy over a deque<C2DVector> range: every element from first up to last
// is destroyed in order, stepping across the buffer boundary through the map
#include <stdio.h>
#include <stdlib.h>
#if !defined(_MSC_VER)
#define __fastcall
#define __declspec(x)
#endif
static int gDtors = 0;
static float gSeen[64];
class C2DVector {
public:
    float X, Y;
    ~C2DVector() { if (gDtors < 64) gSeen[gDtors] = X; ++gDtors; }   // test-only: observe each destruction
};
namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits {
    typedef __false_type has_trivial_destructor;
    typedef __false_type is_POD_type;
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
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
typedef std::_Deque_iterator<C2DVector, std::_Nonconst_traits<C2DVector> > CDequeIter;
namespace std {
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
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
static C2DVector* gMap[3];
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    unsigned int bs = CDequeIter::_S_buffer_size();
    C2DVector* b0 = (C2DVector*)malloc(bs * sizeof(C2DVector));
    C2DVector* b1 = (C2DVector*)malloc(bs * sizeof(C2DVector));
    unsigned int i;
    for (i = 0; i < bs; ++i) { b0[i].X = (float)i; b0[i].Y = 0.0f; b1[i].X = (float)(100 + i); b1[i].Y = 0.0f; }
    gMap[0] = b0; gMap[1] = b1; gMap[2] = 0;
    CDequeIter first, last;
    first._M_set_node(gMap);     first._M_cur = b0 + (bs - 2);     // the last two of the front buffer
    last._M_set_node(gMap + 1);  last._M_cur = b1 + 3;             // ... and the first three of the next
    std::_Destroy(first, last);
    bool ok = bs == 16 && gDtors == 5;
    ok = ok && Near(gSeen[0], (float)(bs - 2)) && Near(gSeen[1], (float)(bs - 1));
    ok = ok && Near(gSeen[2], 100.0f) && Near(gSeen[3], 101.0f) && Near(gSeen[4], 102.0f);
    std::_Destroy(last, last);                                      // empty range
    ok = ok && gDtors == 5;
    free(b0); free(b1);
    if (ok) { printf("0048d490_TEST PASS\n"); return 0; }
    printf("0048d490_TEST FAIL\n");
    return 1;
}