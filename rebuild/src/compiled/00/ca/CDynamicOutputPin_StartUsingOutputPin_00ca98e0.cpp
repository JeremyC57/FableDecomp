// CDynamicOutputPin::StartUsingOutputPin() -- DirectShow BaseClasses amfilter.cpp: under m_BlockStateLock, while the pin is BLOCKED drop the lock and wait for the unblock or the stop event (VFW_E_STATE_CHANGED on stop, AmGetLastErrorToHResult on WAIT_FAILED, E_UNEXPECTED otherwise); then count one more output-pin user
// Leaf: ?StartUsingOutputPin@CDynamicOutputPin@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA98E0, thiscall (virtual: amfilter.h overrides of CBasePin / CBaseOutputPin virtuals).
// Identity (vendor naming V2, proven): CDynamicOutputPin::StartUsingOutputPin (amfilter.cpp body: CAutoLock on m_BlockStateLock, the BLOCKED wait loop on {m_hUnblockOutputPinEvent, m_hStopEvent}, m_dwNumOutstandingOutputPinUsers++); V1 LABELS (strmbase vtable slot + amfilter.h declaration order) name 0x00CA98E0 the same.
//   Was ?WaitForAsyncCompletion@CAudioThreadOwner_00ca98e0@@QAEJXZ (a propagated / descriptive stand-in; the body was already the SDK member's).
//   Members: amfilter.h names at the landed offsets (m_Connected +0x18, m_bRunTimeError +0x24, m_pAllocator +0x98, m_BlockStateLock +0xa4,
//   m_hUnblockOutputPinEvent +0xbc, m_hNotifyCallerPinBlockedEvent +0xc0, m_BlockState +0xc4, m_dwBlockCallerThreadID +0xc8,
//   m_dwNumOutstandingOutputPinUsers +0xcc, m_hStopEvent +0xd0, m_pGraphConfig +0xd4).
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForMultipleObjects(unsigned long count, void* const* handles, int waitAll, unsigned long timeout);
long LastErrorToHResult_00ca98e0();        // 0x00CB0BC0

struct CAutoLock_00ca98e0 {             // scoped critical-section holder
    void* m_pSection;
    CAutoLock_00ca98e0(void* section) : m_pSection(section) { EnterCriticalSection(m_pSection); }
    ~CAutoLock_00ca98e0() { LeaveCriticalSection(m_pSection); }
};

class CDynamicOutputPin {                  // amfilter.h (the members this body reads)
public:
    unsigned char _pad04[0xa0];
    unsigned char m_BlockStateLock[0x18];         // +0xa4
    void* m_hUnblockOutputPinEvent;                         // +0xbc
    void* m_hNotifyCallerPinBlockedEvent;                       // +0xc0
    long m_BlockState;                          // +0xc4: 2 while busy
    unsigned long m_dwBlockCallerThreadID;              // +0xc8
    long m_dwNumOutstandingOutputPinUsers;                          // +0xcc
    void* m_hStopEvent;                        // +0xd0
    virtual long StartUsingOutputPin();
};

long CDynamicOutputPin::StartUsingOutputPin()
{
    CAutoLock_00ca98e0 lock(m_BlockStateLock);
    while (m_BlockState == 2) {               // BLOCKED
        LeaveCriticalSection(m_BlockStateLock);
        void* handles[2];
        handles[0] = m_hUnblockOutputPinEvent;
        handles[1] = m_hStopEvent;
        unsigned long result = WaitForMultipleObjects(2, handles, 0, 0xffffffff);
        EnterCriticalSection(m_BlockStateLock);
        switch (result) {
        case 0:
            break;
        case 1:
            return 0x80040223;             // VFW_E_STATE_CHANGED (the stop event)
        case 0xffffffff:
            return LastErrorToHResult_00ca98e0();
        default:
            return 0x8000ffff;
        }
    }
    ++m_dwNumOutstandingOutputPinUsers;
    return 0;
}