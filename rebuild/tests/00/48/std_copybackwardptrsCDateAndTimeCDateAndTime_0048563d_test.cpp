// Behaviour test for std::__copy_backward_ptrs<CDateAndTime*,CDateAndTime*> (0x0048563D): forwards first, last and
// result unchanged to __copy_backward with a null distance pointer and returns its result.
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
struct random_access_iterator_tag { random_access_iterator_tag() { } };
static int g_calls = 0;
static CDateAndTime *g_first = 0, *g_last = 0, *g_result = 0;
static int* g_dist = (int*)1;
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&,
                                              _Distance* __d)
{
    ++g_calls; g_first = __first; g_last = __last; g_result = __result; g_dist = __d;
    for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last;
    return __result;
}
template CDateAndTime* __fastcall __copy_backward<CDateAndTime*, CDateAndTime*, int>(CDateAndTime*, CDateAndTime*,
    CDateAndTime*, const random_access_iterator_tag&, int*);
template <class _RandomAccessIter, class _BidirectionalIter>
_BidirectionalIter __fastcall __copy_backward_ptrs(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _BidirectionalIter __result, const __false_type&);
}

int main()
{
    CDateAndTime a[5];
    for (int i = 0; i < 5; ++i) a[i].Year = i;
    std::__false_type tag;
    CDateAndTime* r = std::__copy_backward_ptrs(a, a + 3, a + 5, tag);
    if (std::g_calls != 1 || std::g_first != a || std::g_last != a + 3 || std::g_result != a + 5 || std::g_dist != 0 ||
        r != a + 2 || a[4].Year != 2 || a[2].Year != 0) { std::printf("FAIL\n"); return 1; }
    std::printf("0048563d_TEST PASS\n");
    return 0;
}
