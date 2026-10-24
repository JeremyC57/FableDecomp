// behaviour test for 0x00C0BFE0 CString::CString(const char*): copy a C string (null or empty gives the shared empty string)
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

class CString {
public:
    char* m_pchData;
    CString(const char* lpsz);
    void Init() { m_pchData = _afxPchNil; }
    void AllocBuffer(int nLen)                      // stand-in for 0x00C0BDA0
    {
        CStringData* pData = (CStringData*)std::malloc(sizeof(CStringData) + nLen + 1);
        pData->nAllocLength = nLen; pData->nRefs = 1; pData->data()[nLen] = 0; pData->nDataLength = nLen;
        m_pchData = pData->data();
    }
    static int SafeStrlen(const char* lpsz) { return (lpsz == NULL) ? 0 : (int)std::strlen(lpsz); }   // 0x00C0BCC0
};

CString::CString(const char* lpsz)
{
    Init();
    int nLen = SafeStrlen(lpsz);
    if (nLen != 0) {
        AllocBuffer(nLen);
        std::memcpy(m_pchData, lpsz, nLen * sizeof(char));
    }
}

int main()
{
    CString a("text");
    if (std::strcmp(a.m_pchData, "text") != 0 || (((CStringData*)a.m_pchData) - 1)->nDataLength != 4) { std::printf("00c0bfe0_TEST FAIL\n"); return 1; }
    CString b(""), c((const char*)0);
    if (b.m_pchData != _afxPchNil || c.m_pchData != _afxPchNil) { std::printf("00c0bfe0_TEST FAIL\n"); return 1; }
    std::free(((CStringData*)a.m_pchData) - 1);
    std::printf("00c0bfe0_TEST PASS\n");
    return 0;
}