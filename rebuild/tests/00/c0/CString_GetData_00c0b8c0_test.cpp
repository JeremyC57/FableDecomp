// behaviour test for 0x00C0B8C0 CString::GetData: the CStringData header in front of the characters, asserting a non-null buffer
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
    CStringData* GetData() const;
};

CStringData* CString::GetData() const
{
    LIB_ASSERT(m_pchData != NULL)
    return ((CStringData*)m_pchData) - 1;
}

int main()
{
    CStringData* d = MakeData("hello", 1);
    CString s; s.m_pchData = d->data();
    if (s.GetData() != d || s.GetData()->nDataLength != 5 || g_asserts != 0) { std::printf("00c0b8c0_TEST FAIL\n"); return 1; }
    CString e; e.m_pchData = _afxPchNil;
    if (e.GetData() != _afxDataNil) { std::printf("00c0b8c0_TEST FAIL\n"); return 1; }
    CString n; n.m_pchData = 0;
    n.GetData();
    if (g_asserts != 1 || g_breaks != 1) { std::printf("00c0b8c0_TEST FAIL\n"); return 1; }
    std::free(d);
    std::printf("00c0b8c0_TEST PASS\n");
    return 0;
}