// CDynamicOutputPin::StreamingThreadUsingOutputPin() -- DirectShow BaseClasses amfilter.cpp: under m_BlockStateLock, whether any streaming thread is using the output pin (m_dwNumOutstandingOutputPinUsers > 0)
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/amfilter.cpp) ?StreamingThreadUsingOutputPin@CDynamicOutputPin@@UAE_NXZ -- 42 bytes verified against Fable.exe 0x00CA9A40
// Leaf: ?StreamingThreadUsingOutputPin@CDynamicOutputPin@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA9A40, thiscall (virtual: amfilter.h overrides of CBasePin / CBaseOutputPin virtuals).
// Identity (vendor naming V2, proven): CDynamicOutputPin::StreamingThreadUsingOutputPin (amfilter.cpp: CAutoLock on m_BlockStateLock; return m_dwNumOutstandingOutputPinUsers > 0); V1 LABELS (strmbase vtable slot + amfilter.h declaration order) name 0x00CA9A40 the same.
//   Was ?HasPending@CLockedQueue_00ca9a40@@QAE_NXZ (a propagated / descriptive stand-in; the body was already the SDK member's).
//   Members: amfilter.h names at the landed offsets (m_Connected +0x18, m_bRunTimeError +0x24, m_pAllocator +0x98, m_BlockStateLock +0xa4,
//   m_hUnblockOutputPinEvent +0xbc, m_hNotifyCallerPinBlockedEvent +0xc0, m_BlockState +0xc4, m_dwBlockCallerThreadID +0xc8,
//   m_dwNumOutstandingOutputPinUsers +0xcc, m_hStopEvent +0xd0, m_pGraphConfig +0xd4).
struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION* cs);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION* cs);   // IAT 0x0143FE10

class CDynamicOutputPin {                         // amfilter.h (the members this body reads)
public:
    unsigned char         _pad04[0xa0];
    _RTL_CRITICAL_SECTION m_BlockStateLock;               // +0xa4
    unsigned char         _padbc[0x10];
    unsigned long         m_dwNumOutstandingOutputPinUsers;          // +0xcc
    virtual bool StreamingThreadUsingOutputPin();
};

bool CDynamicOutputPin::StreamingThreadUsingOutputPin()
{
    EnterCriticalSection(&m_BlockStateLock);
    bool result = m_dwNumOutstandingOutputPinUsers > 0;
    LeaveCriticalSection(&m_BlockStateLock);
    return result;
}