// Behaviour test for 0x00A3FDC0 std::swap<CSymbolMap::CSymbolInfo>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00a3fdc0_TEST FAIL\n"); return 1; } } while (0)

static int g_copies = 0, g_assigns = 0, g_dtors = 0;
class CCharString {                                     // test model: a short owned text
public:
    char Text[16];
    CCharString() { Text[0] = 0; }
    CCharString(const char* s) { std::strncpy(Text, s, 15); Text[15] = 0; }
    CCharString(const CCharString& o) { std::memcpy(Text, o.Text, 16); ++g_copies; }
    ~CCharString() { ++g_dtors; }
    const CCharString& operator=(const CCharString& o) { std::memcpy(Text, o.Text, 16); ++g_assigns; return *this; }
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

template <class _Tp> inline void swap_model(_Tp& __a, _Tp& __b)    // STLport swap
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

int main()
{
    CSymbolMap::CSymbolInfo a("GROUP1", "ENTRY1", 5);
    CSymbolMap::CSymbolInfo b("GROUP2", "ENTRY2", 6);
    g_copies = g_assigns = g_dtors = 0;
    swap_model(a, b);
    CHECK(a.GroupName.Is("GROUP2") && a.EntryName.Is("ENTRY2") && a.Val == 6);
    CHECK(b.GroupName.Is("GROUP1") && b.EntryName.Is("ENTRY1") && b.Val == 5);
    CHECK(g_copies == 2 && g_assigns == 4 && g_dtors == 2);
    std::printf("00a3fdc0_TEST PASS\n");
    return 0;
}