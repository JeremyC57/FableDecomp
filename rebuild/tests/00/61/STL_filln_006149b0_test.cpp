// Behaviour test for 006149b0 (std::fill_n<unsigned __int64*, unsigned int, unsigned __int64>): n slots get the value, the
// returned iterator is first + n, n == 0 writes nothing, the neighbours are untouched.
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
template <class _OutputIter, class _Size, class _Tp>
_OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __value;
    return __first;
}
#define FAIL() { std::printf("006149b0_TEST FAIL\n"); return 1; }
int main()
{
    u64 buf[8];
    for (int i = 0; i < 8; ++i) buf[i] = (u64)i;
    const u64 v = ((u64)0xDEADBEEF << 32) | 0x01234567u;
    if (fill_n(buf + 2, 0u, v) != buf + 2 || buf[2] != 2) FAIL();
    u64* r = fill_n(buf + 2, 5u, v);
    if (r != buf + 7) FAIL();
    for (int i = 2; i < 7; ++i)
        if (buf[i] != v) FAIL();
    if (buf[1] != 1 || buf[7] != 7) FAIL();
    std::printf("006149b0_TEST PASS\n");
    return 0;
}