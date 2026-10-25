// Behaviour test for 0x00CA4F90 CRendererInputPin::EndOfStream (DirectShow renbase.cpp): under both renderer locks, check streaming, let the renderer note EOS, then pass it to CBaseInputPin.
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
struct CBaseRenderer {
    CCritSec m_InterfaceLock; CCritSec m_RendererLock;
    HRESULT eos_hr; int eos_calls; int locks_held_in_eos;
    CBaseRenderer() : eos_hr(S_OK), eos_calls(0), locks_held_in_eos(0) {}
    virtual HRESULT EndOfStream() { eos_calls++; locks_held_in_eos = m_InterfaceLock.depth == 1 && m_RendererLock.depth == 1; return eos_hr; }
};
struct CBaseInputPin {
    HRESULT streaming_hr; int base_calls;
    CBaseInputPin() : streaming_hr(S_OK), base_calls(0) {}
    virtual HRESULT CheckStreaming() { return streaming_hr; }
    HRESULT EndOfStream() { base_calls++; return 0x55; }
};
struct CRendererInputPin : CBaseInputPin {
    CBaseRenderer* m_pRenderer;
    HRESULT EndOfStream();
};
HRESULT CRendererInputPin::EndOfStream()
{
    CAutoLock cRendererLock(&m_pRenderer->m_InterfaceLock);
    CAutoLock cSampleLock(&m_pRenderer->m_RendererLock);
    HRESULT hr = CheckStreaming();
    if (hr != NOERROR) {
        return hr;
    }
    hr = m_pRenderer->EndOfStream();
    if (SUCCEEDED(hr)) {
        hr = CBaseInputPin::EndOfStream();
    }
    return hr;
}
int main()
{
    CBaseRenderer r; CRendererInputPin pin; pin.m_pRenderer = &r;
    int ok = 1;
    pin.streaming_hr = S_FALSE;
    ok &= pin.EndOfStream() == S_FALSE && r.eos_calls == 0 && pin.base_calls == 0;
    ok &= r.m_InterfaceLock.depth == 0 && r.m_RendererLock.depth == 0 && r.m_InterfaceLock.entered == 1;
    pin.streaming_hr = S_OK; r.eos_hr = E_FAIL;
    ok &= pin.EndOfStream() == E_FAIL && r.eos_calls == 1 && pin.base_calls == 0 && r.locks_held_in_eos;
    r.eos_hr = S_FALSE;
    ok &= pin.EndOfStream() == 0x55 && r.eos_calls == 2 && pin.base_calls == 1;
    ok &= r.m_InterfaceLock.depth == 0 && r.m_RendererLock.depth == 0 && r.m_RendererLock.entered == 3;
    if (!ok) { std::printf("00ca4f90_TEST FAIL\n"); return 1; }
    std::printf("00ca4f90_TEST PASS\n");
    return 0;
}