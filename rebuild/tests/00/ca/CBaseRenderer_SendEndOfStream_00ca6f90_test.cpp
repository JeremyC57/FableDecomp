// Behaviour test for 0x00CA6F90 CBaseRenderer::SendEndOfStream (DirectShow renbase.cpp): deliver EC_COMPLETE now (no clock, under 50 ms to go, or the timer failed) or arm a one-shot timer for (start + signal - now) / 10000 ms.
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
#define TIMEOUT_DELIVERYWAIT 50
#define TIMEOUT_RESOLUTION 10
#define TIME_ONESHOT 0x0000
static unsigned int g_timer_id; static unsigned int g_delay, g_res, g_kind; static void* g_user; static int g_timer_calls;
static unsigned int CompatibleTimeSetEvent(unsigned int uDelay, unsigned int uResolution, void* dwUser, unsigned int fuEvent)
{
    g_timer_calls++; g_delay = uDelay; g_res = uResolution; g_user = dwUser; g_kind = fuEvent; return g_timer_id;
}
struct IReferenceClock {
    REFERENCE_TIME now;
    virtual HRESULT GetTime(REFERENCE_TIME* p) { *p = now; return S_OK; }
};
struct CBaseRenderer {
    IReferenceClock* m_pClock; REFERENCE_TIME m_tStart; int m_bEOS; int m_bEOSDelivered; REFERENCE_TIME m_SignalTime;
    unsigned int m_EndOfStreamTimer; int notifies;
    HRESULT NotifyEndOfStream() { notifies++; m_bEOSDelivered = 1; return 0x44; }
    HRESULT SendEndOfStream();
};
HRESULT CBaseRenderer::SendEndOfStream()
{
    if (m_bEOS == 0 || m_bEOSDelivered || m_EndOfStreamTimer) {
        return NOERROR;
    }
    if (m_pClock == 0) {
        return NotifyEndOfStream();
    }
    REFERENCE_TIME Signal = m_tStart + m_SignalTime;
    REFERENCE_TIME CurrentTime;
    m_pClock->GetTime(&CurrentTime);
    int Delay = (int)((Signal - CurrentTime) / 10000);
    if (Delay < TIMEOUT_DELIVERYWAIT) {
        return NotifyEndOfStream();
    }
    m_EndOfStreamTimer = CompatibleTimeSetEvent((unsigned int)Delay, TIMEOUT_RESOLUTION, this, TIME_ONESHOT);
    if (m_EndOfStreamTimer == 0) {
        return NotifyEndOfStream();
    }
    return NOERROR;
}
int main()
{
    IReferenceClock clock; clock.now = 0;
    CBaseRenderer r; r.m_pClock = 0; r.m_tStart = (REFERENCE_TIME)10000000; r.m_bEOS = 0; r.m_bEOSDelivered = 0;
    r.m_SignalTime = (REFERENCE_TIME)5000000; r.m_EndOfStreamTimer = 0; r.notifies = 0;
    int ok = 1;
    ok &= r.SendEndOfStream() == NOERROR && r.notifies == 0;                       // no EOS pending
    r.m_bEOS = 1; ok &= r.SendEndOfStream() == 0x44 && r.notifies == 1;            // no clock: now
    ok &= r.SendEndOfStream() == NOERROR && r.notifies == 1;                       // already delivered
    r.m_bEOSDelivered = 0; r.m_pClock = &clock;
    clock.now = (REFERENCE_TIME)14600000;                                          // 40 ms to go: now
    ok &= r.SendEndOfStream() == 0x44 && r.notifies == 2 && g_timer_calls == 0;
    r.m_bEOSDelivered = 0; clock.now = (REFERENCE_TIME)13000000; g_timer_id = 77; // 200 ms to go: timer
    ok &= r.SendEndOfStream() == NOERROR && r.notifies == 2 && g_timer_calls == 1 && r.m_EndOfStreamTimer == 77;
    ok &= g_delay == 200 && g_res == 10 && g_user == &r && g_kind == 0;
    ok &= r.SendEndOfStream() == NOERROR && g_timer_calls == 1;                    // timer already armed
    r.m_EndOfStreamTimer = 0; g_timer_id = 0;                                      // the timer cannot be set: now
    ok &= r.SendEndOfStream() == 0x44 && r.notifies == 3 && g_timer_calls == 2;
    if (!ok) { std::printf("00ca6f90_TEST FAIL\n"); return 1; }
    std::printf("00ca6f90_TEST PASS\n");
    return 0;
}