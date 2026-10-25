// Behaviour test (standalone; compiled separately from the source). Plain C++98: the COM interfaces are modelled
// as ordinary virtual classes (no calling-convention keywords), the Win32 imports as local fakes.
#include <cstdio>
#include <cstring>
typedef int HRESULT;
typedef unsigned int ULONG;
typedef unsigned int DWORD;
typedef int LONG;
typedef int BOOL;
typedef void* HANDLE;
typedef unsigned short WCHAR;
typedef unsigned char BYTE;
#ifdef _MSC_VER
typedef __int64 LONGLONG;
#else
typedef long long LONGLONG;
#endif
typedef LONGLONG REFERENCE_TIME;
#ifdef _MSC_VER
#define LL(x) x##i64
#else
#define LL(x) x##LL
#endif
struct GUID { unsigned int Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
typedef GUID IID;
inline bool operator==(const GUID& a, const GUID& b) { return std::memcmp(&a, &b, sizeof(GUID)) == 0; }
#define HR(x) ((HRESULT)(x))
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed at line %d\n", __LINE__); } } while (0)
// CMediaSample::Release: last reference resets the sample (drops a changed media type, clears flags and stream id)
// and hands it back to its allocator; returns the new count.
static LONG InterlockedDecrement(LONG* p) { return --*p; }
class CMediaSample;
struct CBaseAllocator { CMediaSample* released; int count; CBaseAllocator() : released(0), count(0) {} HRESULT ReleaseBuffer(CMediaSample* p) { released = p; count++; return 0; } };
class CMediaSample {
public:
    enum { Sample_TypeChanged = 0x08 };
    DWORD m_dwFlags; DWORD m_dwTypeSpecificFlags; CBaseAllocator* m_pAllocator; DWORD m_dwStreamId; LONG m_cRef; int setTypeNull;
    CMediaSample() : m_dwFlags(0), m_dwTypeSpecificFlags(0), m_pAllocator(0), m_dwStreamId(0), m_cRef(1), setTypeNull(0) {}
    virtual HRESULT SetMediaType(void* p) { if (p == 0) setTypeNull++; return 0; }
    ULONG Release()
    {
        LONG lRef;
        if (m_cRef == 1) {
            lRef = 0;
            m_cRef = 0;
        } else {
            lRef = InterlockedDecrement(&m_cRef);
        }
        if (lRef == 0) {
            if (m_dwFlags & Sample_TypeChanged) {
                SetMediaType(0);
            }
            m_dwFlags = 0;
            m_dwTypeSpecificFlags = 0;
            m_dwStreamId = 0;
            m_pAllocator->ReleaseBuffer(this);
        }
        return (ULONG)lRef;
    }
};
static void run()
{
    CBaseAllocator a; CMediaSample s; s.m_pAllocator = &a;
    s.m_cRef = 3; s.m_dwFlags = 0x19; s.m_dwStreamId = 5; s.m_dwTypeSpecificFlags = 7;
    CHECK(s.Release() == 2); CHECK(a.count == 0); CHECK(s.m_dwFlags == 0x19);
    CHECK(s.Release() == 1);
    CHECK(s.Release() == 0); CHECK(s.m_cRef == 0); CHECK(a.count == 1); CHECK(a.released == &s);
    CHECK(s.setTypeNull == 1); CHECK(s.m_dwFlags == 0); CHECK(s.m_dwStreamId == 0); CHECK(s.m_dwTypeSpecificFlags == 0);
    s.m_cRef = 1; s.m_dwFlags = 0x01;
    CHECK(s.Release() == 0); CHECK(s.setTypeNull == 1); CHECK(a.count == 2);
}

int main()
{
    run();
    if (g_fail) { std::printf("00ca9080_TEST FAIL\n"); return 1; }
    std::printf("00ca9080_TEST PASS\n");
    return 0;
}