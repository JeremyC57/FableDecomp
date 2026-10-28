// Behaviour test for 0x00496FD0 std::__unguarded_linear_insert of a CWideString under NGameComponentHelpers::CCaseInsensitiveStringCompare.
#include <cstdio>

static int g_assigns = 0, g_destroyed = 0;
static int icmp(const wchar_t* a, const wchar_t* b)   // test stand-in for _wcsicmp (ASCII folding)
{
    for (;; ++a, ++b) {
        wchar_t x = *a, y = *b;
        if (x >= L'A' && x <= L'Z') x = (wchar_t)(x - L'A' + L'a');
        if (y >= L'A' && y <= L'Z') y = (wchar_t)(y - L'A' + L'a');
        if (x != y) return x < y ? -1 : 1;
        if (!x) return 0;
    }
}
class CWideString { public:
    const wchar_t* PStringData;
    CWideString(const wchar_t* s) : PStringData(s) { }
    CWideString(const CWideString& other) : PStringData(other.PStringData) { }
    ~CWideString() { ++g_destroyed; }
    const CWideString& operator=(const CWideString& other) { ++g_assigns; PStringData = other.PStringData; return *this; }
    operator const wchar_t*() const { return PStringData; }
};
struct CCaseInsensitiveStringCompare {
    bool operator()(const CWideString& a, const CWideString& b) const { return icmp(a, b) < 0; }
};
static void __unguarded_linear_insert(CWideString* last, CWideString val, CCaseInsensitiveStringCompare comp)
{
    CWideString* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

int main()
{
    CWideString a[5] = { CWideString(L"Alpha"), CWideString(L"bravo"), CWideString(L"CHARLIE"), CWideString(L"delta"), CWideString(L"Bridge") };
    CCaseInsensitiveStringCompare comp;
    g_assigns = 0; g_destroyed = 0;
    __unguarded_linear_insert(a + 4, a[4], comp);   // "Bridge" sinks below delta and CHARLIE, stops above bravo
    if (icmp(a[0], L"alpha") || icmp(a[1], L"bravo") || icmp(a[2], L"bridge") || icmp(a[3], L"charlie") || icmp(a[4], L"delta"))
        { std::printf("00496fd0_TEST FAIL\n"); return 1; }
    if (g_assigns != 3 || g_destroyed != 1) { std::printf("00496fd0_TEST FAIL\n"); return 1; }   // two shifts + the final store; the by-value copy dies
    g_assigns = 0;
    __unguarded_linear_insert(a + 4, a[4], comp);   // already in place: one store
    if (g_assigns != 1 || icmp(a[4], L"DELTA")) { std::printf("00496fd0_TEST FAIL\n"); return 1; }
    std::printf("00496fd0_TEST PASS\n");
    return 0;
}