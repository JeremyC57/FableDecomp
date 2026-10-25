// Behaviour test for 0x00526120 _STL::less<unsigned __int64>::operator(): x < y on 64-bit unsigned keys
#include <stdio.h>
#if !defined(_MSC_VER)
#define __int64 long long
#endif
namespace _STL {
template <class _Arg1, class _Arg2, class _Result>
struct binary_function {
    typedef _Arg1   first_argument_type;
    typedef _Arg2   second_argument_type;
    typedef _Result result_type;
};
template <class _Tp>
struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
}
int main()
{
    _STL::less<unsigned __int64> lt;
    unsigned __int64 one = 1;
    unsigned __int64 high = 1;
    high <<= 32;                         // 0x100000000: only the high dword is set
    unsigned __int64 top = 0;
    top = ~top;                          // all ones: largest unsigned value (would be -1 if signed)
    unsigned __int64 lowmax = 0xFFFFFFFFu;
    bool ok = true;
    ok = ok && lt(one, high) && !lt(high, one);
    ok = ok && lt(lowmax, high);         // high dword decides before the low dword
    ok = ok && lt(one, top) && !lt(top, one);
    ok = ok && !lt(one, one);
    if (ok) { printf("00526120_TEST PASS\n"); return 0; }
    printf("00526120_TEST FAIL\n");
    return 1;
}