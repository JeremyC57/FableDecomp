// Behaviour test for std::__uninitialized_copy<CDateAndTime*,CDateAndTime*> (0x0040DED0): copies every record of
// [first, last) to result and returns result + (last - first); an empty range copies nothing.
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
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&);
}

int main()
{
    CDateAndTime src[3], dst[4];
    for (int i = 0; i < 3; ++i) {
        src[i].Year = 2004 + i; src[i].Month = i + 1; src[i].Day = 10 + i; src[i].Hour = i;
        src[i].Minute = 30 + i; src[i].Second = 40 + i; src[i].Millisecond = 500 + i;
    }
    dst[3].Year = -7;
    std::__false_type tag;
    CDateAndTime* end = std::__uninitialized_copy(src, src + 3, dst, tag);
    if (end != dst + 3) { std::printf("FAIL end\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (dst[i].Year != 2004 + i || dst[i].Month != (unsigned long)i + 1 || dst[i].Millisecond != (unsigned long)500 + i) {
            std::printf("FAIL data %d\n", i); return 1;
        }
    if (dst[3].Year != -7) { std::printf("FAIL overrun\n"); return 1; }
    if (std::__uninitialized_copy(src, src, dst + 1, tag) != dst + 1) { std::printf("FAIL empty\n"); return 1; }
    std::printf("0040ded0_TEST PASS\n");
    return 0;
}
