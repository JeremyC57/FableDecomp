#include <cstdio>
#include <cstring>
// Behaviour model of the library CString copy constructor @ 0x00C0DAB0 (copy of the source logic, stand-in callees).
static int g_asserts = 0, g_breaks = 0;
static void AssertHook(const char*) { ++g_asserts; }
typedef void (*PFNASSERTHOOK)(const char*);
static PFNASSERTHOOK g_pfnAssertHook = AssertHook;
static long InterlockedIncrement(long* p) { return ++*p; }
struct CDebugBreak { CDebugBreak() { ++g_breaks; } };
#define LIB_ASSERT(cond) \
    if (!(cond)) { if (g_pfnAssertHook) g_pfnAssertHook(#cond "\n"); CDebugBreak(); }
struct CStringData { long nRefs; long nDataLength; long nAllocLength; };
struct SBlock { CStringData h; char text[32]; };
static SBlock g_nullBlock = { { -1, 0, 0 }, "" };
static CStringData* _NullString = &g_nullBlock.h;
static SBlock g_copyBlock = { { 1, 0, 32 }, "" };
class CString {
public:
    char* m_pchData;
    CString(char* p) : m_pchData(p) {}
    CString(const CString& stringSrc)
    {
        LIB_ASSERT(stringSrc.GetData()->nRefs != 0)
        if (stringSrc.GetData()->nRefs >= 0) {
            LIB_ASSERT(stringSrc.GetData() != _NullString)
            m_pchData = stringSrc.m_pchData;
            InterlockedIncrement(&GetData()->nRefs);
        }
        else {
            Init();
            *this = stringSrc.m_pchData;
        }
    }
    CStringData* GetData() const { LIB_ASSERT(m_pchData != NULL) return ((CStringData*)m_pchData) - 1; }
    void Init() { m_pchData = g_nullBlock.text; }
    const CString& operator=(const char* text)
    {
        std::strcpy(g_copyBlock.text, text); g_copyBlock.h.nDataLength = (long)std::strlen(text);
        m_pchData = g_copyBlock.text; return *this;
    }
};
int main()
{
    SBlock shared = { { 1, 5, 8 }, "hello" };
    CString a(shared.text);
    CString b(a);
    int ok = b.m_pchData == shared.text && shared.h.nRefs == 2 && g_asserts == 0 && g_breaks == 0;
    SBlock locked = { { -1, 4, 8 }, "lock" };
    CString c(locked.text);
    CString d(c);
    ok = ok && d.m_pchData == g_copyBlock.text && std::strcmp(d.m_pchData, "lock") == 0 && locked.h.nRefs == -1 && g_asserts == 0;
    SBlock dead = { { 0, 0, 0 }, "" };
    CString e(dead.text);
    CString f(e);                                  // nRefs == 0 trips the first assertion, then shares the buffer
    ok = ok && g_asserts == 1 && g_breaks == 1 && dead.h.nRefs == 1;
    if (!ok) { std::printf("00c0dab0_TEST FAIL\n"); return 1; }
    std::printf("00c0dab0_TEST PASS\n");
    return 0;
}