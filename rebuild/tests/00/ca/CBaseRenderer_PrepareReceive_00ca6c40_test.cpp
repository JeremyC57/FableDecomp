// Behaviour test for CBaseRenderer::PrepareReceive 0x00CA6C40: return codes, AddRef / end-time bookkeeping, repaint and
// lock balance, with stand-in pin / sample / position objects (the body is the SDK renbase.cpp text).
#include <cstdio>

typedef int HRESULT;   // 32-bit on both compilers (long is 64-bit under g++ on Linux)
typedef int BOOL;
#define E_FAIL ((HRESULT)0x80004005)
#define E_UNEXPECTED ((HRESULT)0x8000FFFF)
#define VFW_E_SAMPLE_REJECTED ((HRESULT)0x8004022B)
#define NOERROR 0

struct Sample { int refs; Sample() : refs(1) {} void AddRef() { ++refs; } };
struct Lock { int depth; Lock() : depth(0) {} };
struct AutoLock { Lock* l; AutoLock(Lock* p) : l(p) { ++l->depth; } ~AutoLock() { --l->depth; } };
struct Props { void* pMediaType; long tStop; };
struct Pin {
    HRESULT receiveHr, setTypeHr; int setTypeCalls; Props props;
    Pin() : receiveHr(0), setTypeHr(0), setTypeCalls(0) { props.pMediaType = 0; props.tStop = 0; }
    HRESULT Receive(Sample*) { return receiveHr; }
    HRESULT SetMediaType(void*) { ++setTypeCalls; return setTypeHr; }
    Props* SampleProps() { return &props; }
};
struct Position { int calls; Position() : calls(0) {} void RegisterMediaTime(Sample*) { ++calls; } };

struct Renderer {
    Position* m_pPosition; int m_readySets; BOOL m_bAbort, m_bStreaming; Sample* m_pMediaSample; BOOL m_bEOS;
    Pin* m_pInputPin; Lock m_InterfaceLock, m_RendererLock; BOOL m_bRepaintStatus, m_bInReceive; long m_SignalTime;
    BOOL scheduleResult; int scheduleCalls; int repaintCalls;
    Renderer() : m_pPosition(0), m_readySets(0), m_bAbort(0), m_bStreaming(0), m_pMediaSample(0), m_bEOS(0), m_pInputPin(0),
                 m_bRepaintStatus(0), m_bInReceive(0), m_SignalTime(0), scheduleResult(1), scheduleCalls(0), repaintCalls(0) {}
    void Ready() { ++m_readySets; }
    BOOL ScheduleSample(Sample*) { ++scheduleCalls; return scheduleResult; }
    void SetRepaintStatus(BOOL b) { ++repaintCalls; m_bRepaintStatus = b; }
    HRESULT PrepareReceive(Sample* pMediaSample)
    {
        AutoLock cInterfaceLock(&m_InterfaceLock);
        m_bInReceive = 1;
        HRESULT hr = m_pInputPin->Receive(pMediaSample);
        if (hr != NOERROR) { m_bInReceive = 0; return E_FAIL; }
        if (m_pInputPin->SampleProps()->pMediaType) {
            hr = m_pInputPin->SetMediaType(m_pInputPin->SampleProps()->pMediaType);
            if (hr < 0) { m_bInReceive = 0; return hr; }
        }
        AutoLock cSampleLock(&m_RendererLock);
        if (m_pMediaSample || m_bEOS || m_bAbort) { Ready(); m_bInReceive = 0; return E_UNEXPECTED; }
        if (m_pPosition) m_pPosition->RegisterMediaTime(pMediaSample);
        if ((m_bStreaming == 1) && (ScheduleSample(pMediaSample) == 0)) { m_bInReceive = 0; return VFW_E_SAMPLE_REJECTED; }
        m_SignalTime = m_pInputPin->SampleProps()->tStop;
        m_pMediaSample = pMediaSample;
        m_pMediaSample->AddRef();
        if (m_bStreaming == 0) SetRepaintStatus(1);
        return NOERROR;
    }
};
static bool balanced(Renderer& r) { return r.m_InterfaceLock.depth == 0 && r.m_RendererLock.depth == 0; }
int main()
{
    bool ok = true;
    Sample s; Pin pin; Position pos; int fmt = 0;
    { Renderer r; r.m_pInputPin = &pin; pin.receiveHr = 1;                     // S_FALSE from the pin -> E_FAIL
      ok = ok && r.PrepareReceive(&s) == E_FAIL && r.m_bInReceive == 0 && balanced(r); pin.receiveHr = 0; }
    { Renderer r; r.m_pInputPin = &pin; pin.props.pMediaType = &fmt; pin.setTypeHr = (HRESULT)0x80040200;
      ok = ok && r.PrepareReceive(&s) == (HRESULT)0x80040200 && pin.setTypeCalls == 1 && balanced(r);
      pin.setTypeHr = 1;                                                        // S_FALSE is success: carry on
      ok = ok && r.PrepareReceive(&s) == NOERROR && pin.setTypeCalls == 2 && balanced(r);
      pin.props.pMediaType = 0; s.refs = 1; }
    { Renderer r; r.m_pInputPin = &pin; Sample busy; r.m_pMediaSample = &busy;  // sample already pending
      ok = ok && r.PrepareReceive(&s) == E_UNEXPECTED && r.m_readySets == 1 && r.m_bInReceive == 0 && balanced(r); }
    { Renderer r; r.m_pInputPin = &pin; r.m_bEOS = 1;
      ok = ok && r.PrepareReceive(&s) == E_UNEXPECTED && r.m_readySets == 1; }
    { Renderer r; r.m_pInputPin = &pin; r.m_bAbort = 1;
      ok = ok && r.PrepareReceive(&s) == E_UNEXPECTED && r.m_readySets == 1; }
    { Renderer r; r.m_pInputPin = &pin; r.m_pPosition = &pos; r.m_bStreaming = 1; r.scheduleResult = 0;
      ok = ok && r.PrepareReceive(&s) == VFW_E_SAMPLE_REJECTED && pos.calls == 1 && r.m_pMediaSample == 0 && s.refs == 1 && balanced(r); }
    { Renderer r; r.m_pInputPin = &pin; r.m_bStreaming = 1; pin.props.tStop = 1234;   // streaming and scheduled
      ok = ok && r.PrepareReceive(&s) == NOERROR && r.scheduleCalls == 1 && r.m_pMediaSample == &s && s.refs == 2
              && r.m_SignalTime == 1234 && r.repaintCalls == 0 && r.m_bInReceive == 1 && balanced(r); s.refs = 1; }
    { Renderer r; r.m_pInputPin = &pin; pin.props.tStop = 99;                 // not streaming: no schedule, repaint on
      ok = ok && r.PrepareReceive(&s) == NOERROR && r.scheduleCalls == 0 && r.repaintCalls == 1 && r.m_bRepaintStatus == 1
              && r.m_SignalTime == 99 && s.refs == 2 && balanced(r);
      ok = ok && r.PrepareReceive(&s) == E_UNEXPECTED && s.refs == 2; }      // second sample refused
    if (ok) std::printf("00ca6c40_TEST PASS\n"); else std::printf("00ca6c40_TEST FAIL\n");
    return ok ? 0 : 1;
}