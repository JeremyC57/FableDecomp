// Behaviour test for 0x00CA5610 CBaseVideoRenderer::OnRenderEnd (DirectShow renbase.cpp): fold the render time into the running average unless it is an outlier, remember it, and throttle (Sleep m_trThrottle/10000 ms or Sleep(0)).
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
static unsigned int g_now; static int g_sleep_ms = -1;
static unsigned int timeGetTime() { return g_now; }
static void Sleep(unsigned int ms) { g_sleep_ms = (int)ms; }
#define AVGPERIOD 4
struct IMediaSample { int dummy; };
struct CBaseVideoRenderer {
    int m_trThrottle; int m_trRenderAvg; int m_trRenderLast; int m_tRenderStart;
    void ThrottleWait();
    void OnRenderEnd(IMediaSample* pMediaSample);
};
void CBaseVideoRenderer::ThrottleWait()
{
    if (m_trThrottle > 0) {
        int iThrottle = m_trThrottle / 10000;
        Sleep(iThrottle);
    } else {
        Sleep(0);
    }
}
void CBaseVideoRenderer::OnRenderEnd(IMediaSample* pMediaSample)
{
    int tr = (timeGetTime() - m_tRenderStart) * 10000;
    if (tr < m_trRenderAvg * 2 || tr < 2 * m_trRenderLast) {
        m_trRenderAvg = (tr + (AVGPERIOD - 1) * m_trRenderAvg) / AVGPERIOD;
    }
    m_trRenderLast = tr;
    ThrottleWait();
}
int main()
{
    CBaseVideoRenderer v; v.m_trThrottle = 0; v.m_trRenderAvg = 40000; v.m_trRenderLast = 40000; v.m_tRenderStart = 1000;
    int ok = 1;
    g_now = 1006;                       // 6 ms = 60000 units: under 2x the average -> averaged in
    v.OnRenderEnd(0);
    ok &= v.m_trRenderAvg == (60000 + 3 * 40000) / 4 && v.m_trRenderLast == 60000 && g_sleep_ms == 0;
    v.m_trThrottle = 25000; g_now = 1100;   // 100 ms outlier: average kept, last updated, Sleep(2)
    v.OnRenderEnd(0);
    ok &= v.m_trRenderAvg == 45000 && v.m_trRenderLast == 1000000 && g_sleep_ms == 2;
    g_now = 1050;                       // 50 ms: over 2x avg but under 2x last -> averaged in
    v.OnRenderEnd(0);
    ok &= v.m_trRenderAvg == (500000 + 3 * 45000) / 4 && v.m_trRenderLast == 500000;
    if (!ok) { std::printf("00ca5610_TEST FAIL\n"); return 1; }
    std::printf("00ca5610_TEST PASS\n");
    return 0;
}