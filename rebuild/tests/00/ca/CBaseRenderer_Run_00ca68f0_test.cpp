// Behaviour test for CBaseRenderer::Run (0x00CA68F0): a running filter returns at once; an unconnected one signals EC_COMPLETE with its IBaseFilter pointer and is set running; otherwise Ready() then CBaseFilter::Run (a failure returned), the source thread may wait, the repaint flag is cleared, the allocator committed, and coming out of stopped the abort flag and the pending sample cleared; StartStreaming's result is returned.
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
long CBaseRenderer::Pause() { return 0; }
long CBaseRenderer::Run(i64 StartTime)
{
    CAutoLock cRendererLock(&m_InterfaceLock);
    FILTER_STATE OldState = m_State;
    if (m_State == State_Running) {
        return NOERROR;
    }
    if (m_pInputPin->IsConnected() == FALSE) {
        NotifyEvent(EC_COMPLETE,S_OK,(IBaseFilter *)this);
        m_State = State_Running;
        return NOERROR;
    }
    Ready();
    long hr = CBaseFilter::Run(StartTime);
    if (FAILED(hr)) {
        return hr;
    }
    SourceThreadCanWait(TRUE);
    SetRepaintStatus(FALSE);
    if (m_pInputPin->Allocator()) {
        m_pInputPin->Allocator()->Commit();
    }
    if (OldState == State_Stopped) {
        m_bAbort = FALSE;
        ClearPendingSample();
    }
    return StartStreaming();
}
int main()
{
    static int eventFlag; static IMemAllocator allocator; static CRendererInputPin pin; pin.m_pAllocator = &allocator;
    static CBaseRenderer r;
    // 1. already running: nothing
    reset(r, pin, &eventFlag); r.m_State = State_Running; pin.m_Connected = (IPin*)&pin;
    if (r.Run(10) != 0 || g_nSeq != 0 || g_events || g_lockDepth != 0) return fail("00ca68f0", "already running");
    // 2. unconnected: EC_COMPLETE with the IBaseFilter pointer, state running
    reset(r, pin, &eventFlag); r.m_State = State_Paused; pin.m_Connected = 0;
    if (r.Run(10) != 0 || r.m_State != State_Running || r.m_notifies != 1 || r.m_notifyCode != EC_COMPLETE || r.m_notifyParam1 != 0
        || r.m_notifyParam2 != (const void*)(IBaseFilter*)&r || g_nSeq != 0 || g_lockDepth != 0) return fail("00ca68f0", "unconnected");
    // 3. base filter fails after Ready
    reset(r, pin, &eventFlag); r.m_State = State_Paused; pin.m_Connected = (IPin*)&pin; g_baseResult = -7;
    if (r.Run(10) != -7 || g_nSeq != 2 || eventFlag != 1 || g_commits || g_lockDepth != 0) return fail("00ca68f0", "base failure");
    // 4. stopped -> running: full sequence, StartStreaming's result returned
    reset(r, pin, &eventFlag); r.m_State = State_Stopped; pin.m_Connected = (IPin*)&pin;
    if (r.Run(1234) != 0x77 || r.m_lastStart != 1234) return fail("00ca68f0", "return of the full path");
    static const int order[] = { 6, 1, 3, 4, 7, 8 };
    if (g_nSeq != 6) return fail("00ca68f0", "call count");
    for (int i = 0; i < 6; ++i) if (g_seq[i] != order[i]) return fail("00ca68f0", "call order");
    if (r.m_canWait != 1 || r.m_bRepaintStatus != 0 || g_commits != 1 || r.m_bAbort != 0 || r.m_cleared != 1 || g_lockDepth != 0 || g_lockErrors) return fail("00ca68f0", "full path effects");
    // 5. paused -> running: no abort reset / pending-sample clear
    reset(r, pin, &eventFlag); r.m_State = State_Paused; pin.m_Connected = (IPin*)&pin;
    if (r.Run(5) != 0x77 || r.m_bAbort != 1 || r.m_cleared) return fail("00ca68f0", "paused -> running");
    std::printf("00ca68f0_TEST PASS\n");
    return 0;
}