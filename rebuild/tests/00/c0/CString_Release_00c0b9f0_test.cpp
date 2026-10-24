// behaviour test for 0x00C0B9F0 CString::Release(CStringData*): static form, drop one reference unless it is the shared empty block
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
    static void FreeData(CStringData* pData) { FreeDataStub(pData); }
    static void Release(CStringData* pData);
};

void CString::Release(CStringData* pData)
{
    if (pData != _afxDataNil) {
        LIB_ASSERT(pData->nRefs != 0)
        if (TestInterlockedDecrement(&pData->nRefs) <= 0)
            FreeData(pData);
    }
}

int main()
{
    CStringData* d = MakeData("xyz", 2);
    CString::Release(d);
    if (d->nRefs != 1 || g_frees != 0) { std::printf("00c0b9f0_TEST FAIL\n"); return 1; }
    CString::Release(d);
    if (g_frees != 1) { std::printf("00c0b9f0_TEST FAIL\n"); return 1; }
    CString::Release(_afxDataNil);
    if (g_frees != 1 || g_nil.hdr.nRefs != -1 || g_asserts != 0) { std::printf("00c0b9f0_TEST FAIL\n"); return 1; }
    CStringData* z = MakeData("q", 0);                           // zero refs: asserts, then frees
    CString::Release(z);
    if (g_asserts != 1 || g_breaks != 1 || g_frees != 2) { std::printf("00c0b9f0_TEST FAIL\n"); return 1; }
    std::printf("00c0b9f0_TEST PASS\n");
    return 0;
}