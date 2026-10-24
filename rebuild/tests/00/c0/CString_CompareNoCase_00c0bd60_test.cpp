// behaviour test for 0x00C0BD60 CString::CompareNoCase: case-insensitive compare against a C string, asserting it is valid
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

static int TestStricmp(const char* a, const char* b)               // _stricmp stand-in
{
    for (;; ++a, ++b) {
        int ca = (unsigned char)*a, cb = (unsigned char)*b;
        if (ca >= 'A' && ca <= 'Z') ca += 'a' - 'A';
        if (cb >= 'A' && cb <= 'Z') cb += 'a' - 'A';
        if (ca != cb) return ca - cb;
        if (ca == 0) return 0;
    }
}

static inline bool IsValidString(const char* lpsz) { return lpsz != NULL; }

class CString {
public:
    char* m_pchData;
    int CompareNoCase(const char* lpsz) const;
};

int CString::CompareNoCase(const char* lpsz) const
{
    LIB_ASSERT(IsValidString(lpsz))
    return TestStricmp(m_pchData, lpsz);
}

int main()
{
    char text[] = "Hello";
    CString s; s.m_pchData = text;
    if (s.CompareNoCase("hELLO") != 0 || s.CompareNoCase("help") >= 0 || s.CompareNoCase("HELL") <= 0) { std::printf("00c0bd60_TEST FAIL\n"); return 1; }
    if (g_asserts != 0) { std::printf("00c0bd60_TEST FAIL\n"); return 1; }
    std::printf("00c0bd60_TEST PASS\n");
    return 0;
}