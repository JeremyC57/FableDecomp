// Behaviour test for 0x00623110 std::fill(std::pair<float, CWideString>*, ...): every (float, wide string) pair of the range becomes a copy of the value (the float copied, the string through its operator=)
#include <stdio.h>
#if !defined(_MSC_VER)
#define __fastcall
#define __cdecl
#define __int64 long long
#endif
static int gAssigns = 0;
class CWideString {
public:
    const char* PStringData;
    CWideString& operator=(const CWideString& other) { PStringData = other.PStringData; ++gAssigns; return *this; }   // stand-in
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
typedef std::pair<float, CWideString> CFloatWideStringPair;
static CFloatWideStringPair gArr[4];
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
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
    CFloatWideStringPair v; v.first = 2.5f; v.second.PStringData = "label";
    int i;
    for (i = 0; i < 4; ++i) { gArr[i].first = -1.0f; gArr[i].second.PStringData = 0; }
    bool ok = true;
    std::fill(gArr, gArr + 3, v);
    for (i = 0; i < 3; ++i)
        ok = ok && Near(gArr[i].first, 2.5f) && gArr[i].second.PStringData == v.second.PStringData;
    ok = ok && Near(gArr[3].first, -1.0f) && gArr[3].second.PStringData == 0 && gAssigns == 3;
    std::fill(gArr, gArr, v);
    ok = ok && gAssigns == 3;
    if (ok) { printf("00623110_TEST PASS\n"); return 0; }
    printf("00623110_TEST FAIL\n");
    return 1;
}