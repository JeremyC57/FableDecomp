// Behaviour test for std::__uninitialized_fill_n<CDateAndTime*,unsigned int,CDateAndTime> (0x0040DF00): n copies of x
// land at first, the result is first + n, and n == 0 writes nothing.
#include <cstdio>

class CDateAndTime {
public:
    long          Year;
    unsigned long Month, Day, Hour, Minute, Second, Millisecond;
    CDateAndTime();
};
CDateAndTime::CDateAndTime() : Year(0), Month(0), Day(0), Hour(0), Minute(0), Second(0), Millisecond(0) { }

namespace std {
struct __false_type {};
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&);
}

int main()
{
    CDateAndTime x, buf[4];
    x.Year = 2005; x.Month = 9; x.Day = 13; x.Hour = 1; x.Minute = 2; x.Second = 3; x.Millisecond = 4;
    buf[3].Year = -1;
    std::__false_type tag;
    CDateAndTime* end = std::__uninitialized_fill_n(buf, 3u, x, tag);
    if (end != buf + 3) { std::printf("FAIL end\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (buf[i].Year != 2005 || buf[i].Day != 13 || buf[i].Millisecond != 4) { std::printf("FAIL data %d\n", i); return 1; }
    if (buf[3].Year != -1) { std::printf("FAIL overrun\n"); return 1; }
    if (std::__uninitialized_fill_n(buf + 1, 0u, x, tag) != buf + 1) { std::printf("FAIL zero\n"); return 1; }
    std::printf("0040df00_TEST PASS\n");
    return 0;
}
