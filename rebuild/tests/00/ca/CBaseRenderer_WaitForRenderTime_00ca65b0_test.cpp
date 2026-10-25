// Behaviour test for 0x00CA65B0 CBaseRenderer::WaitForRenderTime (DirectShow renbase.cpp): wait in 10 s slices on {thread signal, render event}; VFW_E_STATE_CHANGED when the thread signal fires, else clear the advise cookie.
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
#define WAIT_TIMEOUT 258
#define WAIT_OBJECT_0 0
#define RENDER_TIMEOUT 10000
#define VFW_E_STATE_CHANGED ((HRESULT)0x80040223u)
static unsigned int g_script[4]; static int g_waits; static void* g_h0; static void* g_h1; static unsigned int g_ms;
static unsigned int WaitForMultipleObjects(unsigned int n, void* const* h, int all, unsigned int ms)
{
    g_h0 = h[0]; g_h1 = h[1]; g_ms = ms + n * 0 + all * 0;
    return g_script[g_waits++];
}
struct CBaseRenderer {
    CAMEvent m_RenderEvent; CAMEvent m_ThreadSignal; unsigned int m_dwAdvise; int starts, ends;
    CBaseRenderer() : m_dwAdvise(7), starts(0), ends(0) {}
    virtual void OnWaitStart() { starts++; }
    virtual void OnWaitEnd() { ends++; }
    void SignalTimerFired() { m_dwAdvise = 0; }
    HRESULT WaitForRenderTime();
};
HRESULT CBaseRenderer::WaitForRenderTime()
{
    void* WaitObjects[] = { &m_ThreadSignal, &m_RenderEvent };
    unsigned int Result = WAIT_TIMEOUT;
    OnWaitStart();
    while (Result == WAIT_TIMEOUT) {
        Result = WaitForMultipleObjects(2, WaitObjects, 0, RENDER_TIMEOUT);
    }
    OnWaitEnd();
    if (Result == WAIT_OBJECT_0) {
        return VFW_E_STATE_CHANGED;
    }
    SignalTimerFired();
    return NOERROR;
}
int main()
{
    CBaseRenderer r;
    int ok = 1;
    g_script[0] = WAIT_TIMEOUT; g_script[1] = WAIT_TIMEOUT; g_script[2] = WAIT_OBJECT_0 + 1;
    ok &= r.WaitForRenderTime() == NOERROR && g_waits == 3 && r.m_dwAdvise == 0 && r.starts == 1 && r.ends == 1;
    ok &= g_h0 == &r.m_ThreadSignal && g_h1 == &r.m_RenderEvent && g_ms == RENDER_TIMEOUT;
    r.m_dwAdvise = 9; g_waits = 0; g_script[0] = WAIT_OBJECT_0;
    ok &= r.WaitForRenderTime() == VFW_E_STATE_CHANGED && g_waits == 1 && r.m_dwAdvise == 9 && r.starts == 2 && r.ends == 2;
    if (!ok) { std::printf("00ca65b0_TEST FAIL\n"); return 1; }
    std::printf("00ca65b0_TEST PASS\n");
    return 0;
}