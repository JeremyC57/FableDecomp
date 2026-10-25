// Behaviour test for 0x00CA6B80 CBaseRenderer::CompleteConnect (DirectShow renbase.cpp): clear the abort flag; connected while running: start streaming (failure returned) and disable EC_REPAINT, else enable it.
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
struct IPin { int dummy; };
struct CBaseRenderer {
    int m_State; int m_bAbort; int m_bRepaintStatus; HRESULT start_hr; int starts;
    CBaseRenderer() : m_State(0), m_bAbort(1), m_bRepaintStatus(-1), start_hr(S_OK), starts(0) {}
    int GetRealState() { return m_State; }
    void SetRepaintStatus(int bRepaint) { m_bRepaintStatus = bRepaint; }
    virtual HRESULT StartStreaming() { starts++; return start_hr; }
    HRESULT CompleteConnect(IPin* pReceivePin);
};
HRESULT CBaseRenderer::CompleteConnect(IPin* pReceivePin)
{
    m_bAbort = 0;
    if (State_Running == GetRealState()) {
        HRESULT hr = StartStreaming();
        if (FAILED(hr)) {
            return hr;
        }
        SetRepaintStatus(0);
    } else {
        SetRepaintStatus(1);
    }
    return NOERROR;
}
int main()
{
    CBaseRenderer r; IPin p;
    int ok = 1;
    ok &= r.CompleteConnect(&p) == NOERROR && r.m_bAbort == 0 && r.m_bRepaintStatus == 1 && r.starts == 0;
    r.m_State = State_Running; r.m_bAbort = 1;
    ok &= r.CompleteConnect(&p) == NOERROR && r.m_bAbort == 0 && r.m_bRepaintStatus == 0 && r.starts == 1;
    r.start_hr = E_FAIL; r.m_bRepaintStatus = 7;
    ok &= r.CompleteConnect(&p) == E_FAIL && r.m_bRepaintStatus == 7 && r.starts == 2;
    if (!ok) { std::printf("00ca6b80_TEST FAIL\n"); return 1; }
    std::printf("00ca6b80_TEST PASS\n");
    return 0;
}