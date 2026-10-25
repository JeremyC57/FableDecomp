// Behaviour test for 0x00CA6670 CBaseRenderer::CompleteStateChange (DirectShow renbase.cpp): ready (S_OK) when unconnected, at end of stream, or holding a sample after a non-stopped state; else not ready (S_FALSE).
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
struct CRendererInputPin { int connected; int IsConnected() { return connected; } };
struct CBaseRenderer {
    CRendererInputPin* m_pInputPin; int m_bEOS; int have_sample; CAMEvent m_evComplete;
    int IsEndOfStream() { return m_bEOS; }
    void Ready() { m_evComplete.Set(); }
    void NotReady() { m_evComplete.Reset(); }
    virtual int HaveCurrentSample() { return have_sample; }
    HRESULT CompleteStateChange(int OldState);
};
HRESULT CBaseRenderer::CompleteStateChange(int OldState)
{
    if (m_pInputPin->IsConnected() == 0) { Ready(); return S_OK; }
    if (IsEndOfStream() == 1) { Ready(); return S_OK; }
    if (HaveCurrentSample() == 1) {
        if (OldState != State_Stopped) { Ready(); return S_OK; }
    }
    NotReady();
    return S_FALSE;
}
int main()
{
    CRendererInputPin pin; CBaseRenderer r; r.m_pInputPin = &pin;
    int ok = 1;
    pin.connected = 0; r.m_bEOS = 0; r.have_sample = 0;
    ok &= r.CompleteStateChange(State_Stopped) == S_OK && r.m_evComplete.signalled;
    pin.connected = 1;
    ok &= r.CompleteStateChange(State_Stopped) == S_FALSE && !r.m_evComplete.signalled;
    r.m_bEOS = 1; ok &= r.CompleteStateChange(State_Stopped) == S_OK && r.m_evComplete.signalled;
    r.m_bEOS = 0; r.have_sample = 1;
    ok &= r.CompleteStateChange(State_Stopped) == S_FALSE && !r.m_evComplete.signalled;   // a sample from stopped: still wait
    ok &= r.CompleteStateChange(State_Running) == S_OK && r.m_evComplete.signalled;
    if (!ok) { std::printf("00ca6670_TEST FAIL\n"); return 1; }
    std::printf("00ca6670_TEST PASS\n");
    return 0;
}