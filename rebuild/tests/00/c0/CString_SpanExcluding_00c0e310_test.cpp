// Behaviour test for CString::SpanExcluding (0x00C0E310): ASSERT a non-null set (the assert hook fires and the break runs on
// null), then Left(_tcscspn(m_pchData, set)): the leading run of characters that are all outside the set.
#include <cstdio>
#include <cstring>
#include <string>

static int g_asserts = 0;
static void AssertHook(const char*) { ++g_asserts; }
typedef void (*PFNASSERTHOOK)(const char*);
static PFNASSERTHOOK g_pfnAssertHook = AssertHook;
struct CDebugBreak { CDebugBreak() { } };
#define LIB_ASSERT(cond) if (!(cond)) { if (g_pfnAssertHook) g_pfnAssertHook(#cond "\n"); CDebugBreak(); }
#define IsValidString(lpsz) ((lpsz) != NULL)
static size_t _mbscspn_stub(const unsigned char* s, const unsigned char* set) { return std::strcspn((const char*)s, (const char*)set); }
static size_t _tcscspn(const char* s, const char* set) { return _mbscspn_stub((const unsigned char*)s, (const unsigned char*)set); }

class CString {
public:
    std::string s;
    const char* m_pchData;
    CString(const char* t) : s(t) { m_pchData = s.c_str(); }
    CString(const CString& o) : s(o.s) { m_pchData = s.c_str(); }
    CString Left(int nCount) const { if (nCount < 0) nCount = 0; return CString(s.substr(0, (size_t)nCount).c_str()); }
    CString SpanExcluding(const char* lpszCharSet) const
    {
        LIB_ASSERT(IsValidString(lpszCharSet))
        return Left((int)_tcscspn(m_pchData, lpszCharSet));
    }
};

int main()
{
    CString a("aabbcxyz");
    bool ok = a.SpanExcluding("xyz").s == "aabbc" && a.SpanExcluding("c").s == "aabb";
    ok = ok && a.SpanExcluding("a").s.empty() && a.SpanExcluding("").s == "aabbcxyz" && g_asserts == 0;
    ok = ok && CString("").SpanExcluding("a").s.empty();
    if (!ok) { std::printf("00c0e310_TEST FAIL\n"); return 1; }
    std::printf("00c0e310_TEST PASS\n");
    return 0;
}