// Behaviour test for 0x00527C10 std::fill(CSharerParserOutputToDebugContextMapper::CEntry*, ...): every entry of the range becomes a copy of the value (line numbers copied, the context string through its operator=)
#include <stdio.h>
#if !defined(_MSC_VER)
#define __fastcall
#define __cdecl
#define __int64 long long
#endif
static int gAssigns = 0;
class CCharString {
public:
    const char* PStringData;
    const CCharString& operator=(const CCharString& other) { PStringData = other.PStringData; ++gAssigns; return *this; }   // stand-in
};
class CSharerParserOutputToDebugContextMapper {
public:
    struct CEntry {
    public:
        long        SrcLine;
        long        DestLine;
        CCharString DebugContext;
    };
};
typedef CSharerParserOutputToDebugContextMapper::CEntry CEntry;
static CEntry gArr[4];
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
    CEntry v; v.SrcLine = 12; v.DestLine = 34; v.DebugContext.PStringData = "ctx";
    int i;
    for (i = 0; i < 4; ++i) { gArr[i].SrcLine = -1; gArr[i].DestLine = -1; gArr[i].DebugContext.PStringData = "old"; }
    bool ok = true;
    std::fill(gArr, gArr + 3, v);
    for (i = 0; i < 3; ++i)
        ok = ok && gArr[i].SrcLine == 12 && gArr[i].DestLine == 34 && gArr[i].DebugContext.PStringData == v.DebugContext.PStringData;
    ok = ok && gArr[3].SrcLine == -1 && gAssigns == 3;     // [first, last) only
    std::fill(gArr + 3, gArr + 3, v);
    ok = ok && gAssigns == 3 && gArr[3].SrcLine == -1;      // empty range
    if (ok) { printf("00527c10_TEST PASS\n"); return 0; }
    printf("00527c10_TEST FAIL\n");
    return 1;
}