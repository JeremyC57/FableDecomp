// Behaviour test for 0x00CABAE0 (CBaseAllocator::CBaseAllocator, unsigned short* name): the strmbase constructor body over fakes.
// Checks: the base gets the name and owner, every counter/flag is zeroed, the release-callback flag is stored, a
// semaphore (initial 0, max 0x7FFFFFFF) is made only when bEvent is set, and E_OUTOFMEMORY lands in *phr only on failure.
#include <cstdio>
typedef int HRESULT;
typedef int BOOL;
static int g_semCalls = 0, g_semInit = -1, g_semMax = 0;
static void* g_semResult = 0;
static void* CreateSemaphoreW(void*, int init, int max, void*) { ++g_semCalls; g_semInit = init; g_semMax = max; return g_semResult; }
struct IUnknown { int dummy; };
class CUnknown {
public:
    const unsigned short* m_pName;
    IUnknown* m_pUnknown;
    int m_cRef;
    CUnknown(const unsigned short* pName, IUnknown* pUnk) : m_pName(pName), m_pUnknown(pUnk), m_cRef(0) { }
};
class CBaseAllocator : public CUnknown {
public:
    void* m_hSem;
    int m_lWaiting, m_lCount, m_lAllocated, m_lSize, m_lAlignment, m_lPrefix;
    BOOL m_bChanged, m_bCommitted, m_bDecommitInProgress;
    void* m_pNotify;
    BOOL m_fEnableReleaseCallback;
    CBaseAllocator(unsigned short* pName, IUnknown* pUnk, HRESULT* phr, BOOL bEvent = 1, BOOL fEnableReleaseCallback = 0);
};
CBaseAllocator::CBaseAllocator(unsigned short* pName, IUnknown* pUnk, HRESULT* phr, BOOL bEvent, BOOL fEnableReleaseCallback)
    : CUnknown(pName, pUnk), m_lAllocated(0), m_bChanged(0), m_bCommitted(0), m_bDecommitInProgress(0), m_lSize(0),
      m_lCount(0), m_lAlignment(0), m_lPrefix(0), m_hSem(0), m_lWaiting(0),
      m_fEnableReleaseCallback(fEnableReleaseCallback), m_pNotify(0)
{
    if (bEvent) {
        m_hSem = CreateSemaphoreW(0, 0, 0x7FFFFFFF, 0);
        if (m_hSem == 0) {
            *phr = (HRESULT)0x8007000E;
            return;
        }
    }
}
static int fail(int line) { std::printf("00cabae0_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)
int main()
{
    static unsigned short name[] = { 'A', 'l', 'l', 'o', 'c', 0 };
    IUnknown owner;
    int semaphore;
    HRESULT hr = 0;
    g_semResult = &semaphore;
    CBaseAllocator a(name, &owner, &hr, 1, 1);
    CHECK(a.m_pName == name && a.m_pUnknown == &owner && hr == 0);
    CHECK(g_semCalls == 1 && g_semInit == 0 && g_semMax == 0x7FFFFFFF && a.m_hSem == &semaphore);
    CHECK(a.m_lWaiting == 0 && a.m_lCount == 0 && a.m_lAllocated == 0 && a.m_lSize == 0 && a.m_lAlignment == 0 && a.m_lPrefix == 0);
    CHECK(!a.m_bChanged && !a.m_bCommitted && !a.m_bDecommitInProgress && a.m_pNotify == 0 && a.m_fEnableReleaseCallback == 1);
    g_semResult = 0;
    CBaseAllocator b(name, 0, &hr);
    CHECK(g_semCalls == 2 && b.m_hSem == 0 && hr == (HRESULT)0x8007000E && b.m_fEnableReleaseCallback == 0);
    hr = 0;
    CBaseAllocator c(name, 0, &hr, 0, 0);
    CHECK(g_semCalls == 2 && c.m_hSem == 0 && hr == 0);
    std::printf("00cabae0_TEST PASS\n");
    return 0;
}