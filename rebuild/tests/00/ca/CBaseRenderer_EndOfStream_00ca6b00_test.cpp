// Behaviour test for 0x00CA6B00 CBaseRenderer::EndOfStream (DirectShow renbase.cpp): ignored when stopped; set m_bEOS; with no pending sample signal ready and, if streaming, send EC_COMPLETE.
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
struct IMediaSample { int dummy; };
struct CBaseRenderer {
    int m_State; int m_bEOS; int m_bStreaming; IMediaSample* m_pMediaSample; CAMEvent m_evComplete; int sends;
    CBaseRenderer() : m_State(0), m_bEOS(0), m_bStreaming(0), m_pMediaSample(0), sends(0) {}
    void Ready() { m_evComplete.Set(); }
    virtual HRESULT SendEndOfStream() { sends++; return 0x33; }
    HRESULT EndOfStream();
};
HRESULT CBaseRenderer::EndOfStream()
{
    if (m_State == State_Stopped) {
        return NOERROR;
    }
    m_bEOS = 1;
    if (m_pMediaSample) {
        return NOERROR;
    }
    Ready();
    if (m_bStreaming) {
        SendEndOfStream();
    }
    return NOERROR;
}
int main()
{
    CBaseRenderer r; IMediaSample s;
    int ok = 1;
    ok &= r.EndOfStream() == NOERROR && r.m_bEOS == 0 && !r.m_evComplete.signalled;
    r.m_State = State_Paused; r.m_pMediaSample = &s;
    ok &= r.EndOfStream() == NOERROR && r.m_bEOS == 1 && !r.m_evComplete.signalled && r.sends == 0;
    r.m_bEOS = 0; r.m_pMediaSample = 0;
    ok &= r.EndOfStream() == NOERROR && r.m_bEOS == 1 && r.m_evComplete.signalled && r.sends == 0;
    r.m_State = State_Running; r.m_bStreaming = 1; r.m_evComplete.Reset();
    ok &= r.EndOfStream() == NOERROR && r.m_evComplete.signalled && r.sends == 1;
    if (!ok) { std::printf("00ca6b00_TEST FAIL\n"); return 1; }
    std::printf("00ca6b00_TEST PASS\n");
    return 0;
}