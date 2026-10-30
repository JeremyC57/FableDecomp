// Behaviour test for CBaseRenderer::Pause (0x00CA67B0): the state shortcuts complete the state change at once; a failing CBaseFilter::Pause returns its result; otherwise the repaint flag is raised under the renderer lock, streaming stops, the source thread may wait, the advise is cancelled, the end-of-stream timer is killed, the allocator committed and, coming out of stopped, the abort flag and the pending sample cleared, before CompleteStateChange(old state).
#include <cstdio>
#if defined(_MSC_VER)
typedef __int64 i64;
#else
typedef long long i64;
#endif
static int g_lockDepth = 0, g_lockErrors = 0, g_events = 0, g_timerKills = 0;
struct CRITICAL_SECTION { int count; };
static void EnterCriticalSection(CRITICAL_SECTION* s) { ++s->count; ++g_lockDepth; }
static void LeaveCriticalSection(CRITICAL_SECTION* s) { if (s->count <= 0) ++g_lockErrors; --s->count; --g_lockDepth; }
static int SetEvent(void* event) { ++g_events; *(int*)event = 1; return 1; }
static unsigned int timeKillEvent(unsigned int id) { ++g_timerKills; return id ? 0 : 1; }
class CCritSec { public: CRITICAL_SECTION m_CritSec; void Lock() { EnterCriticalSection(&m_CritSec); } void Unlock() { LeaveCriticalSection(&m_CritSec); } };
class CAutoLock { protected: CCritSec* m_pLock; public: CAutoLock(CCritSec* plock) { m_pLock = plock; m_pLock->Lock(); } ~CAutoLock() { m_pLock->Unlock(); } };
class CAMEvent { public: void* m_hEvent; void Set() { SetEvent(m_hEvent); } };
enum FILTER_STATE { State_Stopped = 0, State_Paused = 1, State_Running = 2 };
#define NOERROR 0L
#define S_OK 0L
#define TRUE 1
#define FALSE 0
#define FAILED(hr) (((long)(hr)) < 0)
#define EC_COMPLETE 0x01
static int g_commits = 0;
struct IMemAllocator { virtual long Commit() { ++g_commits; return 0; } };
struct IPin;
struct CBasePin { IPin* m_Connected; };
struct CRendererInputPin : CBasePin { IMemAllocator* m_pAllocator; int IsConnected() { return m_Connected != 0; } IMemAllocator* Allocator() const { return m_pAllocator; } };
static int g_seq[16]; static int g_nSeq = 0;
static long g_baseResult = 0;
struct IBaseFilter { int dummy; };
class CBaseFilter : public IBaseFilter {
public:
    virtual ~CBaseFilter() {}
    FILTER_STATE m_State;
    long m_notifyCode, m_notifyParam1; const void* m_notifyParam2; int m_notifies;
    long Pause() { g_seq[g_nSeq++] = 1; if (g_baseResult >= 0) m_State = State_Paused; return g_baseResult; }
    long Run(i64 tStart) { g_seq[g_nSeq++] = 1; m_lastStart = tStart; if (g_baseResult >= 0) m_State = State_Running; return g_baseResult; }
    long NotifyEvent(long code, long p1, const void* p2) { m_notifyCode = code; m_notifyParam1 = p1; m_notifyParam2 = p2; ++m_notifies; return 0; }
    i64 m_lastStart;
};
class CBaseRenderer : public CBaseFilter {
public:
    int m_canWait, m_completed, m_cleared;
    long m_completedState;
    virtual long SourceThreadCanWait(int canWait) { g_seq[g_nSeq++] = 3; m_canWait = canWait; return 0; }
    virtual long CompleteStateChange(FILTER_STATE oldState) { g_seq[g_nSeq++] = 9; ++m_completed; m_completedState = oldState; return 0x42 + oldState; }
    virtual long CancelNotification() { g_seq[g_nSeq++] = 5; return 0; }
    virtual long ClearPendingSample() { g_seq[g_nSeq++] = 7; ++m_cleared; return 0; }
    virtual long StartStreaming() { g_seq[g_nSeq++] = 8; return 0x77; }
    virtual long StopStreaming() { g_seq[g_nSeq++] = 2; if (g_lockDepth != 1) ++g_lockErrors; return 0; }
    CAMEvent m_evComplete; int m_bAbort; CRendererInputPin* m_pInputPin; CCritSec m_InterfaceLock; CCritSec m_RendererLock;
    int m_bRepaintStatus; unsigned int m_EndOfStreamTimer;
    void SetRepaintStatus(int bRepaint) { CAutoLock cSampleLock(&m_RendererLock); if (g_lockDepth != 2) ++g_lockErrors; m_bRepaintStatus = bRepaint; g_seq[g_nSeq++] = 4; }
    void Ready() { m_evComplete.Set(); g_seq[g_nSeq++] = 6; }
    void ResetEndOfStreamTimer() { if (m_EndOfStreamTimer) { timeKillEvent(m_EndOfStreamTimer); m_EndOfStreamTimer = 0; } }
    long Pause();
    long Run(i64 StartTime);
};
static int fail(const char* addr, const char* why) { std::printf("%s_TEST FAIL: %s\n", addr, why); return 1; }
static void reset(CBaseRenderer& r, CRendererInputPin& pin, int* eventFlag)
{
    g_nSeq = 0; g_events = 0; g_commits = 0; g_timerKills = 0; g_baseResult = 0; *eventFlag = 0;
    r.m_evComplete.m_hEvent = eventFlag; r.m_pInputPin = &pin; r.m_bAbort = 1; r.m_bRepaintStatus = 7;
    r.m_InterfaceLock.m_CritSec.count = 0; r.m_RendererLock.m_CritSec.count = 0; r.m_EndOfStreamTimer = 5;
    r.m_canWait = -1; r.m_completed = 0; r.m_cleared = 0; r.m_completedState = -1; r.m_notifies = 0;
}
long CBaseRenderer::Pause()
{
    CAutoLock cRendererLock(&m_InterfaceLock);
    FILTER_STATE OldState = m_State;
    if (m_State == State_Paused) {
        return CompleteStateChange(State_Paused);
    }
    if (m_pInputPin->IsConnected() == FALSE) {
        m_State = State_Paused;
        return CompleteStateChange(State_Paused);
    }
    long hr = CBaseFilter::Pause();
    if (FAILED(hr)) {
        return hr;
    }
    SetRepaintStatus(TRUE);
    StopStreaming();
    SourceThreadCanWait(TRUE);
    CancelNotification();
    ResetEndOfStreamTimer();
    if (m_pInputPin->Allocator()) {
        m_pInputPin->Allocator()->Commit();
    }
    if (OldState == State_Stopped) {
        m_bAbort = FALSE;
        ClearPendingSample();
    }
    return CompleteStateChange(OldState);
}
long CBaseRenderer::Run(i64) { return 0; }
int main()
{
    static int eventFlag; static IMemAllocator allocator; static CRendererInputPin pin; pin.m_pAllocator = &allocator;
    static CBaseRenderer r;
    // 1. already paused: CompleteStateChange(State_Paused) only
    reset(r, pin, &eventFlag); r.m_State = State_Paused; pin.m_Connected = (IPin*)&pin;
    if (r.Pause() != 0x43 || r.m_completedState != State_Paused || g_nSeq != 1 || g_lockDepth != 0) return fail("00ca67b0", "already paused");
    // 2. unconnected: state set, CompleteStateChange(State_Paused)
    reset(r, pin, &eventFlag); r.m_State = State_Stopped; pin.m_Connected = 0;
    if (r.Pause() != 0x43 || r.m_State != State_Paused || g_nSeq != 1 || g_commits || g_lockDepth != 0) return fail("00ca67b0", "unconnected");
    // 3. base filter fails: its result returned, nothing else
    reset(r, pin, &eventFlag); r.m_State = State_Stopped; pin.m_Connected = (IPin*)&pin; g_baseResult = -5;
    if (r.Pause() != -5 || g_nSeq != 1 || r.m_completed || g_lockDepth != 0) return fail("00ca67b0", "base failure");
    // 4. stopped -> paused: the full sequence, abort cleared, pending sample cleared
    reset(r, pin, &eventFlag); r.m_State = State_Stopped; pin.m_Connected = (IPin*)&pin;
    if (r.Pause() != 0x42) return fail("00ca67b0", "return of the full path");
    static const int order[] = { 1, 4, 2, 3, 5, 7, 9 };
    if (g_nSeq != 7) return fail("00ca67b0", "call count");
    for (int i = 0; i < 7; ++i) if (g_seq[i] != order[i]) return fail("00ca67b0", "call order");
    if (r.m_bRepaintStatus != 1 || r.m_canWait != 1 || g_timerKills != 1 || r.m_EndOfStreamTimer != 0 || g_commits != 1
        || r.m_bAbort != 0 || r.m_cleared != 1 || r.m_completedState != State_Stopped || g_lockDepth != 0 || g_lockErrors) return fail("00ca67b0", "full path effects");
    // 5. running -> paused: no abort reset, no timer to kill, no allocator
    reset(r, pin, &eventFlag); r.m_State = State_Running; pin.m_Connected = (IPin*)&pin; r.m_EndOfStreamTimer = 0; pin.m_pAllocator = 0;
    if (r.Pause() != 0x44 || r.m_bAbort != 1 || r.m_cleared || g_timerKills || g_commits || r.m_completedState != State_Running) return fail("00ca67b0", "running -> paused");
    std::printf("00ca67b0_TEST PASS\n");
    return 0;
}