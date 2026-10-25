// Behaviour test for 0x00CA6B40 CBaseRenderer::BeginFlush (DirectShow renbase.cpp): not-ready while paused, then release the source thread, cancel the advise, drop the sample and wait for Receive, in that order.
// Test model: plain C++98 copies of the members the function touches; Win32 / COM callees are recording stand-ins.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifdef _MSC_VER
typedef __int64 LONGLONG;
#else
typedef long long LONGLONG;
#endif
typedef LONGLONG REFERENCE_TIME;
typedef int HRESULT;
struct GUID { unsigned int Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
inline int operator==(const GUID& a, const GUID& b) { return std::memcmp(&a, &b, sizeof(GUID)) == 0; }
#define NOERROR 0
#define S_OK 0
#define S_FALSE 1
#define E_POINTER ((HRESULT)0x80004003u)
#define E_OUTOFMEMORY ((HRESULT)0x8007000Eu)
#define E_FAIL ((HRESULT)0x80004005u)
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define CheckPointer(p, ret) { if ((p) == 0) return (ret); }
#define State_Stopped 0
#define State_Paused 1
#define State_Running 2
struct CCritSec {
    int depth; int entered;
    CCritSec() : depth(0), entered(0) {}
    void Lock() { depth++; entered++; }
    void Unlock() { depth--; }
};
struct CAutoLock {
    CCritSec* m_pLock;
    CAutoLock(CCritSec* plock) : m_pLock(plock) { m_pLock->Lock(); }
    ~CAutoLock() { m_pLock->Unlock(); }
};
struct CAMEvent {                                // the test's event: a flag instead of a kernel HANDLE
    int signalled;
    CAMEvent() : signalled(0) {}
    void Set() { signalled = 1; }
    void Reset() { signalled = 0; }
};
static char g_log[16]; static int g_n;
struct CBaseRenderer {
    int m_State; CAMEvent m_evComplete; int can_wait_arg;
    void NotReady() { m_evComplete.Reset(); }
    virtual HRESULT SourceThreadCanWait(int b) { can_wait_arg = b; g_log[g_n++] = 'S'; return S_OK; }
    virtual HRESULT CancelNotification() { g_log[g_n++] = 'C'; return S_OK; }
    virtual HRESULT ClearPendingSample() { g_log[g_n++] = 'P'; return S_OK; }
    void WaitForReceiveToComplete() { g_log[g_n++] = 'W'; }
    HRESULT BeginFlush();
};
HRESULT CBaseRenderer::BeginFlush()
{
    if (m_State == State_Paused) {
        NotReady();
    }
    SourceThreadCanWait(0);
    CancelNotification();
    ClearPendingSample();
    WaitForReceiveToComplete();
    return NOERROR;
}
int main()
{
    CBaseRenderer r; r.m_State = State_Running; r.m_evComplete.Set(); r.can_wait_arg = 5;
    int ok = 1;
    ok &= r.BeginFlush() == NOERROR && r.m_evComplete.signalled && r.can_wait_arg == 0 && std::strcmp(g_log, "SCPW") == 0;
    r.m_State = State_Paused; g_n = 0;
    ok &= r.BeginFlush() == NOERROR && !r.m_evComplete.signalled && g_n == 4;
    if (!ok) { std::printf("00ca6b40_TEST FAIL\n"); return 1; }
    std::printf("00ca6b40_TEST PASS\n");
    return 0;
}