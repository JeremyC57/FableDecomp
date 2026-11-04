// CDynamicOutputPin::DeliverEndFlush() -- DirectShow BaseClasses amfilter.cpp: reset the stop event (+0xd0), then CBaseOutputPin::DeliverEndFlush (inlined): VFW_E_NOT_CONNECTED without a connected pin (+0x18), else IPin::EndFlush
// Leaf: ?DeliverEndFlush@CDynamicOutputPin@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA9B40, thiscall (virtual: amfilter.h overrides of CBasePin / CBaseOutputPin virtuals).
// Identity (vendor naming V2, proven): CDynamicOutputPin::DeliverEndFlush (amfilter.cpp: EXECUTE_ASSERT(ResetEvent(m_hStopEvent)); return CBaseOutputPin::DeliverEndFlush() = m_Connected ? m_Connected->EndFlush() : VFW_E_NOT_CONNECTED; IPin slot 16); V1 LABELS (strmbase vtable slot + amfilter.h declaration order) name 0x00CA9B40 the same.
//   Was ?SuspendStream@CVideoPlayer_00ca9a90@@QAEJXZ (a propagated / descriptive stand-in; the body was already the SDK member's).
//   Members: amfilter.h names at the landed offsets (m_Connected +0x18, m_bRunTimeError +0x24, m_pAllocator +0x98, m_BlockStateLock +0xa4,
//   m_hUnblockOutputPinEvent +0xbc, m_hNotifyCallerPinBlockedEvent +0xc0, m_BlockState +0xc4, m_dwBlockCallerThreadID +0xc8,
//   m_dwNumOutstandingOutputPinUsers +0xcc, m_hStopEvent +0xd0, m_pGraphConfig +0xd4).
extern "C" __declspec(dllimport) int __stdcall ResetEvent(void* event);   // IAT 0x0143FED4

struct IPin {                    // strmif.h COM interface (slots as declared): __stdcall, `this` pushed
    virtual long __stdcall QueryInterface(); virtual long __stdcall AddRef(); virtual long __stdcall Release(); virtual long __stdcall Connect();
    virtual long __stdcall ReceiveConnection(); virtual long __stdcall Disconnect(); virtual long __stdcall ConnectedTo(); virtual long __stdcall ConnectionMediaType();
    virtual long __stdcall QueryPinInfo(); virtual long __stdcall QueryDirection(); virtual long __stdcall QueryId(); virtual long __stdcall QueryAccept();
    virtual long __stdcall EnumMediaTypes(); virtual long __stdcall QueryInternalConnections(); virtual long __stdcall EndOfStream();
    virtual long __stdcall BeginFlush();           // slot 15 (+0x3c)
    virtual long __stdcall EndFlush();             // slot 16 (+0x40)
};

class CDynamicOutputPin {                         // amfilter.h (CBasePin / CDynamicOutputPin members this body reads)
public:
    unsigned char            _pad04[0x14];
    IPin*   m_Connected;            // +0x18
    unsigned char            _pad1c[0xb4];
    void*                    m_hStopEvent;             // +0xd0
    virtual long DeliverEndFlush();
};

long CDynamicOutputPin::DeliverEndFlush()
{
    ResetEvent(m_hStopEvent);
    if (!m_Connected)
        return 0x80040209;                         // CBaseOutputPin::DeliverEndFlush (inline, same TU): VFW_E_NOT_CONNECTED
    return m_Connected->EndFlush();
}