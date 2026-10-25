// Behaviour test for std::__copy_backward<CDateAndTime*,CDateAndTime*,int> (0x00485372): copies [first, last) so that it
// ends at result (last element first, so overlapping right shifts work) and returns result - (last - first).
#include <cstdio>

class CDateAndTime {
public:
    long          Year;
    unsigned long Month, Day, Hour, Minute, Second, Millisecond;
    CDateAndTime();
};
CDateAndTime::CDateAndTime() : Year(0), Month(0), Day(0), Hour(0), Minute(0), Second(0), Millisecond(0) { }

namespace std {
struct random_access_iterator_tag {};
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&,
                                              _Distance*);
}

int main()
{
    CDateAndTime a[6];
    for (int i = 0; i < 6; ++i) { a[i].Year = 100 + i; a[i].Millisecond = 900 + i; }
    std::random_access_iterator_tag tag;
    // overlapping shift right by 2: a[0..3] -> a[2..5]
    CDateAndTime* r = std::__copy_backward(a, a + 4, a + 6, tag, (int*)0);
    if (r != a + 2) { std::printf("FAIL ret\n"); return 1; }
    if (a[0].Year != 100 || a[1].Year != 101 || a[2].Year != 100 || a[3].Year != 101 || a[4].Year != 102 ||
        a[5].Year != 103 || a[5].Millisecond != 903) { std::printf("FAIL data\n"); return 1; }
    if (std::__copy_backward(a, a, a + 3, tag, (int*)0) != a + 3) { std::printf("FAIL empty\n"); return 1; }
    std::printf("00485372_TEST PASS\n");
    return 0;
}
