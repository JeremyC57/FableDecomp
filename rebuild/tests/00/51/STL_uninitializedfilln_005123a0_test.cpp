// behaviour test for 0x005123A0 _STL::uninitialized_fill_n<unsigned short*, unsigned int, unsigned short>: fill n slots, return first + n
#include <cstdio>

namespace _STL {
struct __true_type { __true_type() { } };
template <class _Tp> struct __type_traits { typedef __true_type is_POD_type; };

template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for ( ; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n_aux(_ForwardIter __first, _Size __n, const _Tp& __x, const __true_type&)
{
    return fill_n(__first, __n, __x);
}
template <class _ForwardIter, class _Size, class _Tp, class _Tp1>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, _Tp1*)
{
    typedef typename __type_traits<_Tp1>::is_POD_type _Is_POD;
    return __uninitialized_fill_n_aux(__first, __n, __x, _Is_POD());
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, (_Tp*)0);
}
}

int main()
{
    unsigned short buffer[8];
    for (int i = 0; i < 8; ++i) buffer[i] = 0xEEEE;
    unsigned short value = 0x1234;
    unsigned short* end = _STL::uninitialized_fill_n(buffer + 1, 5u, value);
    if (end != buffer + 6) { std::printf("005123a0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i) {
        unsigned short expect = (i >= 1 && i <= 5) ? 0x1234 : 0xEEEE;
        if (buffer[i] != expect) { std::printf("005123a0_TEST FAIL\n"); return 1; }
    }
    if (_STL::uninitialized_fill_n(buffer, 0u, value) != buffer || buffer[0] != 0xEEEE) { std::printf("005123a0_TEST FAIL\n"); return 1; }
    // the value is read through its reference on every pass (it may live in the range)
    buffer[0] = 7;
    _STL::uninitialized_fill_n(buffer, 3u, buffer[0]);
    if (buffer[0] != 7 || buffer[1] != 7 || buffer[2] != 7 || buffer[3] != 0x1234) { std::printf("005123a0_TEST FAIL\n"); return 1; }
    std::printf("005123a0_TEST PASS\n");
    return 0;
}