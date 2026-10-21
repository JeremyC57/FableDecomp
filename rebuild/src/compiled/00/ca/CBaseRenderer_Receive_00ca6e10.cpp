// CBaseRenderer::Receive(IMediaSample* sample) -- DirectShow base class (strmbase renbase.cpp): prepare the sample (a rejected one is not an error); while paused, render-prepare, then under both locks hand over the first sample and signal completion; wait for the render time, then under both locks render the pending sample, clear it, send end-of-stream and cancel the clock advise
// Leaf: Receive
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA6E10, 363 bytes. Virtual (primary slot 39), `ret 4`, /EHsc frame (handler
//         0x01216B30).
// Class: catalog "CTextureRenderer::vfunc_39" -- slot 39 of CTextureRenderer's primary vtable, inherited strmbase
//        CBaseRenderer::Receive (the body is the SDK source line for line). VIDEO LAYER (DirectShow strmbase, replacement
//        candidate).
// Callees: EnterCriticalSection / LeaveCriticalSection through the IAT (0x0143FE0C / 0x0143FE10, held in ebx / ebp), SetEvent
//          through the IAT 0x0143FDE0 (the inline CAMEvent::Set of Ready()); primary-vtable virtuals PrepareReceive (slot 38),
//          PrepareRender (20), OnReceiveFirstSample (13), WaitForRenderTime (11), Render (42), ClearPendingSample (28),
//          SendEndOfStream (24), CancelNotification (27) -- slot numbers as the landed CBaseRenderer::Stop 0x00CA66D0 model.
// Shape: `hr == VFW_E_SAMPLE_REJECTED ? NOERROR : hr` folds to the sete/dec/and mask; each CAutoLock keeps its pointer in
//        edi and in the frame (EH states 0/1 for the paused block, 2/3 for the render block); the early returns unlock and
//        share the `xor eax,eax` epilogue.
// Layout: strmbase CBaseFilter m_State +0x14; CBaseRenderer m_evComplete +0x5C, m_pMediaSample +0x6C, m_InterfaceLock +0x7C,
//         m_RendererLock +0x94, m_bInReceive +0xB4 (the landed Stop's layout).
struct GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
struct IUnknown {
    virtual long __stdcall QueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
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
class CCritSec {                                 // strmbase wxutil.h
    CRITICAL_SECTION m_CritSec;
public:
    void Lock() { EnterCriticalSection(&m_CritSec); }
    void Unlock() { LeaveCriticalSection(&m_CritSec); }
};
class CAutoLock {                                // strmbase wxutil.h
protected:
    CCritSec* m_pLock;
public:
    CAutoLock(CCritSec* plock) { m_pLock = plock; m_pLock->Lock(); }
    ~CAutoLock() { m_pLock->Unlock(); }
};
class CAMEvent {                                 // strmbase wxutil.h
    void* m_hEvent;
public:
    void Set() { SetEvent(m_hEvent); }
};
struct IReferenceClock; struct IEnumPins; struct IPin; struct IFilterGraph; struct IMediaEventSink; struct FILTER_INFO; struct AMOVIESETUP_FILTER;
struct IBaseFilter : IUnknown {                  // IPersist + IMediaFilter + IBaseFilter
    virtual long __stdcall GetClassID(GUID* clsid) = 0;                        // 3
    virtual long __stdcall Stop() = 0;                                         // 4 (+0x10)
    virtual long __stdcall Pause() = 0;                                        // 5
    virtual long __stdcall Run(__int64 start) = 0;                             // 6
    virtual long __stdcall GetState(unsigned long timeout, long* state) = 0;   // 7
    virtual long __stdcall SetSyncSource(IReferenceClock* clock) = 0;          // 8
    virtual long __stdcall GetSyncSource(IReferenceClock** clock) = 0;         // 9
    virtual long __stdcall EnumPins(IEnumPins** ppEnum) = 0;                   // 10
    virtual long __stdcall FindPin(const unsigned short* id, IPin** pin) = 0;  // 11
    virtual long __stdcall QueryFilterInfo(FILTER_INFO* info) = 0;             // 12
    virtual long __stdcall JoinFilterGraph(IFilterGraph* graph, const unsigned short* name) = 0;   // 13
    virtual long __stdcall QueryVendorInfo(unsigned short** info) = 0;         // 14
};
struct IAMovieSetup : IUnknown {
    virtual long __stdcall Register() = 0;       // 3
    virtual long __stdcall Unregister() = 0;     // 4
};
struct IMediaSample;
struct CUnknown : INonDelegatingUnknown {        // strmbase combase.h (CBaseObject is empty in release)
    IUnknown* m_pUnknown;                        // +0x04
    long m_cRef;                                 // +0x08
    virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
};
class CBaseFilter : public CUnknown, public IBaseFilter, public IAMovieSetup {   // IBaseFilter +0x0C, IAMovieSetup +0x10
public:
    long m_State;                                // +0x14 FILTER_STATE (0 = State_Stopped, 1 = State_Paused)
    IReferenceClock* m_pClock;                   // +0x18
    __int64 m_tStart;                            // +0x20 CRefTime
    GUID m_clsid;                                // +0x28
    CCritSec* m_pLock;                           // +0x38
    unsigned short* m_pName;                     // +0x3c
    IFilterGraph* m_pGraph;                      // +0x40
    IMediaEventSink* m_pSink;                    // +0x44
    long m_PinVersion;                           // +0x48
    virtual AMOVIESETUP_FILTER* GetSetupData();  // primary slot 8 (+0x20)
    virtual void v09();
};
enum { State_Stopped = 0, State_Paused = 1 };
#define NOERROR 0
#define VFW_E_SAMPLE_REJECTED ((long)0x8004022BL)
#define FAILED(hr) (((long)(hr)) < 0)
class CBaseRenderer : public CBaseFilter {
public:
    virtual long SourceThreadCanWait(int canWait);              // primary slot 10 (+0x28)
    virtual long WaitForRenderTime();                           // primary slot 11 (+0x2c)
    virtual void v12();
    virtual void OnReceiveFirstSample(IMediaSample* sample);    // primary slot 13 (+0x34)
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void PrepareRender();                               // primary slot 20 (+0x50)
    virtual void v21(); virtual void v22(); virtual void v23();
    virtual long SendEndOfStream();                             // primary slot 24 (+0x60)
    virtual long ResetEndOfStream();                            // primary slot 25 (+0x64)
    virtual void v26();
    virtual long CancelNotification();                          // primary slot 27 (+0x6c)
    virtual long ClearPendingSample();                          // primary slot 28 (+0x70)
    virtual void v29(); virtual void v30(); virtual void v31();
    virtual long StopStreaming();                               // primary slot 32 (+0x80)
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual long PrepareReceive(IMediaSample* sample);          // primary slot 38 (+0x98)
    virtual long Receive(IMediaSample* sample);                 // primary slot 39 (+0x9c)
    virtual void v40(); virtual void v41();
    virtual long Render(IMediaSample* sample);                  // primary slot 42 (+0xa8)
    unsigned char _pad50[0x5c - 0x50];           // CBaseFilter is 0x50 (8-aligned by m_tStart)
    CAMEvent m_evComplete;                       // +0x5c
    int m_bAbort;                                // +0x60
    unsigned char _pad64[0x6c - 0x64];
    IMediaSample* m_pMediaSample;                // +0x6c
    unsigned char _pad70[0x7c - 0x70];
    CCritSec m_InterfaceLock;                    // +0x7c
    CCritSec m_RendererLock;                     // +0x94
    unsigned char _padac[0xb4 - 0xac];
    int m_bInReceive;                            // +0xb4
    void Ready() { m_evComplete.Set(); }
};

long CBaseRenderer::Receive(IMediaSample* pSample)
{
    // It may return VFW_E_SAMPLE_REJECTED code to say don't bother
    long hr = PrepareReceive(pSample);
    if (FAILED(hr)) {
        if (hr == VFW_E_SAMPLE_REJECTED) {
            return NOERROR;
        }
        return hr;
    }

    // We realize the palette in "PrepareRender()" so we have to give away the filter lock here.
    if (m_State == State_Paused) {
        PrepareRender();
        // no need to use InterlockedExchange
        m_bInReceive = 0;
        {
            // We must hold both these locks
            CAutoLock cRendererLock(&m_InterfaceLock);
            if (m_State == State_Stopped)
                return NOERROR;

            m_bInReceive = 1;
            CAutoLock cSampleLock(&m_RendererLock);
            OnReceiveFirstSample(pSample);
        }
        Ready();
    }
    // Having set an advise link with the clock we sit and wait.
    hr = WaitForRenderTime();
    if (FAILED(hr)) {
        m_bInReceive = 0;
        return NOERROR;
    }

    PrepareRender();
    m_bInReceive = 0;

    // We must hold both these locks
    CAutoLock cRendererLock(&m_InterfaceLock);

    // since we gave away the filter wide lock, the state of the filter could have changed to Stopped
    if (m_State == State_Stopped)
        return NOERROR;

    CAutoLock cSampleLock(&m_RendererLock);

    // Deal with this sample
    Render(m_pMediaSample);
    ClearPendingSample();
    SendEndOfStream();
    CancelNotification();
    return NOERROR;
}