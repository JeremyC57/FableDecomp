// behaviour test for 0x00C0BE20 CString::CopyBeforeWrite: give a shared string its own copy of the text before a write
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
    CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    void Release()                                  // stand-in for 0x00C0B910
    {
        if (GetData() != _afxDataNil) {
            if (TestInterlockedDecrement(&GetData()->nRefs) <= 0)
                FreeDataStub(GetData());
            m_pchData = _afxPchNil;
        }
    }
    void AllocBuffer(int nLen)                      // stand-in for 0x00C0BDA0
    {
        if (nLen == 0) { m_pchData = _afxPchNil; return; }
        CStringData* pData = (CStringData*)std::malloc(sizeof(CStringData) + nLen + 1);
        pData->nAllocLength = nLen; pData->nRefs = 1; pData->data()[nLen] = 0; pData->nDataLength = nLen;
        m_pchData = pData->data();
    }
    void CopyBeforeWrite();
};

void CString::CopyBeforeWrite()
{
    if (GetData()->nRefs > 1) {
        CStringData* pData = GetData();
        Release();
        AllocBuffer(pData->nDataLength);
        std::memcpy(m_pchData, pData->data(), (pData->nDataLength + 1) * sizeof(char));
    }
    LIB_ASSERT(GetData()->nRefs <= 1)
}

int main()
{
    CStringData* d = MakeData("shared", 2);
    CString a; a.m_pchData = d->data();
    a.CopyBeforeWrite();
    if (a.m_pchData == d->data() || d->nRefs != 1 || std::strcmp(a.m_pchData, "shared") != 0) { std::printf("00c0be20_TEST FAIL\n"); return 1; }
    if (a.GetData()->nRefs != 1 || a.GetData()->nDataLength != 6) { std::printf("00c0be20_TEST FAIL\n"); return 1; }
    char* own = a.m_pchData;
    a.CopyBeforeWrite();                                          // already unique: untouched
    if (a.m_pchData != own || g_asserts != 0) { std::printf("00c0be20_TEST FAIL\n"); return 1; }
    std::free(d); std::free(a.GetData());
    std::printf("00c0be20_TEST PASS\n");
    return 0;
}