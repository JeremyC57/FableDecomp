// Behaviour test for CBaseRenderer::Stop (0x00CA66D0): under the interface lock a stopped filter is left
// alone; an unconnected one only moves to State_Stopped; otherwise the base filter stops, the input
// allocator is decommitted, the repaint flag is raised under the renderer lock, streaming stops, the source
// thread is told not to wait, end-of-stream and the clock advise are reset, the completion event is set, the
// receive is waited for and the abort flag cleared. Lock nesting is checked with a balance counter.
#include <cstdio>
static int g_lockDepth = 0, g_maxDepth = 0, g_lockErrors = 0, g_events = 0;
struct CRITICAL_SECTION { int count; };
static void EnterCriticalSection(CRITICAL_SECTION* s) { ++s->count; if (++g_lockDepth > g_maxDepth) g_maxDepth = g_lockDepth; }
static void LeaveCriticalSection(CRITICAL_SECTION* s) { if (s->count <= 0) ++g_lockErrors; --s->count; --g_lockDepth; }
static int SetEvent(void* event) { ++g_events; *(int*)event = 1; return 1; }
class CCritSec { public: CRITICAL_SECTION m_CritSec; void Lock() { EnterCriticalSection(&m_CritSec); } void Unlock() { LeaveCriticalSection(&m_CritSec); } };
class CAutoLock { protected: CCritSec* m_pLock; public: CAutoLock(CCritSec* plock) { m_pLock = plock; m_pLock->Lock(); } ~CAutoLock() { m_pLock->Unlock(); } };
class CAMEvent { public: void* m_hEvent; void Set() { SetEvent(m_hEvent); } };
static int g_decommits = 0;
struct IMemAllocator { virtual long Decommit() { ++g_decommits; return 0; } };
struct IPin;
struct CBasePin { unsigned char _pad00[0x18]; IPin* m_Connected; };
struct CRendererInputPin : CBasePin { unsigned char _pad1c[0x9c - 0x1c]; IMemAllocator* m_pAllocator; int IsConnected() { return m_Connected != 0; } IMemAllocator* Allocator() const { return m_pAllocator; } };
static int g_baseStops = 0;
static int g_seq[8]; static int g_nSeq = 0;
class CBaseFilter {
public:
    virtual void v00() {}
    long m_State;
    long Stop() { ++g_baseStops; g_seq[g_nSeq++] = 1; m_State = 0; return 0; }
};
class CBaseRenderer : public CBaseFilter {
public:
    virtual long SourceThreadCanWait(int canWait) { g_seq[g_nSeq++] = 3; lastCanWait = canWait; return 0; }
    virtual long ResetEndOfStream() { g_seq[g_nSeq++] = 4; return 0; }
    virtual long CancelNotification() { g_seq[g_nSeq++] = 5; return 0; }
    virtual long StopStreaming() { g_seq[g_nSeq++] = 2; if (g_lockDepth != 1) ++g_lockErrors; return 0; }
    int lastCanWait; int waits;
    CAMEvent m_evComplete; int m_bAbort; CRendererInputPin* m_pInputPin; CCritSec m_InterfaceLock; CCritSec m_RendererLock; int m_bRepaintStatus;
    void SetRepaintStatus(int repaint) { CAutoLock cRendererLock(&m_RendererLock); if (g_lockDepth != 2) ++g_lockErrors; m_bRepaintStatus = repaint; }
    void Ready() { m_evComplete.Set(); }
    void WaitForReceiveToComplete() { ++waits; g_seq[g_nSeq++] = 6; }
    long Stop();
};
long CBaseRenderer::Stop()
{
    CAutoLock cRendererLock(&m_InterfaceLock);
    if (m_State == 0) {
        return 0;
    }
    if (m_pInputPin->IsConnected() == 0) {
        m_State = 0;
        return 0;
    }
    CBaseFilter::Stop();
    if (m_pInputPin->Allocator()) {
        m_pInputPin->Allocator()->Decommit();
    }
    SetRepaintStatus(1);
    StopStreaming();
    SourceThreadCanWait(0);
    ResetEndOfStream();
    CancelNotification();
    Ready();
    WaitForReceiveToComplete();
    m_bAbort = 0;
    return 0;
}
static int fail(const char* why) { std::printf("00ca66d0_TEST FAIL: %s\n", why); return 1; }
int main()
{
    static int eventFlag = 0; static IMemAllocator allocator; static CRendererInputPin pin; pin.m_pAllocator = &allocator;
    static CBaseRenderer r; r.m_evComplete.m_hEvent = &eventFlag; r.m_pInputPin = &pin; r.m_bAbort = 1; r.m_bRepaintStatus = 0; r.waits = 0; r.lastCanWait = 1;
    r.m_InterfaceLock.m_CritSec.count = 0; r.m_RendererLock.m_CritSec.count = 0;
    // 1. already stopped: only the interface lock is taken and released
    r.m_State = 0; pin.m_Connected = (IPin*)&pin;
    if (r.Stop() != 0 || g_baseStops || g_decommits || r.m_bAbort != 1 || r.m_InterfaceLock.m_CritSec.count != 0 || g_lockDepth != 0) return fail("stopped filter touched");
    // 2. running but unconnected: state only
    r.m_State = 2; pin.m_Connected = 0;
    if (r.Stop() != 0 || r.m_State != 0 || g_baseStops || g_decommits || r.m_bAbort != 1 || g_lockDepth != 0) return fail("unconnected path");
    // 3. running and connected: the full sequence
    r.m_State = 2; pin.m_Connected = (IPin*)&pin;
    if (r.Stop() != 0) return fail("return");
    if (g_baseStops != 1 || g_decommits != 1 || r.m_bRepaintStatus != 1 || r.lastCanWait != 0 || g_events != 1 || eventFlag != 1 || r.waits != 1 || r.m_bAbort != 0 || r.m_State != 0) return fail("full stop effects");
    static const int expected[6] = { 1, 2, 3, 4, 5, 6 };
    if (g_nSeq != 6) return fail("sequence length");
    for (int i = 0; i < 6; ++i) if (g_seq[i] != expected[i]) return fail("sequence order");
    if (g_lockErrors || g_lockDepth != 0 || g_maxDepth != 2 || r.m_InterfaceLock.m_CritSec.count != 0 || r.m_RendererLock.m_CritSec.count != 0) return fail("lock discipline");
    // 4. no allocator on the pin: no decommit, everything else still runs
    r.m_State = 1; pin.m_pAllocator = 0; r.m_bAbort = 1;
    r.Stop();
    if (g_decommits != 1 || g_baseStops != 2 || r.m_bAbort != 0 || r.waits != 2) return fail("no-allocator path");
    std::printf("00ca66d0_TEST PASS\n");
    return 0;
}