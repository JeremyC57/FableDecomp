// Behaviour test for 0x00CAACA0 CBaseFilter::Run: lock held across the body, m_tStart stored, Pause() of a stopped
// filter (failure returned), Run(tStart) on connected pins only (failure returned, state left unchanged), State_Running.
#include <cstdio>

typedef int HRESULT;      // 32-bit on every host (long is 64-bit under x64 g++)
typedef long long REFERENCE_TIME;
enum FILTER_STATE { State_Stopped = 0, State_Paused = 1, State_Running = 2 };

static int g_lockDepth, g_lockCalls;
struct CCritSec { void Lock() { ++g_lockDepth; ++g_lockCalls; } void Unlock() { --g_lockDepth; } };
struct CAutoLock {
    CCritSec* m_pLock;
    CAutoLock(CCritSec* p) { m_pLock = p; m_pLock->Lock(); }
    ~CAutoLock() { m_pLock->Unlock(); }
};

struct CBasePin {
    void* m_Connected; HRESULT m_result; int m_runs; REFERENCE_TIME m_t;
    bool IsConnected() { return m_Connected != 0; }
    virtual HRESULT Run(REFERENCE_TIME t) { ++m_runs; m_t = t; return m_result; }
    virtual ~CBasePin() {}
};

struct CBaseFilter {
    FILTER_STATE m_State; REFERENCE_TIME m_tStart; CCritSec* m_pLock;
    CBasePin* m_pins[3]; int m_count; HRESULT m_pauseResult; int m_pauses;
    virtual HRESULT Pause() { ++m_pauses; if (m_pauseResult >= 0) m_State = State_Paused; return m_pauseResult; }
    virtual int GetPinCount() { return m_count; }
    virtual CBasePin* GetPin(int n) { return m_pins[n]; }
    virtual ~CBaseFilter() {}
    HRESULT Run(REFERENCE_TIME tStart)
    {
        CAutoLock cObjectLock(m_pLock);
        m_tStart = tStart;
        if (m_State == State_Stopped) {
            HRESULT hr = Pause();
            if (hr < 0)
                return hr;
        }
        if (m_State != State_Running) {
            int cPins = GetPinCount();
            for (int c = 0; c < cPins; c++) {
                CBasePin* pPin = GetPin(c);
                if (pPin->IsConnected()) {
                    HRESULT hr = pPin->Run(tStart);
                    if (hr < 0)
                        return hr;
                }
            }
        }
        m_State = State_Running;
        return 0;
    }
};

int main()
{
    int bad = 0, dummy = 0;
    CCritSec cs;
    CBasePin a, b, c;
    a.m_Connected = &dummy; a.m_result = 0; a.m_runs = 0; a.m_t = 0;
    b.m_Connected = 0;      b.m_result = 0; b.m_runs = 0; b.m_t = 0;
    c.m_Connected = &dummy; c.m_result = 0; c.m_runs = 0; c.m_t = 0;
    CBaseFilter f;
    f.m_State = State_Stopped; f.m_tStart = 0; f.m_pLock = &cs; f.m_pins[0] = &a; f.m_pins[1] = &b; f.m_pins[2] = &c;
    f.m_count = 3; f.m_pauseResult = 0; f.m_pauses = 0;
    if (f.Run(12345678901LL) != 0 || f.m_State != State_Running || f.m_tStart != 12345678901LL || f.m_pauses != 1) bad = 1;
    if (a.m_runs != 1 || b.m_runs != 0 || c.m_runs != 1 || a.m_t != 12345678901LL || g_lockDepth != 0) bad = 1;
    // already running: no pin calls, tStart still stored
    if (f.Run(5) != 0 || a.m_runs != 1 || f.m_tStart != 5 || f.m_pauses != 1) bad = 1;
    // Pause failure is returned, state untouched
    f.m_State = State_Stopped; f.m_pauseResult = (HRESULT)0x80004005L;
    if (f.Run(7) != (HRESULT)0x80004005L || f.m_State != State_Stopped || a.m_runs != 1 || g_lockDepth != 0) bad = 1;
    // paused filter: pin failure returned at once, later pins skipped
    f.m_State = State_Paused; a.m_result = (HRESULT)0x8004020BL;
    if (f.Run(9) != (HRESULT)0x8004020BL || f.m_State != State_Paused || c.m_runs != 1 || g_lockDepth != 0) bad = 1;
    if (g_lockCalls != 4) bad = 1;
    if (bad) { std::printf("00caaca0_TEST FAIL\n"); return 1; }
    std::printf("00caaca0_TEST PASS\n");
    return 0;
}