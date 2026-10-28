// Behaviour test for 0x004CFBF0 std::swap<pair<float,CCharString>>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("004cfbf0_TEST FAIL\n"); return 1; } } while (0)

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
    pair<float, CCharString> a(1.5f, CCharString("one"));
    pair<float, CCharString> b(-2.25f, CCharString("two"));
    g_copies = g_assigns = 0;
    swap_model(a, b);
    CHECK(a.first > -2.2501f && a.first < -2.2499f && a.second.Is("two"));
    CHECK(b.first > 1.4999f && b.first < 1.5001f && b.second.Is("one"));
    CHECK(g_copies == 1 && g_assigns == 2);
    std::printf("004cfbf0_TEST PASS\n");
    return 0;
}