// Behaviour test for CBasePin::ReceiveConnection 0x00CA8120: pointer checks, already-connected / not-stopped refusals, the
// CheckConnect / CheckMediaType / SetMediaType / CompleteConnect sequence, the VFW_E_TYPE_NOT_ACCEPTED mapping, reference
// counts, BreakConnect calls and lock balance, with a plain stand-in pin.
#include <cstdio>

typedef int HRESULT;   // 32-bit on both compilers
#define E_POINTER ((HRESULT)0x80004003)
#define E_FAIL ((HRESULT)0x80004005)
#define E_INVALIDARG ((HRESULT)0x80070057)
#define E_OUTOFMEMORY ((HRESULT)0x8007000E)
#define VFW_E_ALREADY_CONNECTED ((HRESULT)0x80040204)
#define VFW_E_NOT_STOPPED ((HRESULT)0x80040224)
#define VFW_E_TYPE_NOT_ACCEPTED ((HRESULT)0x8004022A)
#define VFW_E_INVALIDMEDIATYPE ((HRESULT)0x80040200)

struct Peer { int refs; Peer() : refs(1) {} void AddRef() { ++refs; } void Release() { --refs; } };
struct MediaType { int id; };
struct Pin {
    Peer* m_Connected; int lockDepth; int state; bool m_bCanReconnectWhenActive;
    HRESULT checkConnectHr, checkTypeHr, setTypeHr, completeHr; int breaks, setTypes, completes;
    Pin() : m_Connected(0), lockDepth(0), state(0), m_bCanReconnectWhenActive(false),
            checkConnectHr(0), checkTypeHr(0), setTypeHr(0), completeHr(0), breaks(0), setTypes(0), completes(0) {}
    struct AutoLock { Pin* p; AutoLock(Pin* q) : p(q) { ++p->lockDepth; } ~AutoLock() { --p->lockDepth; } };
    bool IsStopped() { return state == 0; }
    HRESULT CheckConnect(Peer*) { return checkConnectHr; }
    HRESULT CheckMediaType(const MediaType*) { return checkTypeHr; }
    HRESULT SetMediaType(const MediaType*) { ++setTypes; return setTypeHr; }
    HRESULT CompleteConnect(Peer*) { ++completes; return completeHr; }
    HRESULT BreakConnect() { ++breaks; return 0; }
    HRESULT ReceiveConnection(Peer* pConnector, const MediaType* pmt)
    {
        if (pConnector == 0) return E_POINTER;
        if (pmt == 0) return E_POINTER;
        AutoLock cObjectLock(this);
        if (m_Connected) return VFW_E_ALREADY_CONNECTED;
        if (!IsStopped() && !m_bCanReconnectWhenActive) return VFW_E_NOT_STOPPED;
        HRESULT hr = CheckConnect(pConnector);
        if (hr < 0) { BreakConnect(); return hr; }
        const MediaType* pcmt = pmt;
        hr = CheckMediaType(pcmt);
        if (hr != 0) {
            BreakConnect();
            if (hr >= 0 || hr == E_FAIL || hr == E_INVALIDARG) hr = VFW_E_TYPE_NOT_ACCEPTED;
            return hr;
        }
        m_Connected = pConnector;
        m_Connected->AddRef();
        hr = SetMediaType(pcmt);
        if (hr >= 0) {
            hr = CompleteConnect(pConnector);
            if (hr >= 0) return 0;
        }
        m_Connected->Release();
        m_Connected = 0;
        BreakConnect();
        return hr;
    }
};
int main()
{
    bool ok = true;
    Peer peer; MediaType mt = { 7 };
    { Pin p; ok = ok && p.ReceiveConnection(0, &mt) == E_POINTER && p.ReceiveConnection(&peer, 0) == E_POINTER && p.lockDepth == 0; }
    { Pin p; Peer other; p.m_Connected = &other;
      ok = ok && p.ReceiveConnection(&peer, &mt) == VFW_E_ALREADY_CONNECTED && p.breaks == 0 && p.lockDepth == 0; }
    { Pin p; p.state = 2;
      ok = ok && p.ReceiveConnection(&peer, &mt) == VFW_E_NOT_STOPPED && p.lockDepth == 0;
      p.m_bCanReconnectWhenActive = true;                                    // running but may reconnect: connects
      ok = ok && p.ReceiveConnection(&peer, &mt) == 0 && p.m_Connected == &peer && peer.refs == 2; peer.refs = 1; }
    { Pin p; p.checkConnectHr = E_OUTOFMEMORY;
      ok = ok && p.ReceiveConnection(&peer, &mt) == E_OUTOFMEMORY && p.breaks == 1 && p.m_Connected == 0; }
    { Pin p; p.checkTypeHr = 1;                                              // S_FALSE -> type not accepted
      ok = ok && p.ReceiveConnection(&peer, &mt) == VFW_E_TYPE_NOT_ACCEPTED && p.breaks == 1 && p.setTypes == 0;
      p.checkTypeHr = E_FAIL;       ok = ok && p.ReceiveConnection(&peer, &mt) == VFW_E_TYPE_NOT_ACCEPTED;
      p.checkTypeHr = E_INVALIDARG; ok = ok && p.ReceiveConnection(&peer, &mt) == VFW_E_TYPE_NOT_ACCEPTED;
      p.checkTypeHr = VFW_E_INVALIDMEDIATYPE;                                // a specific error is kept
      ok = ok && p.ReceiveConnection(&peer, &mt) == VFW_E_INVALIDMEDIATYPE && p.breaks == 4 && p.lockDepth == 0; }
    { Pin p; p.setTypeHr = E_FAIL;                                           // SetMediaType fails: undo
      ok = ok && p.ReceiveConnection(&peer, &mt) == E_FAIL && p.completes == 0 && p.m_Connected == 0 && peer.refs == 1 && p.breaks == 1; }
    { Pin p; p.completeHr = E_OUTOFMEMORY;                                   // CompleteConnect fails: undo
      ok = ok && p.ReceiveConnection(&peer, &mt) == E_OUTOFMEMORY && p.setTypes == 1 && p.m_Connected == 0 && peer.refs == 1 && p.breaks == 1; }
    { Pin p; p.setTypeHr = 1; p.completeHr = 1;                              // S_FALSE counts as success
      ok = ok && p.ReceiveConnection(&peer, &mt) == 0 && p.m_Connected == &peer && peer.refs == 2 && p.breaks == 0 && p.lockDepth == 0; }
    if (ok) std::printf("00ca8120_TEST PASS\n"); else std::printf("00ca8120_TEST FAIL\n");
    return ok ? 0 : 1;
}