// CDynamicOutputPin::Active() -- DirectShow BaseClasses amfilter.cpp: E_FAIL unless SetConfigInfo set the stop event (+0xd0) and the graph config (+0xd4); reset the stop event, then CBaseOutputPin::Active (inlined): VFW_E_NO_ALLOCATOR without an allocator, else IMemAllocator::Commit
// Leaf: ?Active@CDynamicOutputPin@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA9A90, thiscall (virtual: amfilter.h overrides of CBasePin / CBaseOutputPin virtuals).
// Identity (vendor naming V2, proven): CDynamicOutputPin::Active (amfilter.cpp: the SetConfigInfo check, EXECUTE_ASSERT(ResetEvent(m_hStopEvent)), return CBaseOutputPin::Active() = m_pAllocator ? m_pAllocator->Commit() : VFW_E_NO_ALLOCATOR; IMemAllocator slot 5 Commit); V1 LABELS (strmbase vtable slot + amfilter.h declaration order) name 0x00CA9A90 the same.
//   Was ?Pause@CVideoPlayer_00ca9a90@@QAEJXZ (a propagated / descriptive stand-in; the body was already the SDK member's).
//   Members: amfilter.h names at the landed offsets (m_Connected +0x18, m_bRunTimeError +0x24, m_pAllocator +0x98, m_BlockStateLock +0xa4,
//   m_hUnblockOutputPinEvent +0xbc, m_hNotifyCallerPinBlockedEvent +0xc0, m_BlockState +0xc4, m_dwBlockCallerThreadID +0xc8,
//   m_dwNumOutstandingOutputPinUsers +0xcc, m_hStopEvent +0xd0, m_pGraphConfig +0xd4).
extern "C" __declspec(dllimport) int __stdcall ResetEvent(void* event);   // IAT 0x0143FED4

struct IMemAllocator {                   // strmif.h COM interface (slots as declared): __stdcall, `this` pushed
    virtual long __stdcall QueryInterface(); virtual long __stdcall AddRef(); virtual long __stdcall Release(); virtual long __stdcall SetProperties(); virtual long __stdcall GetProperties();
    virtual long __stdcall Commit();               // slot 5 (+0x14)
};

class CDynamicOutputPin {                         // amfilter.h (CBaseOutputPin / CDynamicOutputPin members this body reads)
public:
    unsigned char            _pad04[0x94];
    IMemAllocator*  m_pAllocator;           // +0x98
    unsigned char            _pad9c[0x34];
    void*                    m_hStopEvent;             // +0xd0
    void*                    m_pGraphConfig;     // IGraphConfig*            // +0xd4
    virtual long Active();
};

long CDynamicOutputPin::Active()
{
    if (!m_hStopEvent || !m_pGraphConfig)
        return 0x80004005;                         // E_FAIL: SetConfigInfo was never called
    ResetEvent(m_hStopEvent);                   // EXECUTE_ASSERT(ResetEvent(m_hStopEvent))
    if (!m_pAllocator)
        return 0x8004020a;                         // CBaseOutputPin::Active (inline, same TU): VFW_E_NO_ALLOCATOR
    return m_pAllocator->Commit();
}