// Behaviour test for pair<CCharString, CSymbolMap::CSymbolInfo>::pair(const pair&) (0x009E6050).
#include <cstdio>
#include <cstring>

static int g_copies = 0;
class CCharString {
public:
    char Text[16];
    CCharString(const char* s) { std::strcpy(Text, s); }
    CCharString(const CCharString& other) { std::strcpy(Text, other.Text); ++g_copies; }
};

class CSymbolInfo {
public:
    CCharString GroupName;
    CCharString EntryName;
    long        Val;
    CSymbolInfo(const char* g, const char* e, long v) : GroupName(g), EntryName(e), Val(v) {}
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
    pair(const pair& p) : first(p.first), second(p.second) {}
};

int main()
{
    pair<CCharString, CSymbolInfo> a(CCharString("KEY"), CSymbolInfo("GROUP", "ENTRY", -9));
    g_copies = 0;
    pair<CCharString, CSymbolInfo> b(a);
    if (g_copies != 3 || std::strcmp(b.first.Text, "KEY") != 0 || std::strcmp(b.second.GroupName.Text, "GROUP") != 0 ||
        std::strcmp(b.second.EntryName.Text, "ENTRY") != 0 || b.second.Val != -9) {
        std::printf("009e6050_TEST FAIL\n"); return 1;
    }
    std::printf("009e6050_TEST PASS\n");
    return 0;
}