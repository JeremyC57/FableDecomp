// CDynamicOutputPin::Inactive() -- DirectShow BaseClasses amfilter.cpp: signal the stop event (+0xd0), then CBaseOutputPin::Inactive (inlined): clear m_bRunTimeError, VFW_E_NO_ALLOCATOR without an allocator, else IMemAllocator::Decommit
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/amfilter.cpp) ?Inactive@CDynamicOutputPin@@UAEJXZ -- 45 bytes verified against Fable.exe 0x00CA9AE0
// Leaf: ?Inactive@CDynamicOutputPin@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA9AE0, thiscall (virtual: amfilter.h overrides of CBasePin / CBaseOutputPin virtuals).
// Identity (vendor naming V2, proven): CDynamicOutputPin::Inactive (amfilter.cpp: EXECUTE_ASSERT(SetEvent(m_hStopEvent)); return CBaseOutputPin::Inactive() = m_bRunTimeError = FALSE, then Decommit; IMemAllocator slot 6); V1 LABELS (strmbase vtable slot + amfilter.h declaration order) name 0x00CA9AE0 the same.
//   Was ?Resume@CVideoPlayer_00ca9a90@@QAEJXZ (a propagated / descriptive stand-in; the body was already the SDK member's).
//   Members: amfilter.h names at the landed offsets (m_Connected +0x18, m_bRunTimeError +0x24, m_pAllocator +0x98, m_BlockStateLock +0xa4,
//   m_hUnblockOutputPinEvent +0xbc, m_hNotifyCallerPinBlockedEvent +0xc0, m_BlockState +0xc4, m_dwBlockCallerThreadID +0xc8,
//   m_dwNumOutstandingOutputPinUsers +0xcc, m_hStopEvent +0xd0, m_pGraphConfig +0xd4).
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);     // IAT 0x0143FDE0

struct IMemAllocator {                   // strmif.h COM interface (slots as declared): __stdcall, `this` pushed
    virtual long __stdcall QueryInterface(); virtual long __stdcall AddRef(); virtual long __stdcall Release(); virtual long __stdcall SetProperties(); virtual long __stdcall GetProperties();
    virtual long __stdcall Commit();               // slot 5 (+0x14)
    virtual long __stdcall Decommit();             // slot 6 (+0x18)
};

class CDynamicOutputPin {                         // amfilter.h (CBasePin / CBaseOutputPin / CDynamicOutputPin members this body reads)
public:
    unsigned char            _pad04[0x20];
    bool                     m_bRunTimeError;      // +0x24 CBasePin (BOOL in the SDK; retail stores one byte)
    unsigned char            _pad25[0x73];
    IMemAllocator*  m_pAllocator;           // +0x98
    unsigned char            _pad9c[0x34];
    void*                    m_hStopEvent;             // +0xd0
    void*                    m_pGraphConfig;            // +0xd4
    virtual long Inactive();
};

long CDynamicOutputPin::Inactive()
{
    SetEvent(m_hStopEvent);
    m_bRunTimeError = false;
    if (!m_pAllocator)
        return 0x8004020a;
    return m_pAllocator->Decommit();
}