// Behaviour test for 0x00CAA290 CMemAllocator::SetProperties: E_POINTER, alignment against the allocation granularity,
// committed / outstanding-buffer refusals, size rounding with the prefix, and the lock held across the call.
#include <cstdio>
#include <cstring>

typedef long HRESULT;
typedef long LONG;
struct ALLOCATOR_PROPERTIES { long cBuffers; long cbBuffer; long cbAlign; long cbPrefix; };

static int g_lockDepth = 0, g_lockCalls = 0;
static unsigned long g_granularity = 0x10000;

class CCritSec {
public:
    void Lock() { ++g_lockDepth; ++g_lockCalls; }
    void Unlock() { --g_lockDepth; }
};
class CAutoLock {
    CCritSec* m_pLock;
public:
    CAutoLock(CCritSec* plock) { m_pLock = plock; m_pLock->Lock(); }
    ~CAutoLock() { m_pLock->Unlock(); }
};

class CMemAllocatorModel : public CCritSec {
public:
    struct CSampleList { void* m_List; int m_nOnList; int GetCount() const { return m_nOnList; } } m_lFree;
    long m_lCount, m_lAllocated, m_lSize, m_lAlignment, m_lPrefix;
    int m_bChanged, m_bCommitted;
    CMemAllocatorModel() : m_lCount(0), m_lAllocated(0), m_lSize(0), m_lAlignment(0), m_lPrefix(0), m_bChanged(0), m_bCommitted(0)
    { m_lFree.m_List = 0; m_lFree.m_nOnList = 0; }

    HRESULT SetProperties(ALLOCATOR_PROPERTIES* pRequest, ALLOCATOR_PROPERTIES* pActual)
    {
        if (pActual == 0) return (HRESULT)0x80004003L;
        CAutoLock cObjectLock(this);
        std::memset(pActual, 0, sizeof(ALLOCATOR_PROPERTIES));
        unsigned long dwAllocationGranularity = g_granularity;      // GetSystemInfo
        if (pRequest->cbAlign == 0 || (dwAllocationGranularity & (pRequest->cbAlign - 1)) != 0)
            return (HRESULT)0x8004020EL;
        if (m_bCommitted == 1)
            return (HRESULT)0x8004020FL;
        if (m_lFree.GetCount() < m_lAllocated)
            return (HRESULT)0x80040210L;
        LONG lSize = pRequest->cbBuffer + pRequest->cbPrefix;
        LONG lRemainder = lSize % pRequest->cbAlign;
        if (lRemainder != 0)
            lSize = lSize - lRemainder + pRequest->cbAlign;
        pActual->cbBuffer = m_lSize = (lSize - pRequest->cbPrefix);
        pActual->cBuffers = m_lCount = pRequest->cBuffers;
        pActual->cbAlign = m_lAlignment = pRequest->cbAlign;
        pActual->cbPrefix = m_lPrefix = pRequest->cbPrefix;
        m_bChanged = 1;
        return 0;
    }
};

int main()
{
    int bad = 0;
    CMemAllocatorModel a;
    ALLOCATOR_PROPERTIES req = { 4, 1000, 16, 8 }, act;
    bad |= a.SetProperties(&req, 0) != (HRESULT)0x80004003L || g_lockCalls != 0;
    std::memset(&act, 0x55, sizeof(act));
    bad |= a.SetProperties(&req, &act) != 0 || g_lockDepth != 0 || g_lockCalls != 1;
    // (1000 + 8) rounded up to 16 = 1008 (already aligned) -> cbBuffer 1000
    bad |= act.cbBuffer != 1000 || act.cBuffers != 4 || act.cbAlign != 16 || act.cbPrefix != 8 || !a.m_bChanged
           || a.m_lSize != 1000 || a.m_lCount != 4 || a.m_lAlignment != 16 || a.m_lPrefix != 8;
    ALLOCATOR_PROPERTIES req2 = { 2, 1001, 32, 4 };           // 1005 -> 1024, minus prefix 1020
    bad |= a.SetProperties(&req2, &act) != 0 || act.cbBuffer != 1020 || a.m_lSize != 1020;
    ALLOCATOR_PROPERTIES bad0 = { 1, 10, 0, 0 };
    std::memset(&act, 0x55, sizeof(act));
    bad |= a.SetProperties(&bad0, &act) != (HRESULT)0x8004020EL || act.cbBuffer != 0 || act.cBuffers != 0 || g_lockDepth != 0;
    g_granularity = 0x1000;
    ALLOCATOR_PROPERTIES big = { 1, 10, 0x2000, 0 };          // granularity & (align - 1) != 0
    bad |= a.SetProperties(&big, &act) != (HRESULT)0x8004020EL;
    a.m_bCommitted = 1;
    bad |= a.SetProperties(&req, &act) != (HRESULT)0x8004020FL || g_lockDepth != 0;
    a.m_bCommitted = 0; a.m_lAllocated = 3; a.m_lFree.m_nOnList = 2;
    bad |= a.SetProperties(&req, &act) != (HRESULT)0x80040210L;
    a.m_lFree.m_nOnList = 3;
    bad |= a.SetProperties(&req, &act) != 0 || g_lockDepth != 0;
    if (bad) { std::printf("00caa290_TEST FAIL\n"); return 1; }
    std::printf("00caa290_TEST PASS\n");
    return 0;
}