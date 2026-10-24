// Behaviour test for 0x006727F0 std::uninitialized_fill_n<ENavigatorType*, unsigned int, ENavigatorType>: n copies of x
// are constructed from first on, the slots after them are untouched, and the result is first + n.
#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif

enum ENavigatorType { NAVIGATOR_TYPE_NONE = -1, NAVIGATOR_TYPE_0 = 0 };

namespace nav {
struct __false_type { };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
}

int main()
{
    ENavigatorType buf[6] = { (ENavigatorType)1, (ENavigatorType)1, (ENavigatorType)1, (ENavigatorType)1,
                              (ENavigatorType)1, (ENavigatorType)1 };
    ENavigatorType x = (ENavigatorType)-1;
    ENavigatorType* end = nav::uninitialized_fill_n(buf + 1, 3u, x);
    if (end != buf + 4 || buf[0] != 1 || buf[1] != -1 || buf[2] != -1 || buf[3] != -1 || buf[4] != 1) {
        std::printf("006727f0_TEST FAIL\n"); return 1;
    }
    ENavigatorType* same = nav::uninitialized_fill_n(buf, 0u, x);
    if (same != buf || buf[0] != 1) { std::printf("006727f0_TEST FAIL\n"); return 1; }
    std::printf("006727f0_TEST PASS\n");
    return 0;
}