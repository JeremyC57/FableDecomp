// CRendererInputPin::CompleteConnect(IPin* pReceivePin) -- DirectShow renbase.cpp: let the renderer complete the connection first, then the base pin (CBaseInputPin, i.e. CBasePin::CompleteConnect)
// Leaf: ?CompleteConnect@CRendererInputPin@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA5190, 40 bytes, thiscall (primary virtual, slot 12), ret 4.
//   Callees: CBaseRenderer::CompleteConnect (the renderer's slot 37, +0x94, landed 0x00CA6B80), CBasePin::CompleteConnect
//   0x00CA8090 (NOERROR, `ret 4`; CBaseInputPin does not override it).
// Library: DirectShow BaseClasses (strmbase, DX9 SDK era, UNICODE release build) linked into Fable.exe at 0xCA4780..;
//   model from the landed CBasePin::QueryPinInfo 0x00CA8420 and CRendererInputPin::EndOfStream 0x00CA4F90 (CBaseRenderer
//   slot list).
// Identity: slot 12 of the renderer input pin's primary vtable 0x012BF408 (xref 0x012BF438, after CheckConnect
//   0x00CA80C0 and BreakConnect 0x00CA5170); m_pRenderer +0xD8; renbase.cpp's body, which sits between the landed
//   CRendererInputPin::EndFlush 0x00CA50D0 / BreakConnect 0x00CA5170 and QueryId 0x00CA51C0. The catalog label is a placeholder.
#define FAILED(hr) (((HRESULT)(hr)) < 0)
// ---- DirectShow BaseClasses model (strmbase, DX9 SDK era, UNICODE build; combase.h / wxutil.h) ----
typedef long HRESULT;
typedef unsigned long ULONG;
typedef unsigned long DWORD;
typedef int BOOL;
typedef long LONG;
typedef void* HANDLE;
typedef unsigned short WCHAR;
typedef unsigned char BYTE;
typedef __int64 LONGLONG;
typedef LONGLONG REFERENCE_TIME;
typedef struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; } GUID;   // guiddef.h
typedef GUID IID;
typedef GUID CLSID;
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int n);
#pragma intrinsic(memcmp)
inline int operator==(const GUID& a, const GUID& b) { return !memcmp(&a, &b, sizeof(GUID)); }   // guiddef.h IsEqualGUID
inline int operator!=(const GUID& a, const GUID& b) { return !(a == b); }
struct __declspec(novtable) IUnknown {
    virtual HRESULT __stdcall QueryInterface(const IID& riid, void** ppv) = 0;   // 0
    virtual ULONG __stdcall AddRef() = 0;                                        // 1
    virtual ULONG __stdcall Release() = 0;                                       // 2
};
struct __declspec(novtable) INonDelegatingUnknown {
    virtual HRESULT __stdcall NonDelegatingQueryInterface(const IID& riid, void** ppv) = 0;
    virtual ULONG __stdcall NonDelegatingAddRef() = 0;
    virtual ULONG __stdcall NonDelegatingRelease() = 0;
};
class CBaseObject {                               // release build: only the static live-object count
    CBaseObject(const CBaseObject&);
    void operator=(const CBaseObject&);
    static LONG m_cObjects;
public:
    CBaseObject(const WCHAR* pName);
    CBaseObject(const char* pName);
    ~CBaseObject();
};
class __declspec(novtable) CUnknown : public INonDelegatingUnknown, public CBaseObject {
    IUnknown* const m_pUnknown;                   // +0x04
protected:
    LONG m_cRef;                                  // +0x08
public:
    CUnknown(const WCHAR* pName, IUnknown* pUnk);
    virtual ~CUnknown() {}                        // slot 3
    IUnknown* GetOwner() const { return m_pUnknown; }
    HRESULT __stdcall NonDelegatingQueryInterface(const IID& riid, void** ppv);
    ULONG __stdcall NonDelegatingAddRef();
    ULONG __stdcall NonDelegatingRelease();
};
#define DECLARE_IUNKNOWN \
    HRESULT __stdcall QueryInterface(const IID& riid, void** ppv) { return GetOwner()->QueryInterface(riid, ppv); } \
    ULONG __stdcall AddRef() { return GetOwner()->AddRef(); } \
    ULONG __stdcall Release() { return GetOwner()->Release(); }
extern "C" HRESULT __stdcall GetInterface(IUnknown* pUnk, void** ppv);   // 0x00CACBE0 (STDAPI)
struct CRITICAL_SECTION { void* DebugInfo; LONG LockCount; LONG RecursionCount; HANDLE OwningThread; HANDLE LockSemaphore; ULONG SpinCount; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE10
class CCritSec {                                  // wxutil.h, release build
    CCritSec(const CCritSec&);
    CCritSec& operator=(const CCritSec&);
    CRITICAL_SECTION m_CritSec;
public:
    CCritSec();
    ~CCritSec();
    void Lock() { EnterCriticalSection(&m_CritSec); }
    void Unlock() { LeaveCriticalSection(&m_CritSec); }
};
class CAutoLock {                                 // wxutil.h
    CAutoLock(const CAutoLock&);
    CAutoLock& operator=(const CAutoLock&);
protected:
    CCritSec* m_pLock;
public:
    CAutoLock(CCritSec* plock) { m_pLock = plock; m_pLock->Lock(); }
    ~CAutoLock() { m_pLock->Unlock(); }
};
class CRefTime {                                  // reftime.h
public:
    REFERENCE_TIME m_time;
    CRefTime() { m_time = 0; }
    CRefTime(REFERENCE_TIME rt) { m_time = rt; }
    operator REFERENCE_TIME() const { return m_time; }
    CRefTime& operator=(const CRefTime& rt) { m_time = rt.m_time; return *this; }
    CRefTime& operator=(const LONGLONG ll) { m_time = ll; return *this; }
    CRefTime& operator+=(const CRefTime& rt) { return (*this = *this + rt); }
};
// ---- strmbase amfilter.h / mtype.h model (only the members and vtable slots these bodies use are named) ----
typedef struct _AMMediaType {
    GUID majortype; GUID subtype; BOOL bFixedSizeSamples; BOOL bTemporalCompression; ULONG lSampleSize;
    GUID formattype; IUnknown* pUnk; ULONG cbFormat; BYTE* pbFormat;      // 0x48 bytes
} AM_MEDIA_TYPE;
class CMediaType : public _AMMediaType {          // mtype.h (no virtuals)
public:
    ~CMediaType();                                // 0x00CB1CC0
    CMediaType();                                 // 0x00CB1D00
    BOOL IsPartiallySpecified() const;            // 0x00CB1AE0
    BOOL MatchesPartial(const CMediaType* ppartial) const;   // 0x00CB1B20
};
void __stdcall DeleteMediaType(AM_MEDIA_TYPE* pmt);           // 0x00CB1F20 (WINAPI)
AM_MEDIA_TYPE* __stdcall CreateMediaType(const AM_MEDIA_TYPE* pSrc);   // 0x00CB1F70 (WINAPI)
typedef enum _PinDirection { PINDIR_INPUT = 0, PINDIR_OUTPUT = 1 } PIN_DIRECTION;   // strmif.h
enum FILTER_STATE { State_Stopped = 0, State_Paused = 1, State_Running = 2 };
struct IBaseFilter;
struct IReferenceClock;
struct IEnumPins;
struct IFilterGraph;
struct IMediaEventSink;
struct IMediaSample;
struct IEnumMediaTypes;
struct FILTER_INFO;
struct AMOVIESETUP_FILTER;
struct PIN_INFO { IBaseFilter* pFilter; PIN_DIRECTION dir; WCHAR achName[128]; };
struct ALLOCATOR_PROPERTIES { long cBuffers; long cbBuffer; long cbAlign; long cbPrefix; };
struct Quality { int Type; long Proportion; REFERENCE_TIME Late; REFERENCE_TIME TimeStamp; };
struct __declspec(novtable) IPin : IUnknown {
    virtual HRESULT __stdcall Connect(IPin* pReceivePin, const AM_MEDIA_TYPE* pmt) = 0;            // 3
    virtual HRESULT __stdcall ReceiveConnection(IPin* pConnector, const AM_MEDIA_TYPE* pmt) = 0;   // 4
    virtual HRESULT __stdcall Disconnect() = 0;                                                    // 5
    virtual HRESULT __stdcall ConnectedTo(IPin** pPin) = 0;                                        // 6
    virtual HRESULT __stdcall ConnectionMediaType(AM_MEDIA_TYPE* pmt) = 0;                         // 7
    virtual HRESULT __stdcall QueryPinInfo(PIN_INFO* pInfo) = 0;                                   // 8
    virtual HRESULT __stdcall QueryDirection(PIN_DIRECTION* pPinDir) = 0;                          // 9
    virtual HRESULT __stdcall QueryId(WCHAR** Id) = 0;                                             // 10
    virtual HRESULT __stdcall QueryAccept(const AM_MEDIA_TYPE* pmt) = 0;                           // 11
    virtual HRESULT __stdcall EnumMediaTypes(IEnumMediaTypes** ppEnum) = 0;                        // 12
    virtual HRESULT __stdcall QueryInternalConnections(IPin** apPin, ULONG* nPin) = 0;             // 13
    virtual HRESULT __stdcall EndOfStream() = 0;                                                   // 14
    virtual HRESULT __stdcall BeginFlush() = 0;                                                    // 15
    virtual HRESULT __stdcall EndFlush() = 0;                                                      // 16
    virtual HRESULT __stdcall NewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate) = 0;   // 17
};
struct __declspec(novtable) IQualityControl : IUnknown {
    virtual HRESULT __stdcall Notify(IBaseFilter* pSelf, Quality q) = 0;   // 3
    virtual HRESULT __stdcall SetSink(IQualityControl* piqc) = 0;          // 4
};
struct __declspec(novtable) IEnumMediaTypes : IUnknown {
    virtual HRESULT __stdcall Next(ULONG cMediaTypes, AM_MEDIA_TYPE** ppMediaTypes, ULONG* pcFetched) = 0;   // 3
    virtual HRESULT __stdcall Skip(ULONG cMediaTypes) = 0;                                                  // 4
    virtual HRESULT __stdcall Reset() = 0;                                                                  // 5
    virtual HRESULT __stdcall Clone(IEnumMediaTypes** ppEnum) = 0;                                          // 6
};
struct __declspec(novtable) IEnumPins : IUnknown {
    virtual HRESULT __stdcall Next(ULONG cPins, IPin** ppPins, ULONG* pcFetched) = 0;   // 3
    virtual HRESULT __stdcall Skip(ULONG cPins) = 0;                                   // 4
    virtual HRESULT __stdcall Reset() = 0;                                             // 5
    virtual HRESULT __stdcall Clone(IEnumPins** ppEnum) = 0;                           // 6
};
struct __declspec(novtable) IMemAllocator : IUnknown {
    virtual HRESULT __stdcall SetProperties(ALLOCATOR_PROPERTIES* pRequest, ALLOCATOR_PROPERTIES* pActual) = 0;   // 3
    virtual HRESULT __stdcall GetProperties(ALLOCATOR_PROPERTIES* pProps) = 0;                                    // 4
    virtual HRESULT __stdcall Commit() = 0;                                                                       // 5
    virtual HRESULT __stdcall Decommit() = 0;                                                                     // 6
    virtual HRESULT __stdcall GetBuffer(IMediaSample** ppBuffer, REFERENCE_TIME* pStartTime, REFERENCE_TIME* pEndTime, DWORD dwFlags) = 0;   // 7
    virtual HRESULT __stdcall ReleaseBuffer(IMediaSample* pBuffer) = 0;                                           // 8
};
struct IMemAllocatorNotifyCallbackTemp;
struct __declspec(novtable) IMemAllocatorCallbackTemp : IMemAllocator {
    virtual HRESULT __stdcall SetNotify(IMemAllocatorNotifyCallbackTemp* pNotify) = 0;   // 9
    virtual HRESULT __stdcall GetFreeCount(LONG* plBuffersFree) = 0;                     // 10
};
struct __declspec(novtable) IMemInputPin : IUnknown {
    virtual HRESULT __stdcall GetAllocator(IMemAllocator** ppAllocator) = 0;                          // 3
    virtual HRESULT __stdcall NotifyAllocator(IMemAllocator* pAllocator, BOOL bReadOnly) = 0;         // 4
    virtual HRESULT __stdcall GetAllocatorRequirements(ALLOCATOR_PROPERTIES* pProps) = 0;             // 5
    virtual HRESULT __stdcall Receive(IMediaSample* pSample) = 0;                                     // 6
    virtual HRESULT __stdcall ReceiveMultiple(IMediaSample** pSamples, long nSamples, long* nSamplesProcessed) = 0;   // 7
    virtual HRESULT __stdcall ReceiveCanBlock() = 0;                                                  // 8
};
struct __declspec(novtable) IPersist : IUnknown {
    virtual HRESULT __stdcall GetClassID(CLSID* pClassID) = 0;   // 3
};
struct __declspec(novtable) IMediaFilter : IPersist {
    virtual HRESULT __stdcall Stop() = 0;                                      // 4
    virtual HRESULT __stdcall Pause() = 0;                                     // 5
    virtual HRESULT __stdcall Run(REFERENCE_TIME tStart) = 0;                  // 6
    virtual HRESULT __stdcall GetState(DWORD dwMilliSecsTimeout, FILTER_STATE* State) = 0;   // 7
    virtual HRESULT __stdcall SetSyncSource(IReferenceClock* pClock) = 0;      // 8
    virtual HRESULT __stdcall GetSyncSource(IReferenceClock** pClock) = 0;     // 9
};
struct __declspec(novtable) IBaseFilter : IMediaFilter {
    virtual HRESULT __stdcall EnumPins(IEnumPins** ppEnum) = 0;                // 10
    virtual HRESULT __stdcall FindPin(const WCHAR* Id, IPin** ppPin) = 0;      // 11
    virtual HRESULT __stdcall QueryFilterInfo(FILTER_INFO* pInfo) = 0;         // 12
    virtual HRESULT __stdcall JoinFilterGraph(IFilterGraph* pGraph, const WCHAR* pName) = 0;   // 13
    virtual HRESULT __stdcall QueryVendorInfo(WCHAR** pVendorInfo) = 0;        // 14
};
struct __declspec(novtable) IAMovieSetup : IUnknown {
    virtual HRESULT __stdcall Register() = 0;     // 3
    virtual HRESULT __stdcall Unregister() = 0;   // 4
};
class CBasePin;
class __declspec(novtable) CBaseFilter : public CUnknown, public IBaseFilter, public IAMovieSetup {   // IBaseFilter +0x0C, IAMovieSetup +0x10
    friend class CBasePin;
protected:
    FILTER_STATE m_State;                         // +0x14
    IReferenceClock* m_pClock;                    // +0x18
    CRefTime m_tStart;                            // +0x20
    CLSID m_clsid;                                // +0x28
    CCritSec* m_pLock;                            // +0x38
    WCHAR* m_pName;                               // +0x3C
    IFilterGraph* m_pGraph;                       // +0x40
    IMediaEventSink* m_pSink;                     // +0x44
    LONG m_PinVersion;                            // +0x48
public:
    ~CBaseFilter();
    DECLARE_IUNKNOWN
    HRESULT __stdcall NonDelegatingQueryInterface(const IID& riid, void** ppv);
    HRESULT __stdcall GetClassID(CLSID* pClsID);
    HRESULT __stdcall GetState(DWORD dwMSecs, FILTER_STATE* State);
    HRESULT __stdcall SetSyncSource(IReferenceClock* pClock);
    HRESULT __stdcall GetSyncSource(IReferenceClock** pClock);
    HRESULT __stdcall Stop();
    HRESULT __stdcall Pause();
    HRESULT __stdcall Run(REFERENCE_TIME tStart);
    virtual HRESULT StreamTime(CRefTime& rtStream);                    // primary slot 4
    BOOL IsStopped() { return (m_State == State_Stopped); }
    HRESULT __stdcall EnumPins(IEnumPins** ppEnum);
    HRESULT __stdcall FindPin(const WCHAR* Id, IPin** ppPin);
    HRESULT __stdcall QueryFilterInfo(FILTER_INFO* pInfo);
    HRESULT __stdcall JoinFilterGraph(IFilterGraph* pGraph, const WCHAR* pName);
    HRESULT __stdcall QueryVendorInfo(WCHAR** pVendorInfo);
    virtual LONG GetPinVersion();                                      // primary slot 5
    virtual int GetPinCount() = 0;                                     // primary slot 6
    virtual CBasePin* GetPin(int n) = 0;                               // primary slot 7
    HRESULT __stdcall Register();
    HRESULT __stdcall Unregister();
    virtual AMOVIESETUP_FILTER* GetSetupData() { return 0; }           // primary slot 8
};
class __declspec(novtable) CBasePin : public CUnknown, public IPin, public IQualityControl {   // IPin +0x0C, IQualityControl +0x10
protected:
    WCHAR* m_pName;                               // +0x14
    IPin* m_Connected;                            // +0x18
    PIN_DIRECTION m_dir;                          // +0x1C
    CCritSec* m_pLock;                            // +0x20
    bool m_bRunTimeError;                         // +0x24
    bool m_bCanReconnectWhenActive;               // +0x25
    bool m_bTryMyTypesFirst;                      // +0x26
    CBaseFilter* m_pFilter;                       // +0x28
    IQualityControl* m_pQSink;                    // +0x2C
    LONG m_TypeVersion;                           // +0x30
    CMediaType m_mt;                              // +0x34
    CRefTime m_tStart;                            // +0x80
    CRefTime m_tStop;                             // +0x88
    double m_dRate;                               // +0x90
    void DisplayPinInfo(IPin* pReceivePin) {}
    void DisplayTypeInfo(IPin* pPin, const CMediaType* pmt) {}
    HRESULT AttemptConnection(IPin* pReceivePin, const CMediaType* pmt);                      // 0x00CA7DC0
    HRESULT TryMediaTypes(IPin* pReceivePin, const CMediaType* pmt, IEnumMediaTypes* pEnum);   // 0x00CA7E90
    HRESULT AgreeMediaType(IPin* pReceivePin, const CMediaType* pmt);                         // 0x00CA7FB0
public:
    virtual ~CBasePin();
    DECLARE_IUNKNOWN
    HRESULT __stdcall NonDelegatingQueryInterface(const IID& riid, void** ppv);
    ULONG __stdcall NonDelegatingRelease();
    ULONG __stdcall NonDelegatingAddRef();
    HRESULT __stdcall Connect(IPin* pReceivePin, const AM_MEDIA_TYPE* pmt);
    HRESULT __stdcall ReceiveConnection(IPin* pConnector, const AM_MEDIA_TYPE* pmt);
    HRESULT __stdcall Disconnect();
    HRESULT __stdcall ConnectedTo(IPin** pPin);
    HRESULT __stdcall ConnectionMediaType(AM_MEDIA_TYPE* pmt);
    HRESULT __stdcall QueryPinInfo(PIN_INFO* pInfo);
    HRESULT __stdcall QueryDirection(PIN_DIRECTION* pPinDir);
    HRESULT __stdcall QueryId(WCHAR** Id);
    HRESULT __stdcall QueryAccept(const AM_MEDIA_TYPE* pmt);
    HRESULT __stdcall EnumMediaTypes(IEnumMediaTypes** ppEnum);
    HRESULT __stdcall QueryInternalConnections(IPin** apPin, ULONG* nPin) { return 0x80004001L; }
    HRESULT __stdcall EndOfStream();
    HRESULT __stdcall NewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate);
    HRESULT __stdcall Notify(IBaseFilter* pSender, Quality q);
    HRESULT __stdcall SetSink(IQualityControl* piqc);
    BOOL IsConnected() { return (m_Connected != 0); }
    IPin* GetConnected() { return m_Connected; }
    BOOL IsStopped() { return (m_pFilter->m_State == State_Stopped); }
    virtual LONG GetMediaTypeVersion();                              // primary slot 4
    virtual HRESULT Active();                                        // 5
    virtual HRESULT Inactive();                                      // 6
    virtual HRESULT Run(REFERENCE_TIME tStart);                      // 7
    virtual HRESULT CheckMediaType(const CMediaType*) = 0;           // 8
    virtual HRESULT SetMediaType(const CMediaType*);                 // 9
    virtual HRESULT CheckConnect(IPin*);                             // 10
    virtual HRESULT BreakConnect();                                  // 11
    virtual HRESULT CompleteConnect(IPin* pReceivePin);              // 12
    virtual HRESULT GetMediaType(int iPosition, CMediaType* pMediaType);   // 13
    WCHAR* Name() { return m_pName; }
};
extern "C" __declspec(dllimport) int __stdcall SetEvent(HANDLE event);     // IAT 0x0143FDE0
extern "C" __declspec(dllimport) int __stdcall ResetEvent(HANDLE event);   // IAT 0x0143FED4
class CAMEvent {                                  // wxutil.h (holds only the HANDLE)
    HANDLE m_hEvent;
public:
    operator HANDLE() const { return m_hEvent; }
    void Set() { SetEvent(m_hEvent); }
    void Reset() { ResetEvent(m_hEvent); }
};
struct AM_SAMPLE2_PROPERTIES {                    // strmif.h, 0x30 bytes
    DWORD cbData; DWORD dwTypeSpecificFlags; DWORD dwSampleFlags; LONG lActual;
    REFERENCE_TIME tStart; REFERENCE_TIME tStop; DWORD dwStreamId; AM_MEDIA_TYPE* pMediaType; BYTE* pbBuffer; LONG cbBuffer;
};
class __declspec(novtable) CBaseInputPin : public CBasePin, public IMemInputPin {   // amfilter.h: IMemInputPin +0x98
protected:
    IMemAllocator* m_pAllocator;                  // +0x9C
    BYTE m_bReadOnly;                             // +0xA0
    BYTE m_bFlushing;                             // +0xA1
    AM_SAMPLE2_PROPERTIES m_SampleProps;          // +0xA8
public:
    virtual HRESULT CheckStreaming();             // primary slot 14
};
class CBaseRenderer;
class CRendererInputPin : public CBaseInputPin {  // renbase.h, 0xDC bytes
protected:
    CBaseRenderer* m_pRenderer;                   // +0xD8
public:
    HRESULT CompleteConnect(IPin* pReceivePin);
};
class CBaseRenderer : public CBaseFilter {        // renbase.h; 0xE0 bytes
public:
    void* m_pPosition;                            // +0x4C CRendererPosPassThru*
    CAMEvent m_RenderEvent;                       // +0x50
    CAMEvent m_ThreadSignal;                      // +0x54
    CAMEvent m_evComplete;                        // +0x58
    BOOL m_bAbort;                                // +0x5C
    BOOL m_bStreaming;                            // +0x60
    DWORD m_dwAdvise;                             // +0x64 DWORD_PTR
    IMediaSample* m_pMediaSample;                 // +0x68
    BOOL m_bEOS;                                  // +0x6C
    BOOL m_bEOSDelivered;                         // +0x70
    CRendererInputPin* m_pInputPin;               // +0x74
    CCritSec m_InterfaceLock;                     // +0x78
    CCritSec m_RendererLock;                      // +0x90
    IQualityControl* m_pQSink;                    // +0xA8
    BOOL m_bRepaintStatus;                        // +0xAC
    BOOL m_bInReceive;                            // +0xB0
    REFERENCE_TIME m_SignalTime;                  // +0xB8
    unsigned int m_EndOfStreamTimer;              // +0xC0
    CCritSec m_ObjectCreationLock;                // +0xC4

    // primary-vtable virtuals in renbase.h declaration order (slots 9..44)
    virtual HRESULT GetMediaPositionInterface(const IID& riid, void** ppv);   // 9 (+0x24)
    virtual HRESULT SourceThreadCanWait(BOOL bCanWait);                       // 10
    virtual HRESULT WaitForRenderTime();                                      // 11
    virtual HRESULT CompleteStateChange(FILTER_STATE OldState);               // 12
    virtual void OnReceiveFirstSample(IMediaSample* pMediaSample);            // 13
    virtual void OnRenderStart(IMediaSample* pMediaSample);                   // 14
    virtual void OnRenderEnd(IMediaSample* pMediaSample);                     // 15
    virtual HRESULT OnStartStreaming();                                       // 16 (+0x40)
    virtual HRESULT OnStopStreaming();                                        // 17 (+0x44)
    virtual void OnWaitStart();                                               // 18
    virtual void OnWaitEnd();                                                 // 19
    virtual void PrepareRender();                                             // 20
    virtual BOOL ScheduleSample(IMediaSample* pMediaSample);                  // 21
    virtual HRESULT GetSampleTimes(IMediaSample* pMediaSample, REFERENCE_TIME* pStartTime,
                                   REFERENCE_TIME* pEndTime);                 // 22
    virtual HRESULT ShouldDrawSampleNow(IMediaSample* pMediaSample, REFERENCE_TIME* ptrStart,
                                        REFERENCE_TIME* ptrEnd);              // 23
    virtual HRESULT SendEndOfStream();                                        // 24
    virtual HRESULT ResetEndOfStream();                                       // 25
    virtual HRESULT EndOfStream();                                            // 26
    virtual HRESULT CancelNotification();                                     // 27
    virtual HRESULT ClearPendingSample();                                     // 28
    virtual HRESULT Active();                                                 // 29
    virtual HRESULT Inactive();                                               // 30
    virtual HRESULT StartStreaming();                                         // 31
    virtual HRESULT StopStreaming();                                          // 32
    virtual HRESULT BeginFlush();                                             // 33
    virtual HRESULT EndFlush();                                               // 34
    virtual HRESULT BreakConnect();                                           // 35 (+0x8C)
    virtual HRESULT SetMediaType(const CMediaType* pmt);                      // 36 (+0x90)
    virtual HRESULT CompleteConnect(IPin* pReceivePin);                       // 37 (+0x94)
    virtual HRESULT PrepareReceive(IMediaSample* pMediaSample);               // 38
    virtual HRESULT Receive(IMediaSample* pMediaSample);                      // 39
    virtual BOOL HaveCurrentSample();                                         // 40
    virtual IMediaSample* GetCurrentSample();                                 // 41
    virtual HRESULT Render(IMediaSample* pMediaSample);                       // 42
    virtual HRESULT DoRenderSample(IMediaSample* pMediaSample) = 0;           // 43
    virtual HRESULT CheckMediaType(const CMediaType* pmt) = 0;                // 44 (+0xB0)
};

HRESULT CRendererInputPin::CompleteConnect(IPin* pReceivePin)
{
    HRESULT hr = m_pRenderer->CompleteConnect(pReceivePin);
    if (FAILED(hr)) {
        return hr;
    }
    return CBaseInputPin::CompleteConnect(pReceivePin);
}