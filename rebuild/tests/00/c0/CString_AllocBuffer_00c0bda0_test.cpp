// behaviour test for 0x00C0BDA0 CString::AllocBuffer: allocate a one-reference block for nLen characters (empty string for 0)
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
#include <limits.h>

class CString {
public:
    char* m_pchData;
    void Init() { m_pchData = _afxPchNil; }
    void AllocBuffer(int nLen);
};

void CString::AllocBuffer(int nLen)
{
    LIB_ASSERT(nLen >= 0)
    LIB_ASSERT(nLen <= INT_MAX-1)
    if (nLen == 0)
        Init();
    else {
        CStringData* pData = (CStringData*)operator new(sizeof(CStringData) + (nLen + 1) * sizeof(char));
        pData->nAllocLength = nLen;
        pData->nRefs = 1;
        pData->data()[nLen] = '\0';
        pData->nDataLength = nLen;
        m_pchData = pData->data();
    }
}

int main()
{
    CString s; s.m_pchData = 0;
    s.AllocBuffer(0);
    if (s.m_pchData != _afxPchNil) { std::printf("00c0bda0_TEST FAIL\n"); return 1; }
    s.AllocBuffer(10);
    CStringData* d = ((CStringData*)s.m_pchData) - 1;
    if (d->nRefs != 1 || d->nDataLength != 10 || d->nAllocLength != 10 || s.m_pchData[10] != 0 || g_asserts != 0) { std::printf("00c0bda0_TEST FAIL\n"); return 1; }
    operator delete(d);
    std::printf("00c0bda0_TEST PASS\n");
    return 0;
}