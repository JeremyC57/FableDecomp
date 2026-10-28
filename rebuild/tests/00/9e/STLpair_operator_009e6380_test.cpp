// Behaviour test for 0x009E6380 pair<CCharString, CSymbolMap::CSymbolInfo>::operator= (compiler-generated).
#include <cstdio>
#include <cstring>

class CCharString {
public:
    const char* PData;
    static int s_assigns;
    CCharString(const char* s) : PData(s) {}
    CCharString(const CCharString& o) : PData(o.PData) {}
    const CCharString& operator=(const CCharString& o) { PData = o.PData; ++s_assigns; return *this; }
};
int CCharString::s_assigns = 0;

class CSymbolMap {
public:
    class CSymbolInfo {
    public:
        CCharString GroupName;
        CCharString EntryName;
        long        Val;
        CSymbolInfo(const char* g, const char* e, long v) : GroupName(g), EntryName(e), Val(v) {}
    };
};

namespace stlp {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
    pair(const pair& p) : first(p.first), second(p.second) {}
};
}

typedef stlp::pair<CCharString, CSymbolMap::CSymbolInfo> TSymbolPair;

int main()
{
    TSymbolPair a(CCharString("KeyA"), CSymbolMap::CSymbolInfo("GroupA", "EntryA", 1));
    TSymbolPair b(CCharString("KeyB"), CSymbolMap::CSymbolInfo("GroupB", "EntryB", 42));
    CCharString::s_assigns = 0;
    TSymbolPair* r = &(a = b);
    if (r != &a || CCharString::s_assigns != 3 ||
        std::strcmp(a.first.PData, "KeyB") != 0 || std::strcmp(a.second.GroupName.PData, "GroupB") != 0 ||
        std::strcmp(a.second.EntryName.PData, "EntryB") != 0 || a.second.Val != 42 ||
        std::strcmp(b.first.PData, "KeyB") != 0) {
        std::printf("009e6380_TEST FAIL\n");
        return 1;
    }
    std::printf("009e6380_TEST PASS\n");
    return 0;
}