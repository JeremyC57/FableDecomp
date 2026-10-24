// behaviour test for 0x00C0DCF0 CString::Append(const CString&): concatenate another string's whole text in place
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

static int g_lastLen = -1;
static const char* g_lastSrc = 0;

class CString {
public:
    char* m_pchData;
    CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    void ConcatInPlace(int nSrcLen, const char* lpszSrcData) { g_lastLen = nSrcLen; g_lastSrc = lpszSrcData; }   // 0x00C0C270 stand-in
    void Append(const CString& string);
};

void CString::Append(const CString& string)
{
    ConcatInPlace(string.GetData()->nDataLength, string.m_pchData);
}

int main()
{
    CStringData* d = MakeData("tail", 1);
    CString a; a.m_pchData = _afxPchNil;
    CString b; b.m_pchData = d->data();
    a.Append(b);
    if (g_lastLen != 4 || g_lastSrc != d->data()) { std::printf("00c0dcf0_TEST FAIL\n"); return 1; }
    CString e; e.m_pchData = _afxPchNil;
    a.Append(e);
    if (g_lastLen != 0 || g_lastSrc != _afxPchNil || g_asserts != 0) { std::printf("00c0dcf0_TEST FAIL\n"); return 1; }
    std::free(d);
    std::printf("00c0dcf0_TEST PASS\n");
    return 0;
}