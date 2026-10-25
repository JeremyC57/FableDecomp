// Behaviour test for std::swap<pair<unsigned __int64, bool> > (0x00A7D520): key and flag exchanged.
#include <cstdio>

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(), second() {}
    pair(const pair& __o) : first(__o.first), second(__o.second) {}
};
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
}

int main()
{
    std::pair<u64, bool> a, b;
    const u64 k1 = ((u64)0x1 << 32) | 0x23456789;
    const u64 k2 = ((u64)0xFFFFFFFF << 32) | 0x00000001;
    a.first = k1; a.second = true;
    b.first = k2; b.second = false;
    std::swap(a, b);
    if (a.first != k2 || a.second != false || b.first != k1 || b.second != true)
        { std::printf("00a7d520_TEST FAIL\n"); return 1; }
    std::printf("00a7d520_TEST PASS\n");
    return 0;
}