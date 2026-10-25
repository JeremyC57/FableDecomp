// Behaviour test for 0x00CA4AA0 CBaseRenderer::ScheduleSample (DirectShow renbase.cpp): render now on S_OK, otherwise ask the clock to signal the render event at the sample's start time.
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
struct IReferenceClock {
    int calls; REFERENCE_TIME base, stream; CAMEvent* ev; unsigned int* cookie; HRESULT result;
    IReferenceClock() : calls(0), base(0), stream(0), ev(0), cookie(0), result(S_OK) {}
    HRESULT AdviseTime(REFERENCE_TIME b, REFERENCE_TIME s, CAMEvent* e, unsigned int* pc) { calls++; base = b; stream = s; ev = e; cookie = pc; *pc = 99; return result; }
};
struct CBaseRenderer {
    IReferenceClock* m_pClock; REFERENCE_TIME m_tStart; CAMEvent m_RenderEvent; unsigned int m_dwAdvise;
    HRESULT times_hr; REFERENCE_TIME times_start;
    virtual HRESULT GetSampleTimes(IMediaSample*, REFERENCE_TIME* pStart, REFERENCE_TIME* pEnd) { *pStart = times_start; *pEnd = times_start + 10; return times_hr; }
    int ScheduleSample(IMediaSample* pMediaSample);
};
int CBaseRenderer::ScheduleSample(IMediaSample* pMediaSample)
{
    REFERENCE_TIME StartSample, EndSample;
    if (pMediaSample == 0) {
        return 0;
    }
    HRESULT hr = GetSampleTimes(pMediaSample, &StartSample, &EndSample);
    if (FAILED(hr)) {
        return 0;
    }
    if (hr == S_OK) {
        m_RenderEvent.Set();
        return 1;
    }
    hr = m_pClock->AdviseTime(m_tStart, StartSample, &m_RenderEvent, &m_dwAdvise);
    if (SUCCEEDED(hr)) {
        return 1;
    }
    return 0;
}
int main()
{
    IReferenceClock clock;
    CBaseRenderer r; r.m_pClock = &clock; r.m_tStart = 5000; r.m_dwAdvise = 0; r.times_start = 70000;
    IMediaSample s;
    int ok = 1;
    ok &= r.ScheduleSample(0) == 0;
    r.times_hr = E_FAIL; ok &= r.ScheduleSample(&s) == 0 && clock.calls == 0 && !r.m_RenderEvent.signalled;
    r.times_hr = S_OK; ok &= r.ScheduleSample(&s) == 1 && r.m_RenderEvent.signalled && clock.calls == 0;
    r.m_RenderEvent.Reset();
    r.times_hr = S_FALSE; ok &= r.ScheduleSample(&s) == 1 && clock.calls == 1 && clock.base == 5000 && clock.stream == 70000;
    ok &= clock.ev == &r.m_RenderEvent && clock.cookie == &r.m_dwAdvise && r.m_dwAdvise == 99 && !r.m_RenderEvent.signalled;
    clock.result = E_FAIL; ok &= r.ScheduleSample(&s) == 0 && clock.calls == 2;
    if (!ok) { std::printf("00ca4aa0_TEST FAIL\n"); return 1; }
    std::printf("00ca4aa0_TEST PASS\n");
    return 0;
}