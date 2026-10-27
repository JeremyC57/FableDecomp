// Behaviour test for 0x00CABB70 (CBaseAllocator::ReleaseBuffer): the strmbase body over fakes.
// Checks: E_POINTER for null; the sample goes on the free list; waiters are released through the semaphore and the
// count cleared; when a decommit is pending and the last buffer is back, Free() runs, the flag clears and the
// allocator Releases itself (after the notify callback); otherwise no Free / Release.
#include <cstdio>
#include <cstring>
typedef int HRESULT;
typedef int BOOL;
typedef int LONG;
static char g_log[512];
static void logs(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_released = -1;
static void ReleaseSemaphore(void*, LONG count, void*) { g_released = count; logs("ReleaseSemaphore"); }
struct IMediaSample { int id; };
class CMediaSample : public IMediaSample { public: CMediaSample* m_pNext; };
struct IMemAllocatorNotifyCallbackTemp { void NotifyRelease() { logs("NotifyRelease"); } };
class CAutoLock {
public:
    explicit CAutoLock(void*) { logs("Lock"); }
    ~CAutoLock() { logs("Unlock"); }
};
class CBaseAllocator {
public:
    class CSampleList {
    public:
        CSampleList() : m_List(0), m_nOnList(0) { }
        int GetCount() const { return m_nOnList; }
        void Add(CMediaSample* pSample) { pSample->m_pNext = m_List; m_List = pSample; m_nOnList++; }
        CMediaSample* m_List;
        int m_nOnList;
    };
    CSampleList m_lFree;
    void* m_hSem;
    LONG m_lWaiting, m_lAllocated;
    BOOL m_bDecommitInProgress;
    IMemAllocatorNotifyCallbackTemp* m_pNotify;
    void Free() { logs("Free"); }
    void Release() { logs("Release"); }
    HRESULT ReleaseBuffer(IMediaSample* pBuffer);
    void NotifySample() {
        if (m_lWaiting != 0) {
            ReleaseSemaphore(m_hSem, m_lWaiting, 0);
            m_lWaiting = 0;
        }
    }
};
HRESULT CBaseAllocator::ReleaseBuffer(IMediaSample* pSample)
{
    if (pSample == 0) {
        return (HRESULT)0x80004003;
    }
    BOOL bRelease = 0;
    {
        CAutoLock cal(this);
        m_lFree.Add((CMediaSample*)pSample);
        if (m_lWaiting != 0) {
            NotifySample();
        }
        LONG l1 = m_lFree.GetCount();
        if (m_bDecommitInProgress && (l1 == m_lAllocated)) {
            Free();
            m_bDecommitInProgress = 0;
            bRelease = 1;
        }
    }
    if (m_pNotify) {
        m_pNotify->NotifyRelease();
    }
    if (bRelease) {
        Release();
    }
    return 0;
}
static int fail(int line) { std::printf("00cabb70_TEST FAIL (line %d) %s\n", line, g_log); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)
int main()
{
    CBaseAllocator a;
    IMemAllocatorNotifyCallbackTemp notify;
    CMediaSample s1, s2;
    a.m_hSem = 0; a.m_lWaiting = 3; a.m_lAllocated = 2; a.m_bDecommitInProgress = 1; a.m_pNotify = 0;
    CHECK(a.ReleaseBuffer(0) == (HRESULT)0x80004003 && g_log[0] == 0);
    CHECK(a.ReleaseBuffer(&s1) == 0);
    CHECK(std::strcmp(g_log, "Lock;ReleaseSemaphore;Unlock;") == 0 && g_released == 3 && a.m_lWaiting == 0);
    CHECK(a.m_lFree.GetCount() == 1 && a.m_lFree.m_List == &s1 && a.m_bDecommitInProgress == 1);
    g_log[0] = 0; a.m_pNotify = &notify;
    CHECK(a.ReleaseBuffer(&s2) == 0);
    CHECK(std::strcmp(g_log, "Lock;Free;Unlock;NotifyRelease;Release;") == 0);
    CHECK(a.m_lFree.GetCount() == 2 && a.m_lFree.m_List == &s2 && s2.m_pNext == &s1 && a.m_bDecommitInProgress == 0);
    std::printf("00cabb70_TEST PASS\n");
    return 0;
}