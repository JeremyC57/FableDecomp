// Behaviour test for 0x004DA6FC _STL::swap<pair<CCharString,CTCBase::CRegisteredTCInfo>>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("004da6fc_TEST FAIL\n"); return 1; } } while (0)

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

static int AllocTC() { return 8; }
class CTCBase {
public:
    class CRegisteredTCInfo {
    public:
        int (*AllocFunc)();
        unsigned long DriverType;
        CCharString   Name;
        CRegisteredTCInfo(int (*f)(), unsigned long d, const char* n) : AllocFunc(f), DriverType(d), Name(n) { }
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
    pair<CCharString, CTCBase::CRegisteredTCInfo> a(CCharString("TC_A"), CTCBase::CRegisteredTCInfo(AllocTC, 0x47, "Carryable"));
    pair<CCharString, CTCBase::CRegisteredTCInfo> b(CCharString("TC_B"), CTCBase::CRegisteredTCInfo(0, 0x48, "Weapon"));
    g_copies = g_assigns = 0;
    swap_model(a, b);
    CHECK(a.first.Is("TC_B") && a.second.AllocFunc == 0 && a.second.DriverType == 0x48 && a.second.Name.Is("Weapon"));
    CHECK(b.first.Is("TC_A") && b.second.AllocFunc == AllocTC && b.second.DriverType == 0x47 && b.second.Name.Is("Carryable"));
    CHECK(g_copies == 2 && g_assigns == 4);
    std::printf("004da6fc_TEST PASS\n");
    return 0;
}