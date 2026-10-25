// Behaviour test for 0x00CA5DF0 CBaseVideoRenderer::get_AvgFrameRate (DirectShow renbase.cpp, IQualProp): frames drawn per 100 s of streaming time (elapsed since the start while streaming, else the stored total).
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
static unsigned int g_now;
static unsigned int timeGetTime() { return g_now; }
static int MulDiv(int a, int b, int c)       // Win32 MulDiv: 64-bit product, rounded quotient
{
    LONGLONG p = (LONGLONG)a * b;
    LONGLONG q = (p + (p >= 0 ? c / 2 : -(c / 2))) / c;
    return (int)q;
}
struct CBaseVideoRenderer {
    CCritSec m_InterfaceLock; int m_bStreaming; int m_tStreamingStart; int m_cFramesDrawn;
    HRESULT get_AvgFrameRate(int* piAvgFrameRate);
};
HRESULT CBaseVideoRenderer::get_AvgFrameRate(int* piAvgFrameRate)
{
    CheckPointer(piAvgFrameRate, E_POINTER);
    CAutoLock cVideoLock(&m_InterfaceLock);
    int t;
    if (m_bStreaming) {
        t = timeGetTime() - m_tStreamingStart;
    } else {
        t = m_tStreamingStart;
    }
    if (t <= 0) {
        *piAvgFrameRate = 0;
    } else {
        *piAvgFrameRate = MulDiv(100000, m_cFramesDrawn, t);
    }
    return NOERROR;
}
int main()
{
    CBaseVideoRenderer v; v.m_cFramesDrawn = 300; v.m_bStreaming = 1; v.m_tStreamingStart = 2000;
    int r = -1;
    int ok = 1;
    ok &= v.get_AvgFrameRate(0) == E_POINTER;
    g_now = 12000;                                // 300 frames in 10 s -> 30.00 fps = 3000
    ok &= v.get_AvgFrameRate(&r) == NOERROR && r == 3000;
    g_now = 2000; ok &= v.get_AvgFrameRate(&r) == NOERROR && r == 0;
    v.m_bStreaming = 0; v.m_tStreamingStart = 4000; ok &= v.get_AvgFrameRate(&r) == NOERROR && r == 7500;
    v.m_tStreamingStart = 0; ok &= v.get_AvgFrameRate(&r) == NOERROR && r == 0;
    ok &= v.m_InterfaceLock.entered == 4 && v.m_InterfaceLock.depth == 0;
    if (!ok) { std::printf("00ca5df0_TEST FAIL\n"); return 1; }
    std::printf("00ca5df0_TEST PASS\n");
    return 0;
}