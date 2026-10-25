// Behaviour test for 0x006FBBD0 std::fill(std::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> >*, ...): every (id, event list) pair of the range becomes a copy of the value; the small vector skips the copy when assigned to itself
#include <stdio.h>
#if !defined(_MSC_VER)
#define __fastcall
#define __cdecl
#define __int64 long long
#endif
static int gAssigns = 0;
namespace NAnimationEvents {
class CEvent { public: int Id; };
}
template <class T, int N> class CSmallVector {
public:
    T*  Start;
    int Size;
    CSmallVector& operator=(const CSmallVector& other)
    {
        if (&other != this)
            Copy(other);
        return *this;
    }
    void Copy(const CSmallVector& other) { Start = other.Start; Size = other.Size; ++gAssigns; }   // stand-in
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
typedef std::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> > CAnimEventsPair;
static NAnimationEvents::CEvent gEvents[3];
static CAnimEventsPair gArr[4];
namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}
int main()
{
    int i;
    for (i = 0; i < 4; ++i) { gArr[i].first = 0; gArr[i].second.Start = 0; gArr[i].second.Size = 0; }
    gArr[1].first = 77; gArr[1].second.Start = gEvents; gArr[1].second.Size = 3;
    bool ok = true;
    std::fill(gArr, gArr + 3, gArr[1]);             // the value lives inside the range
    for (i = 0; i < 3; ++i)
        ok = ok && gArr[i].first == 77 && gArr[i].second.Start == gEvents && gArr[i].second.Size == 3;
    ok = ok && gArr[3].first == 0 && gAssigns == 2;   // element 1 is the value itself: no Copy
    CAnimEventsPair v; v.first = 5; v.second.Start = 0; v.second.Size = 0;
    std::fill(gArr + 3, gArr + 4, v);
    ok = ok && gArr[3].first == 5 && gAssigns == 3;
    if (ok) { printf("006fbbd0_TEST PASS\n"); return 0; }
    printf("006fbbd0_TEST FAIL\n");
    return 1;
}