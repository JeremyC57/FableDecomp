// behaviour test for 0x00C0E440 CString::Mid(int): the substring from nFirst to the end, through Mid(nFirst, length - nFirst)
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

static int g_lastFirst = -1, g_lastCount = -1;

class CString {
public:
    char* m_pchData;
    CString() : m_pchData(_afxPchNil) { }
    CString(const CString& stringSrc) : m_pchData(stringSrc.m_pchData) { }   // stand-in for 0x00C0DAB0 (shares the text)
    ~CString() { }                                                         // stand-in for 0x00C0BA40
    CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    CString Mid(int nFirst, int nCount) const                              // stand-in for 0x00C0DF50
    {
        g_lastFirst = nFirst; g_lastCount = nCount;
        CString result;
        if (nCount > 0) result.m_pchData = m_pchData + nFirst;             // a view is enough for the test
        return result;
    }
    CString Mid(int nFirst) const;
};

CString CString::Mid(int nFirst) const
{
    return Mid(nFirst, GetData()->nDataLength - nFirst);
}

int main()
{
    CStringData* d = MakeData("filename.ext", 1);
    CString s; s.m_pchData = d->data();
    CString r = s.Mid(9);
    if (g_lastFirst != 9 || g_lastCount != 3 || std::strcmp(r.m_pchData, "ext") != 0) { std::printf("00c0e440_TEST FAIL\n"); return 1; }
    CString all = s.Mid(0);
    if (g_lastCount != 12 || std::strcmp(all.m_pchData, "filename.ext") != 0) { std::printf("00c0e440_TEST FAIL\n"); return 1; }
    std::free(d);
    std::printf("00c0e440_TEST PASS\n");
    return 0;
}