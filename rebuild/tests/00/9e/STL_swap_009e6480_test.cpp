// Behaviour test for 0x009E6480 std::swap<pair<CCharString,CSymbolMap::CSymbolInfo>>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("009e6480_TEST FAIL\n"); return 1; } } while (0)

static int g_copies = 0, g_assigns = 0, g_dtors = 0;
class CCharString {                                     // test model: a short owned text
public:
    char Text[16];
    CCharString() { Text[0] = 0; }
    CCharString(const char* s) { std::strncpy(Text, s, 15); Text[15] = 0; }
    CCharString(const CCharString& o) { std::memcpy(Text, o.Text, 16); ++g_copies; }
    ~CCharString() { ++g_dtors; }
    CCharString& operator=(const CCharString& o) { std::memcpy(Text, o.Text, 16); ++g_assigns; return *this; }
    bool Is(const char* s) const { return std::strcmp(Text, s) == 0; }
};

class CSymbolMap {
public:
    class CSymbolInfo {
    public:
        CCharString GroupName;
        CCharString EntryName;
        long        Val;
        CSymbolInfo(const char* g, const char* e, long v) : GroupName(g), EntryName(e), Val(v) { }
    };
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _Tp> inline void swap_model(_Tp& __a, _Tp& __b)    // STLport swap
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

int main()
{
    pair<CCharString, CSymbolMap::CSymbolInfo> a(CCharString("KEY_A"), CSymbolMap::CSymbolInfo("GROUP1", "ENTRY1", 11));
    pair<CCharString, CSymbolMap::CSymbolInfo> b(CCharString("KEY_B"), CSymbolMap::CSymbolInfo("GROUP2", "ENTRY2", -4));
    g_copies = g_assigns = 0;
    swap_model(a, b);
    CHECK(a.first.Is("KEY_B") && a.second.GroupName.Is("GROUP2") && a.second.EntryName.Is("ENTRY2") && a.second.Val == -4);
    CHECK(b.first.Is("KEY_A") && b.second.GroupName.Is("GROUP1") && b.second.EntryName.Is("ENTRY1") && b.second.Val == 11);
    CHECK(g_copies == 3 && g_assigns == 6);
    std::printf("009e6480_TEST PASS\n");
    return 0;
}