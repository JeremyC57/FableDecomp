// Behaviour test for 0x00CA6BC0 CBaseRenderer::BreakConnect (DirectShow renbase.cpp): release the quality sink; S_FALSE when unconnected; VFW_E_NOT_STOPPED when active on a pin that cannot reconnect; else reset and stop streaming if running.
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
#define VFW_E_NOT_STOPPED ((HRESULT)0x80040224u)
struct IQualityControl { int refs; virtual unsigned int Release() { return --refs; } };
struct CRendererInputPin {
    int connected; bool m_bCanReconnectWhenActive;
    int IsConnected() { return connected; }
    bool CanReconnectWhenActive() { return m_bCanReconnectWhenActive; }
};
static char g_log[16]; static int g_n;
struct CBaseRenderer {
    int m_State; int m_bAbort; int m_bRepaintStatus; IQualityControl* m_pQSink; CRendererInputPin* m_pInputPin;
    void SetRepaintStatus(int b) { m_bRepaintStatus = b; g_log[g_n++] = 'R'; }
    virtual HRESULT ResetEndOfStream() { g_log[g_n++] = 'E'; return S_OK; }
    virtual HRESULT ClearPendingSample() { g_log[g_n++] = 'P'; return S_OK; }
    virtual HRESULT StopStreaming() { g_log[g_n++] = 'S'; return S_OK; }
    HRESULT BreakConnect();
};
HRESULT CBaseRenderer::BreakConnect()
{
    if (m_pQSink) {
        m_pQSink->Release();
        m_pQSink = 0;
    }
    if (m_pInputPin->IsConnected() == 0) {
        return S_FALSE;
    }
    if (m_State != State_Stopped && !m_pInputPin->CanReconnectWhenActive()) {
        return VFW_E_NOT_STOPPED;
    }
    SetRepaintStatus(0);
    ResetEndOfStream();
    ClearPendingSample();
    m_bAbort = 0;
    if (State_Running == m_State) {
        StopStreaming();
    }
    return NOERROR;
}
int main()
{
    IQualityControl q; q.refs = 2;
    CRendererInputPin pin; pin.connected = 0; pin.m_bCanReconnectWhenActive = false;
    CBaseRenderer r; r.m_State = State_Running; r.m_bAbort = 1; r.m_bRepaintStatus = 1; r.m_pQSink = &q; r.m_pInputPin = &pin;
    int ok = 1;
    ok &= r.BreakConnect() == S_FALSE && q.refs == 1 && r.m_pQSink == 0 && g_n == 0;
    pin.connected = 1;
    ok &= r.BreakConnect() == VFW_E_NOT_STOPPED && g_n == 0 && r.m_bAbort == 1;
    pin.m_bCanReconnectWhenActive = true;
    ok &= r.BreakConnect() == NOERROR && std::strcmp(g_log, "REPS") == 0 && r.m_bAbort == 0 && r.m_bRepaintStatus == 0 && q.refs == 1;
    g_n = 0; std::memset(g_log, 0, sizeof(g_log)); pin.m_bCanReconnectWhenActive = false; r.m_State = State_Stopped; r.m_bAbort = 1;
    ok &= r.BreakConnect() == NOERROR && std::strcmp(g_log, "REP") == 0 && r.m_bAbort == 0;
    if (!ok) { std::printf("00ca6bc0_TEST FAIL\n"); return 1; }
    std::printf("00ca6bc0_TEST PASS\n");
    return 0;
}