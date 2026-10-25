// Behaviour test for std::uninitialized_fill_n<CDateAndTime*,unsigned int,CDateAndTime> (0x00485239): forwards first,
// n and x unchanged to __uninitialized_fill_n and returns its result.
#include <cstdio>

class CDateAndTime {
public:
    long          Year;
    unsigned long Month, Day, Hour, Minute, Second, Millisecond;
    CDateAndTime();
};
CDateAndTime::CDateAndTime() : Year(0), Month(0), Day(0), Hour(0), Minute(0), Second(0), Millisecond(0) { }

namespace std {
struct __false_type { __false_type() { } };
static int g_calls = 0;
static unsigned int g_n = 0;
static const CDateAndTime* g_x = 0;
static CDateAndTime* g_first = 0;
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    ++g_calls; g_first = __first; g_n = __n; g_x = &__x;
    for (; __n > 0; --__n, ++__first) *__first = __x;
    return __first;
}
template CDateAndTime* __fastcall __uninitialized_fill_n<CDateAndTime*, unsigned int, CDateAndTime>(CDateAndTime*,
    unsigned int, const CDateAndTime&, const __false_type&);
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);
}

int main()
{
    CDateAndTime x, buf[3];
    x.Year = 2004; x.Second = 59;
    CDateAndTime* end = std::uninitialized_fill_n(buf, 2u, x);
    if (std::g_calls != 1 || std::g_first != buf || std::g_n != 2 || std::g_x != &x || end != buf + 2) {
        std::printf("FAIL forward\n"); return 1;
    }
    if (buf[0].Year != 2004 || buf[1].Second != 59 || buf[2].Year != 0) { std::printf("FAIL data\n"); return 1; }
    std::printf("00485239_TEST PASS\n");
    return 0;
}
