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
// CBaseAllocator::GetFreeCount: buffers not yet allocated plus the samples on the free list.
struct CCritSec { int locks, unlocks; CCritSec() : locks(0), unlocks(0) {} void Lock() { locks++; } void Unlock() { unlocks++; } };
class CAutoLock { CCritSec* m_pLock; public: CAutoLock(CCritSec* p) : m_pLock(p) { m_pLock->Lock(); } ~CAutoLock() { m_pLock->Unlock(); } };
struct CSampleList { int m_nOnList; int GetCount() const { return m_nOnList; } };
class CBaseAllocator : public CCritSec {
public:
    CSampleList m_lFree; long m_lCount, m_lAllocated;
    HRESULT GetFreeCount(LONG* plBuffersFree)
    {
        CAutoLock cObjectLock(this);
        *plBuffersFree = m_lCount - m_lAllocated + m_lFree.GetCount();
        return 0;
    }
};
static void run()
{
    CBaseAllocator a; a.m_lCount = 8; a.m_lAllocated = 5; a.m_lFree.m_nOnList = 2;
    LONG n = -1;
    CHECK(a.GetFreeCount(&n) == 0); CHECK(n == 5);
    a.m_lAllocated = 8; a.m_lFree.m_nOnList = 0;
    CHECK(a.GetFreeCount(&n) == 0); CHECK(n == 0);
    CHECK(a.locks == 2 && a.unlocks == 2);
}

int main()
{
    run();
    if (g_fail) { std::printf("00caa040_TEST FAIL\n"); return 1; }
    std::printf("00caa040_TEST PASS\n");
    return 0;
}