// behaviour test for 0x00C0D740 CString::TrimRight(const char*): cut the trailing run of characters found in a target list
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <stddef.h>

// stand-ins for the library's assertion hook (0x0143BB2C) and CDebugBreak (0x00C16750): count instead of breaking
static int g_asserts = 0, g_breaks = 0;
static void CountAssert(const char*) { ++g_asserts; }
typedef void (*PFNASSERTHOOK)(const char* message);
static PFNASSERTHOOK g_pfnAssertHook = CountAssert;
struct CDebugBreak { CDebugBreak() { ++g_breaks; } };

#define LIB_ASSERT(cond) \
    if (!(cond)) { if (g_pfnAssertHook) g_pfnAssertHook(#cond "\n"); CDebugBreak(); }

struct CStringData {
    long nRefs;
    long nDataLength;
    long nAllocLength;
    char* data() { return (char*)(this + 1); }
};

// the shared empty string (_afxDataNil / _afxPchNil @ 0x0139C8FC / 0x0139C900)
static struct { CStringData hdr; char text[4]; } g_nil = { { -1, 0, 0 }, { 0 } };
static CStringData* _afxDataNil = &g_nil.hdr;
static char* _afxPchNil = g_nil.text;

static long TestInterlockedDecrement(long* p) { return --*p; }   // InterlockedDecrement stand-in

static int g_frees = 0;
static void FreeDataStub(CStringData* pData) { ++g_frees; std::free(pData); }   // FreeData 0x00C0B900 stand-in

static CStringData* MakeData(const char* text, long refs)
{
    long n = (long)std::strlen(text);
    CStringData* d = (CStringData*)std::malloc(sizeof(CStringData) + n + 1);
    d->nRefs = refs; d->nDataLength = n; d->nAllocLength = n;
    std::memcpy(d->data(), text, n + 1);
    return d;
}

static char* _tcschr(const char* s, unsigned int c) { return (char*)std::strchr(s, (int)c); }   // _mbschr stand-in
static char* _tcsinc(const char* s) { return (char*)(s + 1); }                                  // _mbsinc stand-in

static int g_copies = 0;

class CString {
public:
    char* m_pchData;
    CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    void CopyBeforeWrite() { ++g_copies; }          // stand-in for 0x00C0BE20 (the test strings are unique)
    void TrimRight(const char* lpszTargetList);
};

void CString::TrimRight(const char* lpszTargetList)
{
    CopyBeforeWrite();
    char* lpsz = m_pchData;
    char* lpszLast = NULL;
    while (*lpsz != '\0') {
        if (_tcschr(lpszTargetList, *lpsz) != NULL) {
            if (lpszLast == NULL)
                lpszLast = lpsz;
        } else {
            lpszLast = NULL;
        }
        lpsz = _tcsinc(lpsz);
    }
    if (lpszLast != NULL) {
        *lpszLast = '\0';
        GetData()->nDataLength = (long)(lpszLast - m_pchData);
    }
}

int main()
{
    CStringData* d = MakeData("a b.c . ", 1);
    CString s; s.m_pchData = d->data();
    s.TrimRight(" .");
    if (std::strcmp(s.m_pchData, "a b.c") != 0 || s.GetData()->nDataLength != 5) { std::printf("00c0d740_TEST FAIL\n"); return 1; }
    s.TrimRight("xyz");                                           // nothing to cut
    if (std::strcmp(s.m_pchData, "a b.c") != 0 || s.GetData()->nDataLength != 5 || g_copies != 2) { std::printf("00c0d740_TEST FAIL\n"); return 1; }
    s.TrimRight("abc. ");                                         // everything goes
    if (s.m_pchData[0] != 0 || s.GetData()->nDataLength != 0) { std::printf("00c0d740_TEST FAIL\n"); return 1; }
    std::free(d);
    std::printf("00c0d740_TEST PASS\n");
    return 0;
}