// Behaviour test for 0x00566870 std::swap<pair<float,CWideString>>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00566870_TEST FAIL\n"); return 1; } } while (0)

static int g_copies = 0, g_assigns = 0, g_dtors = 0;
class CWideString {                                     // test model: a short owned text
public:
    char Text[16];
    CWideString() { Text[0] = 0; }
    CWideString(const char* s) { std::strncpy(Text, s, 15); Text[15] = 0; }
    CWideString(const CWideString& o) { std::memcpy(Text, o.Text, 16); ++g_copies; }
    ~CWideString() { ++g_dtors; }
    const CWideString& operator=(const CWideString& o) { std::memcpy(Text, o.Text, 16); ++g_assigns; return *this; }
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
    pair<float, CWideString> a(0.75f, CWideString("left"));
    pair<float, CWideString> b(8.0f, CWideString("right"));
    g_copies = g_assigns = 0;
    swap_model(a, b);
    CHECK(a.first > 7.999f && a.first < 8.001f && a.second.Is("right"));
    CHECK(b.first > 0.749f && b.first < 0.751f && b.second.Is("left"));
    CHECK(g_copies == 1 && g_assigns == 2);
    std::printf("00566870_TEST PASS\n");
    return 0;
}