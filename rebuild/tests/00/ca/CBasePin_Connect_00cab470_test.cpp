#include <cstdio>

// Behaviour test for 0x00CAB470 CBasePin::Connect (DirectShow BaseClasses): pointer check, lock held across every exit,
// already-connected and not-stopped refusals, AgreeMediaType failure -> BreakConnect + error, success -> NOERROR.
typedef int HRESULT;
struct CCritSec { int depth; int enters; void Lock() { ++depth; ++enters; } void Unlock() { --depth; } };
class CAutoLock {
    CCritSec* m_pLock;
public:
    CAutoLock(CCritSec* plock) { m_pLock = plock; m_pLock->Lock(); }
    ~CAutoLock() { m_pLock->Unlock(); }
};
enum FILTER_STATE { State_Stopped = 0, State_Paused = 1, State_Running = 2 };
struct CBaseFilter { FILTER_STATE m_State; };
struct IPin { int id; };
struct AM_MEDIA_TYPE { int major; };
class CMediaType : public AM_MEDIA_TYPE { };

class CBasePin {
public:
    IPin* m_Connected;
    CCritSec* m_pLock;
    bool m_bCanReconnectWhenActive;
    CBaseFilter* m_pFilter;
    HRESULT agreeResult;
    int agreeCalls, breakCalls, lockDepthInAgree;
    const CMediaType* lastType;
    HRESULT AgreeMediaType(IPin*, const CMediaType* pmt) { ++agreeCalls; lastType = pmt; lockDepthInAgree = m_pLock->depth; return agreeResult; }
    HRESULT BreakConnect() { ++breakCalls; return 0; }
    void DisplayPinInfo(IPin*) { }
    int IsStopped() { return (m_pFilter->m_State == State_Stopped); }
    // The graded body.
    HRESULT Connect(IPin* pReceivePin, const AM_MEDIA_TYPE* pmt)
    {
        if (pReceivePin == 0) return (HRESULT)0x80004003L;
        CAutoLock cObjectLock(m_pLock);
        DisplayPinInfo(pReceivePin);
        if (m_Connected) {
            return (HRESULT)0x80040204L;
        }
        if (!IsStopped() && !m_bCanReconnectWhenActive) {
            return (HRESULT)0x80040224L;
        }
        const CMediaType* ptype = (const CMediaType*)pmt;
        HRESULT hr = AgreeMediaType(pReceivePin, ptype);
        if (hr < 0) {
            BreakConnect();
            return hr;
        }
        return 0;
    }
};

int main()
{
    int bad = 0;
    CCritSec cs = { 0, 0 };
    CBaseFilter f; f.m_State = State_Stopped;
    IPin other = { 7 }, connected = { 9 };
    CMediaType mt;
    CBasePin p; p.m_Connected = 0; p.m_pLock = &cs; p.m_bCanReconnectWhenActive = false; p.m_pFilter = &f;
    p.agreeResult = 0; p.agreeCalls = 0; p.breakCalls = 0; p.lockDepthInAgree = 0; p.lastType = 0;

    bad |= p.Connect(0, &mt) != (HRESULT)0x80004003L || cs.enters != 0;                       // null pin: no lock
    p.m_Connected = &connected;
    bad |= p.Connect(&other, &mt) != (HRESULT)0x80040204L || cs.depth != 0 || cs.enters != 1;  // already connected
    p.m_Connected = 0; f.m_State = State_Running;
    bad |= p.Connect(&other, &mt) != (HRESULT)0x80040224L || cs.depth != 0 || p.agreeCalls != 0;   // running, no reconnect
    p.m_bCanReconnectWhenActive = true;
    bad |= p.Connect(&other, &mt) != 0 || p.agreeCalls != 1 || p.lockDepthInAgree != 1 || p.lastType != &mt;
    f.m_State = State_Stopped; p.m_bCanReconnectWhenActive = false; p.agreeResult = (HRESULT)0x80040207L;
    bad |= p.Connect(&other, 0) != (HRESULT)0x80040207L || p.breakCalls != 1 || cs.depth != 0 || p.lastType != 0;
    p.agreeResult = 1;                                                                         // S_FALSE is success
    bad |= p.Connect(&other, &mt) != 0 || p.breakCalls != 1 || cs.depth != 0;
    if (bad) { std::printf("00cab470_TEST FAIL\n"); return 1; }
    std::printf("00cab470_TEST PASS\n");
    return 0;
}