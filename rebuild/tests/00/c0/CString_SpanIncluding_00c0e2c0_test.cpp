// Behaviour test for CString::SpanIncluding (0x00C0E2C0): ASSERT a non-null set (the assert hook fires and the break runs on
// null), then Left(_tcsspn(m_pchData, set)): the leading run of characters that are all in the set.
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
static size_t _mbsspn_stub(const unsigned char* s, const unsigned char* set) { return std::strspn((const char*)s, (const char*)set); }
static size_t _tcsspn(const char* s, const char* set) { return _mbsspn_stub((const unsigned char*)s, (const unsigned char*)set); }

class CString {
public:
    std::string s;
    const char* m_pchData;
    CString(const char* t) : s(t) { m_pchData = s.c_str(); }
    CString(const CString& o) : s(o.s) { m_pchData = s.c_str(); }
    CString Left(int nCount) const { if (nCount < 0) nCount = 0; return CString(s.substr(0, (size_t)nCount).c_str()); }
    CString SpanIncluding(const char* lpszCharSet) const
    {
        LIB_ASSERT(IsValidString(lpszCharSet))
        return Left((int)_tcsspn(m_pchData, lpszCharSet));
    }
};

int main()
{
    CString a("aabbcxyz");
    bool ok = a.SpanIncluding("ab").s == "aabb" && a.SpanIncluding("abc").s == "aabbc";
    ok = ok && a.SpanIncluding("xyz").s.empty() && a.SpanIncluding("").s.empty() && g_asserts == 0;
    ok = ok && CString("").SpanIncluding("a").s.empty();
    if (!ok) { std::printf("00c0e2c0_TEST FAIL\n"); return 1; }
    std::printf("00c0e2c0_TEST PASS\n");
    return 0;
}