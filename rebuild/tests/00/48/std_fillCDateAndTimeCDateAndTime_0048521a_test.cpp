// Behaviour test for std::fill<CDateAndTime*,CDateAndTime> (0x0048521A): every record of [first, last) becomes value;
// nothing outside the range changes; an empty range does nothing.
#include <cstdio>

class CDateAndTime {
public:
    long          Year;
    unsigned long Month, Day, Hour, Minute, Second, Millisecond;
    CDateAndTime();
};
CDateAndTime::CDateAndTime() : Year(0), Month(0), Day(0), Hour(0), Minute(0), Second(0), Millisecond(0) { }

namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val);
}

int main()
{
    CDateAndTime a[5], v;
    v.Year = 2005; v.Month = 10; v.Millisecond = 7;
    a[0].Year = -1; a[4].Year = -4;
    std::fill(a + 1, a + 4, v);
    for (int i = 1; i < 4; ++i)
        if (a[i].Year != 2005 || a[i].Month != 10 || a[i].Millisecond != 7) { std::printf("FAIL data %d\n", i); return 1; }
    if (a[0].Year != -1 || a[4].Year != -4) { std::printf("FAIL bounds\n"); return 1; }
    std::fill(a, a, v);
    if (a[0].Year != -1) { std::printf("FAIL empty\n"); return 1; }
    std::printf("0048521a_TEST PASS\n");
    return 0;
}
