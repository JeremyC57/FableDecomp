// CBaseRenderer::ScheduleSample(IMediaSample* pMediaSample) -- DirectShow BaseClasses renbase.cpp: get the sample's times; render now (set m_RenderEvent) on S_OK, else ask the reference clock to signal m_RenderEvent at the sample's start time
// Leaf: ?ScheduleSample@CBaseRenderer@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA4AA0, 124 bytes (+4 int3). __fastcall (primary slot 21), `ret 4`.
// Callees: GetSampleTimes (primary slot 22, `call [edx+58h]`), SetEvent (KERNEL32 IAT 0x0143FDE0), IReferenceClock::AdviseTime
//   (slot 4, `call [ebx+10h]`).
// Layout: CBaseFilter m_pClock +0x18, m_tStart +0x20; CBaseRenderer m_RenderEvent +0x54, m_dwAdvise +0x68.
// Shape: renbase.cpp verbatim; the two REFERENCE_TIME locals fill the 0x10-byte frame, `FAILED(hr)` and `pMediaSample == NULL`
//   share the `return FALSE` block, `if (SUCCEEDED(hr)) return TRUE; return FALSE;` folds into `setge dl`.
// Label: manifest "Audio_ProcessEventWithCallback" is a descriptive placeholder; the real name is the strmbase member.
// ---- strmbase model (DirectShow BaseClasses, DX9 SDK era: combase.h / wxutil.h; UNICODE, release build) ----
typedef struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; } GUID;
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int n);
#pragma intrinsic(memcmp)
inline int IsEqualGUID(const GUID& a, const GUID& b) { return !memcmp(&a, &b, sizeof(GUID)); }
inline int operator==(const GUID& a, const GUID& b) { return IsEqualGUID(a, b); }
typedef __int64 REFERENCE_TIME;
typedef __int64 LONGLONG;
#define NOERROR 0L
#define S_OK 0L
#define S_FALSE 1L
#define E_POINTER ((long)0x80004003L)
#define E_OUTOFMEMORY ((long)0x8007000EL)
struct IUnknown {
    virtual long __stdcall QueryInterface(const GUID& riid, void** ppv) = 0;   // 0
    virtual unsigned long __stdcall AddRef() = 0;                              // 1 (+0x04)
    virtual unsigned long __stdcall Release() = 0;                             // 2 (+0x08)
};
struct INonDelegatingUnknown {
    virtual long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};
struct CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE10
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);                              // IAT 0x0143FDE0
extern "C" __declspec(dllimport) int __stdcall ResetEvent(void* event);                            // IAT 0x0143FED4
class CCritSec {                                 // wxutil.h
    CRITICAL_SECTION m_CritSec;
public:
    void Lock() { EnterCriticalSection(&m_CritSec); }
    void Unlock() { LeaveCriticalSection(&m_CritSec); }
};
class CAutoLock {                                // wxutil.h
protected:
    CCritSec* m_pLock;
public:
    CAutoLock(CCritSec* plock) { m_pLock = plock; m_pLock->Lock(); }
    ~CAutoLock() { m_pLock->Unlock(); }
};
class CAMEvent {                                 // wxutil.h (holds only the HANDLE)
    void* m_hEvent;
public:
    operator void*() const { return m_hEvent; }
    void Set() { SetEvent(m_hEvent); }
    void Reset() { ResetEvent(m_hEvent); }
};
#define CheckPointer(p, ret) { if ((p) == 0) return (ret); }
class CUnknown : public INonDelegatingUnknown {  // combase.h (CBaseObject is empty in release)
public:
    IUnknown* m_pUnknown;                        // +0x04 (const LPUNKNOWN)
    long m_cRef;                                 // +0x08
    long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv);   // 0x00CACCB0
    virtual void CUnknown_v03();                 // primary slot 3 (the virtual destructor)
};
class CRefTime {                                 // reftime.h: exactly one REFERENCE_TIME, no virtuals
public:
    REFERENCE_TIME m_time;
    inline CRefTime() { m_time = 0; }
    inline CRefTime(REFERENCE_TIME rt) { m_time = rt; }
    inline operator REFERENCE_TIME() const { return m_time; }
    inline CRefTime& operator=(const CRefTime& rt) { m_time = rt.m_time; return *this; }
    inline CRefTime& operator=(const LONGLONG ll) { m_time = ll; return *this; }
};
struct IReferenceClock : IUnknown {
    virtual long __stdcall GetTime(REFERENCE_TIME* pTime) = 0;                                  // 3 (+0x0c)
    virtual long __stdcall AdviseTime(REFERENCE_TIME baseTime, REFERENCE_TIME streamTime,
                                      unsigned long hEvent, unsigned long* pdwAdviseCookie) = 0; // 4 (+0x10) HEVENT
};
struct IPin;
struct IQualityControl;
struct IEnumPins;
struct IFilterGraph : IUnknown { };
struct IMediaEventSink : IUnknown { };
struct IMediaSample : IUnknown { };
struct FILTER_INFO;
struct AMOVIESETUP_FILTER;
class CBasePin;
class CMediaType;
struct IPersist : IUnknown {
    virtual long __stdcall GetClassID(GUID* clsid) = 0;                        // 3
};
struct IMediaFilter : IPersist {
    virtual long __stdcall Stop() = 0;                                         // 4
    virtual long __stdcall Pause() = 0;                                        // 5
    virtual long __stdcall Run(REFERENCE_TIME start) = 0;                      // 6
    virtual long __stdcall GetState(unsigned long dwMSecs, long* State) = 0;   // 7
    virtual long __stdcall SetSyncSource(IReferenceClock* clock) = 0;          // 8
    virtual long __stdcall GetSyncSource(IReferenceClock** clock) = 0;         // 9
};
struct IBaseFilter : IMediaFilter {
    virtual long __stdcall EnumPins(IEnumPins** ppEnum) = 0;                   // 10
    virtual long __stdcall FindPin(const unsigned short* Id, IPin** ppPin) = 0;   // 11
    virtual long __stdcall QueryFilterInfo(FILTER_INFO* pInfo) = 0;            // 12
    virtual long __stdcall JoinFilterGraph(IFilterGraph* pGraph, const unsigned short* pName) = 0;   // 13
    virtual long __stdcall QueryVendorInfo(unsigned short** pVendorInfo) = 0;  // 14
};
struct IAMovieSetup : IUnknown {
    virtual long __stdcall Register() = 0;       // 3
    virtual long __stdcall Unregister() = 0;     // 4
};
class CBaseFilter : public CUnknown, public IBaseFilter, public IAMovieSetup {   // amfilter.h: IBaseFilter +0x0C, IAMovieSetup +0x10
public:
    long m_State;                                // +0x14 FILTER_STATE (0 Stopped, 1 Paused, 2 Running)
    IReferenceClock* m_pClock;                   // +0x18
    CRefTime m_tStart;                           // +0x20
    GUID m_clsid;                                // +0x28
    CCritSec* m_pLock;                           // +0x38
    unsigned short* m_pName;                     // +0x3c WCHAR*
    IFilterGraph* m_pGraph;                      // +0x40
    IMediaEventSink* m_pSink;                    // +0x44
    long m_PinVersion;                           // +0x48
    // DECLARE_IUNKNOWN
    long __stdcall QueryInterface(const GUID& riid, void** ppv);
    unsigned long __stdcall AddRef();
    unsigned long __stdcall Release();
    long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv);   // 0x00CA7500
    long __stdcall JoinFilterGraph(IFilterGraph* pGraph, const unsigned short* pName);   // 0x00CA7850
    virtual long StreamTime(CRefTime& rtStream);         // primary slot 4
    virtual long GetPinVersion();                        // 5
    virtual int GetPinCount() = 0;                       // 6
    virtual CBasePin* GetPin(int n) = 0;                 // 7 (+0x1c)
    virtual AMOVIESETUP_FILTER* GetSetupData();          // 8
    long NotifyEvent(long EventCode, long EventParam1, long EventParam2);   // 0x00CA7930
};
class CRendererInputPin;
class CBaseRenderer : public CBaseFilter {      // renbase.h; 0xE0 bytes
public:
    void* m_pPosition;                           // +0x50 CRendererPosPassThru*
    CAMEvent m_RenderEvent;                      // +0x54
    CAMEvent m_ThreadSignal;                     // +0x58
    CAMEvent m_evComplete;                       // +0x5c
    int m_bAbort;                                // +0x60
    int m_bStreaming;                            // +0x64
    unsigned long m_dwAdvise;                    // +0x68 DWORD_PTR
    IMediaSample* m_pMediaSample;                // +0x6c
    int m_bEOS;                                  // +0x70
    int m_bEOSDelivered;                         // +0x74
    CRendererInputPin* m_pInputPin;              // +0x78
    CCritSec m_InterfaceLock;                    // +0x7c
    CCritSec m_RendererLock;                     // +0x94
    IQualityControl* m_pQSink;                   // +0xac
    int m_bRepaintStatus;                        // +0xb0
    int m_bInReceive;                            // +0xb4 (cv-qualified BOOL in renbase.h; modelled plain)
    REFERENCE_TIME m_SignalTime;                 // +0xb8
    unsigned int m_EndOfStreamTimer;             // +0xc0
    CCritSec m_ObjectCreationLock;               // +0xc4

    // primary-vtable virtuals in renbase.h declaration order (slots 9..44)
    virtual long GetMediaPositionInterface(const GUID& riid, void** ppv);   // 9 (+0x24)
    long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv);
    virtual long SourceThreadCanWait(int bCanWait);                         // 10 (+0x28)
    virtual long WaitForRenderTime();                                       // 11 (+0x2c)
    virtual long CompleteStateChange(long OldState);                        // 12 (+0x30)
    int IsEndOfStream() { return m_bEOS; }
    virtual void OnReceiveFirstSample(IMediaSample* pMediaSample);          // 13 (+0x34)
    void Ready() { m_evComplete.Set(); }
    void NotReady() { m_evComplete.Reset(); }
    long GetRealState() { return m_State; }
    void SetRepaintStatus(int bRepaint);                                    // 0x00CA4EA0
    virtual void OnRenderStart(IMediaSample* pMediaSample);                 // 14 (+0x38)
    virtual void OnRenderEnd(IMediaSample* pMediaSample);                   // 15 (+0x3c)
    virtual long OnStartStreaming();                                        // 16 (+0x40)
    virtual long OnStopStreaming();                                         // 17 (+0x44)
    virtual void OnWaitStart();                                             // 18 (+0x48)
    virtual void OnWaitEnd();                                               // 19 (+0x4c)
    virtual void PrepareRender();                                           // 20 (+0x50)
    virtual int ScheduleSample(IMediaSample* pMediaSample);                 // 21 (+0x54)
    virtual long GetSampleTimes(IMediaSample* pMediaSample, REFERENCE_TIME* pStartTime,
                                REFERENCE_TIME* pEndTime);                  // 22 (+0x58)
    virtual long ShouldDrawSampleNow(IMediaSample* pMediaSample, REFERENCE_TIME* ptrStart,
                                     REFERENCE_TIME* ptrEnd);               // 23 (+0x5c)
    long NotifyEndOfStream();                                               // 0x00CA4CC0
    virtual long SendEndOfStream();                                         // 24 (+0x60)
    virtual long ResetEndOfStream();                                        // 25 (+0x64)
    virtual long EndOfStream();                                             // 26 (+0x68)
    void SignalTimerFired() { m_dwAdvise = 0; }
    virtual long CancelNotification();                                      // 27 (+0x6c)
    virtual long ClearPendingSample();                                      // 28 (+0x70)
    virtual long Active();                                                  // 29 (+0x74)
    virtual long Inactive();                                                // 30 (+0x78)
    virtual long StartStreaming();                                          // 31 (+0x7c)
    virtual long StopStreaming();                                           // 32 (+0x80)
    virtual long BeginFlush();                                              // 33 (+0x84)
    virtual long EndFlush();                                                // 34 (+0x88)
    virtual long BreakConnect();                                            // 35 (+0x8c)
    virtual long SetMediaType(const CMediaType* pmt);                       // 36 (+0x90)
    virtual long CompleteConnect(IPin* pReceivePin);                        // 37 (+0x94)
    virtual long PrepareReceive(IMediaSample* pMediaSample);                // 38 (+0x98)
    virtual long Receive(IMediaSample* pMediaSample);                       // 39 (+0x9c)
    virtual int HaveCurrentSample();                                        // 40 (+0xa0)
    virtual IMediaSample* GetCurrentSample();                               // 41 (+0xa4)
    virtual long Render(IMediaSample* pMediaSample);                        // 42 (+0xa8)
    virtual long DoRenderSample(IMediaSample* pMediaSample) = 0;            // 43 (+0xac)
    virtual long CheckMediaType(const CMediaType* pmt) = 0;                 // 44 (+0xb0)
    void WaitForReceiveToComplete();                                        // 0x00CA47D0
};

#define SUCCEEDED(hr) (((long)(hr)) >= 0)
#define FAILED(hr) (((long)(hr)) < 0)
#define EXECUTE_ASSERT(_x_) _x_

int CBaseRenderer::ScheduleSample(IMediaSample* pMediaSample)
{
    REFERENCE_TIME StartSample, EndSample;

    // Is someone pulling our leg

    if (pMediaSample == 0) {
        return 0;
    }

    // Get the next sample due up for rendering.  If there aren't any ready
    // then GetNextSampleTimes returns an error.  If there is one to be done
    // then it succeeds and yields the sample times. If it is due now then
    // it returns S_OK other if it's to be done when due it returns S_FALSE

    long hr = GetSampleTimes(pMediaSample, &StartSample, &EndSample);
    if (FAILED(hr)) {
        return 0;
    }

    // If we don't have a reference clock then we cannot set up the advise
    // time so we simply set the event indicating an image to render. This
    // will cause us to run flat out without any timing or synchronisation

    if (hr == S_OK) {
        EXECUTE_ASSERT(SetEvent((void*)m_RenderEvent));
        return 1;
    }

    // We do have a valid reference clock interface so we can ask it to
    // set an event when the image comes due for rendering. We pass in
    // the reference time we were told to start at and also the current
    // stream time which is the offset from the start reference time

    hr = m_pClock->AdviseTime(
            (REFERENCE_TIME)m_tStart,           // Start run time
            StartSample,                        // Stream time
            (unsigned long)(void*)m_RenderEvent,   // Render notification (HEVENT)(HANDLE)
            &m_dwAdvise);                       // Advise cookie

    if (SUCCEEDED(hr)) {
        return 1;
    }

    // We could not schedule the next sample for rendering despite the fact
    // we have a valid sample here. This is a fair indication that either
    // the system clock is wrong or the time stamp for the sample is duff

    return 0;
}