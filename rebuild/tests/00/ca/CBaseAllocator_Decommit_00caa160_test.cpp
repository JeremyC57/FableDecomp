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
// CBaseAllocator::Decommit: stop handing out buffers; free them now (and drop the commit reference) when all are
// back on the free list, otherwise mark the decommit as in progress; wake any waiters either way.
struct CCritSec { int locks, unlocks; CCritSec() : locks(0), unlocks(0) {} void Lock() { locks++; } void Unlock() { unlocks++; } };
class CAutoLock { CCritSec* m_pLock; public: CAutoLock(CCritSec* p) : m_pLock(p) { m_pLock->Lock(); } ~CAutoLock() { m_pLock->Unlock(); } };
static LONG g_released = 0;
static BOOL ReleaseSemaphore(HANDLE, LONG n, LONG*) { g_released += n; return 1; }
struct CSampleList { int m_nOnList; int GetCount() const { return m_nOnList; } };
class CBaseAllocator : public CCritSec {
public:
    CSampleList m_lFree; HANDLE m_hSem; long m_lWaiting; long m_lAllocated; BOOL m_bCommitted; BOOL m_bDecommitInProgress;
    int frees, releases, lockedAtRelease;
    CBaseAllocator() : m_hSem(0), m_lWaiting(0), m_lAllocated(0), m_bCommitted(0), m_bDecommitInProgress(0), frees(0), releases(0), lockedAtRelease(-1) { m_lFree.m_nOnList = 0; }
    virtual void Free() { frees++; }
    ULONG Release() { releases++; lockedAtRelease = locks - unlocks; return 0; }
    void NotifySample()
    {
        if (m_lWaiting != 0) {
            ReleaseSemaphore(m_hSem, m_lWaiting, 0);
            m_lWaiting = 0;
        }
    }
    HRESULT Decommit()
    {
        BOOL bRelease = 0;
        {
            CAutoLock cObjectLock(this);
            if (m_bCommitted == 0) {
                if (m_bDecommitInProgress == 0) {
                    return 0;
                }
            }
            m_bCommitted = 0;
            if (m_lFree.GetCount() < m_lAllocated) {
                m_bDecommitInProgress = 1;
            } else {
                m_bDecommitInProgress = 0;
                Free();
                bRelease = 1;
            }
            NotifySample();
        }
        if (bRelease) {
            Release();
        }
        return 0;
    }
};
static void run()
{
    { CBaseAllocator a; CHECK(a.Decommit() == 0); CHECK(a.frees == 0 && a.releases == 0); CHECK(a.locks == 1 && a.unlocks == 1); }
    { CBaseAllocator a; a.m_bCommitted = 1; a.m_lAllocated = 4; a.m_lFree.m_nOnList = 2; a.m_lWaiting = 3; g_released = 0;
      CHECK(a.Decommit() == 0); CHECK(a.m_bCommitted == 0); CHECK(a.m_bDecommitInProgress == 1);
      CHECK(a.frees == 0 && a.releases == 0); CHECK(g_released == 3 && a.m_lWaiting == 0); }
    { CBaseAllocator a; a.m_bDecommitInProgress = 1; a.m_lAllocated = 4; a.m_lFree.m_nOnList = 4;
      CHECK(a.Decommit() == 0); CHECK(a.m_bDecommitInProgress == 0); CHECK(a.frees == 1 && a.releases == 1);
      CHECK(a.lockedAtRelease == 0); }
}

int main()
{
    run();
    if (g_fail) { std::printf("00caa160_TEST FAIL\n"); return 1; }
    std::printf("00caa160_TEST PASS\n");
    return 0;
}