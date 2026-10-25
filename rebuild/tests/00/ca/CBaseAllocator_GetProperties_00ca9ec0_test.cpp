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
// CBaseAllocator::GetProperties: under the allocator lock report size, count, alignment and prefix.
struct ALLOCATOR_PROPERTIES { long cBuffers; long cbBuffer; long cbAlign; long cbPrefix; };
struct CCritSec { int locks, unlocks; CCritSec() : locks(0), unlocks(0) {} void Lock() { locks++; } void Unlock() { unlocks++; } };
class CAutoLock { CCritSec* m_pLock; public: CAutoLock(CCritSec* p) : m_pLock(p) { m_pLock->Lock(); } ~CAutoLock() { m_pLock->Unlock(); } };
class CBaseAllocator : public CCritSec {
public:
    long m_lCount, m_lSize, m_lAlignment, m_lPrefix;
    HRESULT GetProperties(ALLOCATOR_PROPERTIES* pActual)
    {
        if (pActual == 0) return HR(0x80004003);
        CAutoLock cObjectLock(this);
        pActual->cbBuffer = m_lSize;
        pActual->cBuffers = m_lCount;
        pActual->cbAlign = m_lAlignment;
        pActual->cbPrefix = m_lPrefix;
        return 0;
    }
};
static void run()
{
    CBaseAllocator a; a.m_lCount = 4; a.m_lSize = 4096; a.m_lAlignment = 16; a.m_lPrefix = 8;
    ALLOCATOR_PROPERTIES p = { 0, 0, 0, 0 };
    CHECK(a.GetProperties(&p) == 0);
    CHECK(p.cBuffers == 4 && p.cbBuffer == 4096 && p.cbAlign == 16 && p.cbPrefix == 8);
    CHECK(a.locks == 1 && a.unlocks == 1);
    CHECK(a.GetProperties(0) == HR(0x80004003)); CHECK(a.locks == 1);
}

int main()
{
    run();
    if (g_fail) { std::printf("00ca9ec0_TEST FAIL\n"); return 1; }
    std::printf("00ca9ec0_TEST PASS\n");
    return 0;
}