// Behaviour test for 00ca4cc0: CBaseRenderer::NotifyEndOfStream -- nothing when not streaming; else timer off, EOS to the position, delivered, EC_COMPLETE
#include <cstdio>
static int g_enter = 0, g_leave = 0, g_held = 0;
class CCritSec { public: void Lock() { ++g_enter; ++g_held; } void Unlock() { ++g_leave; --g_held; } };
class CAutoLock { CCritSec* m_pLock; public: CAutoLock(CCritSec* l) : m_pLock(l) { m_pLock->Lock(); } ~CAutoLock() { m_pLock->Unlock(); } };
static long g_evCode = -1, g_ev1 = -1, g_ev2 = -1; static int g_events = 0;
static int g_add = 0, g_rel = 0, g_eos = 0, g_send = 0, g_stopped = 0, g_period = 0, g_clear = 0;
struct IPinish { virtual unsigned long AddRef() { return ++g_add; } virtual unsigned long Release() { return ++g_rel; } };
class CRendererInputPin : public IPinish { public: void* m_Connected; unsigned char m_bFlushing;
    int IsConnected() { return m_Connected != 0; } int IsFlushing() { return m_bFlushing; } };
class CRendererPosPassThru { public: long EOS() { ++g_eos; return 0; } };
class CBaseRenderer {
public:
    CBaseRenderer() : m_pPosition(0), m_bAbort(0), m_bStreaming(0), m_bEOS(0), m_bEOSDelivered(0), m_pInputPin(0),
                      m_bRepaintStatus(1), m_EndOfStreamTimer(0) { }
    CRendererPosPassThru* m_pPosition; int m_bAbort; int m_bStreaming; int m_bEOS; int m_bEOSDelivered;
    CRendererInputPin* m_pInputPin; CCritSec m_RendererLock; int m_bRepaintStatus; unsigned int m_EndOfStreamTimer;
    long NotifyEvent(long code, long p1, long p2) { ++g_events; g_evCode = code; g_ev1 = p1; g_ev2 = p2; return 0x1234; }
    virtual long SendEndOfStream() { ++g_send; return 0; }
    virtual long OnStopStreaming() { ++g_stopped; return 0; }
    virtual long ClearPendingSample() { ++g_clear; return 0; }
    int IsEndOfStream() { return m_bEOS; }
    void SetAbortSignal(int b) { m_bAbort = b; }
    void SetRepaintStatus(int bRepaint) { CAutoLock l(&m_RendererLock); m_bRepaintStatus = bRepaint; }
    void TimerCallback(); long NotifyEndOfStream(); long StopStreaming(); void SendRepaint(); int OnDisplayChange();
};
static void timeEndPeriod(unsigned int p) { if (p == 1) ++g_period; }
long CBaseRenderer::NotifyEndOfStream()
{
    CAutoLock l(&m_RendererLock);
    if (m_bStreaming == 0) return 0;
    m_EndOfStreamTimer = 0;
    if (m_pPosition) m_pPosition->EOS();
    m_bEOSDelivered = 1;
    return NotifyEvent(1, 0, (long)this);
}
int main()
{
    CBaseRenderer r; CRendererPosPassThru pos; r.m_EndOfStreamTimer = 9;
    int ok = r.NotifyEndOfStream() == 0 && g_events == 0 && r.m_EndOfStreamTimer == 9;
    r.m_bStreaming = 1; r.m_pPosition = &pos;
    ok = ok && r.NotifyEndOfStream() == 0x1234 && g_eos == 1 && r.m_bEOSDelivered == 1 && r.m_EndOfStreamTimer == 0
         && g_evCode == 1 && g_ev1 == 0 && g_held == 0;
    std::printf(ok ? "00ca4cc0_TEST PASS\n" : "00ca4cc0_TEST FAIL\n");
    return ok ? 0 : 1;
}
