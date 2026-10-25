// Behaviour test for 0x00AD11A0 _STL::less<pair<long,long> >::operator(): lexicographic order on (first, second)
#include <stdio.h>
namespace _STL {
template <class _Arg1, class _Arg2, class _Result>
struct binary_function {
    typedef _Arg1   first_argument_type;
    typedef _Arg2   second_argument_type;
    typedef _Result result_type;
};
template <class _T1, class _T2>
struct pair {
    _T1 first;
    _T2 second;
};
template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first ||
           (!(__y.first < __x.first) && __x.second < __y.second);
}
template <class _Tp>
struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
}
typedef _STL::pair<long, long> LL;
static LL mk(long a, long b) { LL p; p.first = a; p.second = b; return p; }
int main()
{
    _STL::less<LL> lt;
    bool ok = true;
    ok = ok && lt(mk(1, 9), mk(2, 0)) && !lt(mk(2, 0), mk(1, 9));   // first decides
    ok = ok && lt(mk(3, 1), mk(3, 2)) && !lt(mk(3, 2), mk(3, 1));   // tie on first: second decides
    ok = ok && !lt(mk(4, 4), mk(4, 4));                             // irreflexive
    ok = ok && lt(mk(-5, 0), mk(-4, -100));                         // signed compare
    if (ok) { printf("00ad11a0_TEST PASS\n"); return 0; }
    printf("00ad11a0_TEST FAIL\n");
    return 1;
}