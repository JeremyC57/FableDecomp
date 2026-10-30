// CBaseVideoRenderer::SendQuality(REFERENCE_TIME trLate, REFERENCE_TIME trRealStream) -- DirectShow BaseClasses renbase.cpp: build the quality message (Famine/Flood from the frame vs render averages, proportion 1000 scaled down when late or up to 2000 when waiting a lot, lateness plus half the render time, the stream time) and send it to the quality sink, fetching the upstream pin's IQualityControl first; S_FALSE without a sink
// Leaf: ?SendQuality@CBaseVideoRenderer@@UAEJ_J0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA56E0, 366 bytes, thiscall `ret 10h`, 0x18-byte frame (the Quality type; the
//   QueryInterface out pointer is homed in trLate's argument slot).
// Identity (proven): renbase.cpp SendQuality verbatim against the binary -- m_trFrameAvg +0x10C < 0 / > 2 * m_trRenderAvg
//   +0xF4, proportion 1000 - trLate / 10000 floored at 500 (`__alldiv` 0x00F3FC50), m_trWaitAvg +0x108 > 20000 &&
//   trLate < -20000 arm (1000 * (frame / (frame + 20000 - wait)), capped at 2000), Late = trLate + m_trRenderAvg / 2,
//   m_pQSink +0xAC from m_pInputPin (+0x78)->GetConnected() QueryInterface(IID_IQualityControl 0x012A99B4), Notify (slot
//   3) with the IBaseFilter sub-object (this + 0xC). The two references to 0x00CA56E0 are virtual-table slots
//   (0x0129D14C, 0x012BF698), as for the other landed CBaseVideoRenderer virtuals. The catalog label
//   Audio_ProcessTimeAdjustment is wrong.
// Model: the landed CBaseVideoRenderer model (JoinFilterGraph 0x00CA6110) pasted verbatim, with IQualityControl::Notify
//   given its real `Quality` by-value argument, CBasePin::GetConnected and CRendererInputPin's base filled in.
// Shape: renbase.cpp verbatim; CBaseVideoRenderer's DECLARE_IUNKNOWN makes the QueryInterface call unambiguous and virtual.
//   The address-taken pFilter out local is homed in pGraph's argument slot (`lea ecx,[esp+10h]`; pGraph itself is held in edi).
// Label: manifest "CSurface_HandleRefCountDecrement" is propagated; the real name is the strmbase member.
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
enum QualityMessageType { Famine = 0, Flood = 1 };   // strmif.h
typedef struct tagQuality {
    QualityMessageType Type;
    long Proportion;
    REFERENCE_TIME Late;
    REFERENCE_TIME TimeStamp;
} Quality;
struct IQualityControl : IUnknown {
    virtual long __stdcall Notify(IBaseFilter* pSelf, Quality q) = 0;          // 3
    virtual long __stdcall SetSink(IQualityControl* piqc) = 0;                 // 4
};
struct IPin : IUnknown {
    virtual long __stdcall Connect(IPin* pReceivePin, const void* pmt) = 0;            // 3
    virtual long __stdcall ReceiveConnection(IPin* pConnector, const void* pmt) = 0;   // 4
    virtual long __stdcall Disconnect() = 0;                                           // 5
    virtual long __stdcall ConnectedTo(IPin** pPin) = 0;                               // 6
    virtual long __stdcall ConnectionMediaType(void* pmt) = 0;                         // 7
    virtual long __stdcall QueryPinInfo(void* pInfo) = 0;                              // 8
    virtual long __stdcall QueryDirection(long* pPinDir) = 0;                          // 9
    virtual long __stdcall QueryId(unsigned short** Id) = 0;                           // 10
    virtual long __stdcall QueryAccept(const void* pmt) = 0;                           // 11
    virtual long __stdcall EnumMediaTypes(void** ppEnum) = 0;                          // 12
    virtual long __stdcall QueryInternalConnections(IPin** apPin, unsigned long* nPin) = 0;   // 13
    virtual long __stdcall EndOfStream() = 0;                                          // 14
    virtual long __stdcall BeginFlush() = 0;                                           // 15
    virtual long __stdcall EndFlush() = 0;                                             // 16
    virtual long __stdcall NewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate) = 0;   // 17
};
class CBasePin : public CUnknown, public IPin, public IQualityControl {   // amfilter.h: IPin +0x0C, IQualityControl +0x10
public:
    unsigned short* m_pName;                     // +0x14
    IPin* m_Connected;                           // +0x18
    long m_dir;                                  // +0x1c PIN_DIRECTION
    CCritSec* m_pLock;                           // +0x20
    bool m_bRunTimeError;                        // +0x24
    bool m_bCanReconnectWhenActive;              // +0x25
    bool m_bTryMyTypesFirst;                     // +0x26
    CBaseFilter* m_pFilter;                      // +0x28
    IQualityControl* m_pQSink;                   // +0x2c
    long m_TypeVersion;                          // +0x30
    unsigned char m_mt[0x48];                    // +0x34 CMediaType (AM_MEDIA_TYPE)
    CRefTime m_tStart;                           // +0x80
    CRefTime m_tStop;                            // +0x88
    double m_dRate;                              // +0x90
    virtual void CBasePin_v04(); virtual void CBasePin_v05(); virtual void CBasePin_v06(); virtual void CBasePin_v07();
    virtual void CBasePin_v08(); virtual void CBasePin_v09(); virtual void CBasePin_v10(); virtual void CBasePin_v11();
    virtual void CBasePin_v12(); virtual void CBasePin_v13();
    int IsConnected() { return (m_Connected != 0); }
    IPin* GetConnected() { return m_Connected; }
    bool CanReconnectWhenActive() { return m_bCanReconnectWhenActive; }
};
struct IQualProp : IUnknown {
    virtual long __stdcall get_FramesDroppedInRenderer(int* pcFrames) = 0;   // 3
    virtual long __stdcall get_FramesDrawn(int* pcFramesDrawn) = 0;          // 4
    virtual long __stdcall get_AvgFrameRate(int* piAvgFrameRate) = 0;        // 5
    virtual long __stdcall get_Jitter(int* iJitter) = 0;                     // 6
    virtual long __stdcall get_AvgSyncOffset(int* piAvg) = 0;                // 7
    virtual long __stdcall get_DevSyncOffset(int* piDev) = 0;                // 8
};
class CBaseVideoRenderer : public CBaseRenderer, public IQualProp, public IQualityControl {   // renbase.h: IQualProp +0xe0, IQualityControl +0xe4
public:
    int m_nNormal;                               // +0xe8
    int m_bSupplierHandlingQuality;              // +0xec
    int m_trThrottle;                            // +0xf0
    int m_trRenderAvg;                           // +0xf4
    int m_trRenderLast;                          // +0xf8
    int m_tRenderStart;                          // +0xfc
    int m_trEarliness;                           // +0x100
    int m_trTarget;                              // +0x104
    int m_trWaitAvg;                             // +0x108
    int m_trFrameAvg;                            // +0x10c
    int m_trDuration;                            // +0x110
    REFERENCE_TIME m_trRememberStampForPerf;     // +0x118
    int m_cFramesDropped;                        // +0x120
    int m_cFramesDrawn;                          // +0x124
    LONGLONG m_iTotAcc;                          // +0x128
    LONGLONG m_iSumSqAcc;                        // +0x130
    REFERENCE_TIME m_trLastDraw;                 // +0x138
    LONGLONG m_iSumSqFrameTime;                  // +0x140
    LONGLONG m_iSumFrameTime;                    // +0x148
    int m_trLate;                                // +0x150
    int m_trFrame;                               // +0x154
    int m_tStreamingStart;                       // +0x158
    // DECLARE_IUNKNOWN (renbase.h): one override of the four inherited IUnknown method sets
    long __stdcall QueryInterface(const GUID& riid, void** ppv);
    unsigned long __stdcall AddRef();
    unsigned long __stdcall Release();
    long __stdcall JoinFilterGraph(IFilterGraph* pGraph, const unsigned short* pName);
    virtual long SendQuality(REFERENCE_TIME trLate, REFERENCE_TIME trRealStream);
};


class CRendererInputPin : public CBasePin { };   // renbase.h (CBaseInputPin chain; only the inherited m_Connected is used)

extern "C" const GUID IID_IQualityControl;       // 0x012A99B4
#define UNITS 10000000
#define S_FALSE 1L
#define SUCCEEDED(hr) (((long)(hr)) >= 0)

long CBaseVideoRenderer::SendQuality(REFERENCE_TIME trLate,
                                     REFERENCE_TIME trRealStream)
{
    Quality q;
    long hr;

    // If we are the main user of time, then report this as Flood/Dry.
    // If our suppliers are, then report it as Famine/Glut.
    //
    // We need to take action, but avoid hunting.  Hunting is caused by
    // 1. Taking too much action too soon and overshooting
    // 2. Taking too long to react (so averaging can CAUSE hunting).
    //
    // The reason why we use trLate as well as Wait is to reduce hunting;
    // if the wait time is coming down and about to go into the red, we do
    // NOT want to rely on some average which is only telling is that it used
    // to be OK once.

    q.TimeStamp = (REFERENCE_TIME)trRealStream;

    if (m_trFrameAvg<0) {
        q.Type = Famine;      // guess
    }
    // Is the greater part of the time taken bltting or something else
    else if (m_trFrameAvg > 2*m_trRenderAvg) {
        q.Type = Famine;                        // mainly other
    } else {
        q.Type = Flood;                         // mainly bltting
    }

    q.Proportion = 1000;               // default

    if (m_trFrameAvg<0) {
        // leave it alone - we don't know enough
    }
    else if ( trLate> 0 ) {
        // try to catch up over the next second
        // We could be Really, REALLY late, but rendering all the frames
        // anyway, just because it's so cheap.

        q.Proportion = 1000 - (int)((trLate)/(UNITS/1000));
        if (q.Proportion<500) {
           q.Proportion = 500;      // don't go daft. (could've been negative!)
        } else {
        }

    } else if (  m_trWaitAvg>20000
              && trLate<-20000
              ){
        // Go cautiously faster - aim at 2mSec wait.
        if (m_trWaitAvg>=m_trFrameAvg) {
            // This can happen because of some fudges.
            // The waitAvg is how long we originally planned to wait
            // The frameAvg is more honest.
            // It means that we are spending a LOT of time waiting
            q.Proportion = 2000;    // double.
        } else {
            if (m_trFrameAvg+20000 > m_trWaitAvg) {
                q.Proportion
                    = 1000 * (m_trFrameAvg / (m_trFrameAvg + 20000 - m_trWaitAvg));
            } else {
                // We're apparently spending more than the whole frame time waiting.
                // Assume that the averages are slightly out of kilter, but that we
                // are indeed doing a lot of waiting.  (This leg probably never
                // happens, but the code avoids any potential divide by zero).
                q.Proportion = 2000;
            }
        }

        if (q.Proportion>2000) {
            q.Proportion = 2000;    // don't go crazy.
        }
    }

    // Tell the supplier how late frames are when they get rendered
    // That's how late we are now.
    q.Late = trLate + m_trRenderAvg/2;

    // A specific sink interface may be set through IPin

    if (m_pQSink==0) {
        // Get our input pin's peer.  We send quality management messages
        // to any nominated receiver of these things (set in the IPin
        // interface), or else to our source filter.

        IQualityControl *pQC = 0;
        IPin *pOutputPin = m_pInputPin->GetConnected();

        // And get an AddRef'd quality control interface

        hr = pOutputPin->QueryInterface(IID_IQualityControl,(void**) &pQC);
        if (SUCCEEDED(hr)) {
            m_pQSink = pQC;
        }
    }
    if (m_pQSink) {
        return m_pQSink->Notify(this,q);
    }

    return S_FALSE;

} // SendQuality