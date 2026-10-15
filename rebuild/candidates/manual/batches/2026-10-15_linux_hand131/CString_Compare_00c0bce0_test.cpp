// Behaviour test for 0x00C0BCE0 CString::Compare: a null argument fires the assertion hook; the global flag selects a
// case-insensitive compare, otherwise a plain strcmp sign.
#include <cstdio>
#include <cstring>
static int g_asserts = 0;
static bool g_bCaseInsensitiveStrings = false;
static int Lower(int c) { return (c >= 'A' && c <= 'Z') ? c + 32 : c; }
static int StubStricmp(const char* a, const char* b)
{
    for (;; ++a, ++b) { int x = Lower((unsigned char)*a), y = Lower((unsigned char)*b); if (x != y) return x < y ? -1 : 1; if (!x) return 0; }
}
struct CString {
    const char* m_pchData;
    int Compare(const char* lpsz) const
    {
        if (!lpsz) { ++g_asserts; return 99; }    // the retail hook + DebugBreak; the test stops before strcmp(NULL)
        if (g_bCaseInsensitiveStrings) return StubStricmp(m_pchData, lpsz);
        int r = std::strcmp(m_pchData, lpsz);
        return r < 0 ? -1 : (r > 0 ? 1 : 0);
    }
};
int main()
{
    CString s; s.m_pchData = "Hello";
    if (s.Compare("Hello") != 0 || s.Compare("hello") != -1 || s.Compare("Abc") != 1) { std::printf("00c0bce0_TEST FAIL\n"); return 1; }
    g_bCaseInsensitiveStrings = true;
    if (s.Compare("hELLO") != 0 || s.Compare("help") >= 0) { std::printf("00c0bce0_TEST FAIL\n"); return 1; }
    s.Compare(0);
    if (g_asserts != 1) { std::printf("00c0bce0_TEST FAIL\n"); return 1; }
    std::printf("00c0bce0_TEST PASS\n");
    return 0;
}
