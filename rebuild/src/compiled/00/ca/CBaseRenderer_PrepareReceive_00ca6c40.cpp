// CBaseRenderer::PrepareReceive(IMediaSample* pMediaSample) -- DirectShow BaseClasses (strmbase renbase.cpp): under the interface lock mark the filter as in Receive, run the pin's CBaseInputPin::Receive checks (E_FAIL on any refusal), apply a changed media type, then under the renderer lock refuse a second sample / end-of-stream / abort (E_UNEXPECTED after Ready()), register the media times, schedule the sample (VFW_E_SAMPLE_REJECTED when not scheduled), record its end time and keep an AddRef'd reference; repaint is re-enabled when not streaming
// Leaf: ?PrepareReceive@CBaseRenderer@@UAEJPAUIMediaSample@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA6C40, 460 bytes, virtual (primary slot 38, called through +0x98 by the landed
//   CBaseRenderer::Receive 0x00CA6E10), thiscall, `ret 4`, /EHsc frame (handler 0x01216B00; the two CAutoLocks are states 0/1).
// Identity (two sources): (1) the DX9 SDK BaseClasses renbase.cpp CBaseRenderer::PrepareReceive body verbatim -- E_FAIL
//   0x80004005, E_UNEXPECTED 0x8000FFFF, VFW_E_SAMPLE_REJECTED 0x8004022B, m_InterfaceLock +0x7C then m_RendererLock +0x94
//   (Enter/LeaveCriticalSection IAT 0x0143FE0C / 0x0143FE10), the direct CBaseInputPin::Receive call 0x00CA8B90 on the pin's
//   IMemInputPin (+0x98), SampleProps()->pMediaType (+0xCC) / tStop (+0xC0); (2) primary slot 38 of the renderer vtable, the
//   PrepareReceive slot the landed Receive 0x00CA6E10 calls first. The manifest label "CEngine_InitializeRender_WithSurface"
//   is propagated and wrong.
// Callees: CBaseInputPin::Receive 0x00CA8B90 (STDMETHODIMP), CBasePin::SetMediaType (pin primary slot 9), SetEvent IAT
//   0x0143FDE0 (inline Ready()), CRendererPosPassThru::RegisterMediaTime(IMediaSample*) 0x00CADC10 (landed under a wrong
//   descriptive name: NAME_FIXES), ScheduleSample (primary slot 21), IMediaSample::AddRef, CBaseRenderer::SetRepaintStatus
//   0x00CA4EA0.
// Layout: renbase.h CBaseRenderer members from +0x50 (CBaseFilter is 0x50): m_pPosition +0x50, m_RenderEvent +0x54,
//   m_ThreadSignal +0x58, m_evComplete +0x5C, m_bAbort +0x60, m_bStreaming +0x64, m_dwAdvise +0x68, m_pMediaSample +0x6C,
//   m_bEOS +0x70, m_bEOSDelivered +0x74, m_pInputPin +0x78, m_InterfaceLock +0x7C, m_RendererLock +0x94, m_pQSink +0xAC,
//   m_bRepaintStatus +0xB0, m_bInReceive +0xB4 (the SDK adds a cv-qualifier; plain BOOL here, byte-exact), m_SignalTime +0xB8.
// Model: the strmbase model of the landed CBaseInputPin::Receive 0x00CA8B90 (the DirectShow CBaseObject, not the engine's).
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
enum PIN_DIRECTION { PINDIR_INPUT = 0, PINDIR_OUTPUT = 1 };
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
struct __declspec(novtable) IMediaSample : IUnknown {
    virtual HRESULT __stdcall GetPointer(BYTE** ppBuffer) = 0;                                   // 3
    virtual long __stdcall GetSize() = 0;                                                        // 4
    virtual HRESULT __stdcall GetTime(REFERENCE_TIME* pTimeStart, REFERENCE_TIME* pTimeEnd) = 0;  // 5
    virtual HRESULT __stdcall SetTime(REFERENCE_TIME* pTimeStart, REFERENCE_TIME* pTimeEnd) = 0;  // 6
    virtual HRESULT __stdcall IsSyncPoint() = 0;                                                 // 7
    virtual HRESULT __stdcall SetSyncPoint(BOOL bIsSyncPoint) = 0;                               // 8
    virtual HRESULT __stdcall IsPreroll() = 0;                                                   // 9
    virtual HRESULT __stdcall SetPreroll(BOOL bIsPreroll) = 0;                                   // 10
    virtual long __stdcall GetActualDataLength() = 0;                                            // 11
    virtual HRESULT __stdcall SetActualDataLength(long lLen) = 0;                                // 12
    virtual HRESULT __stdcall GetMediaType(AM_MEDIA_TYPE** ppMediaType) = 0;                     // 13
    virtual HRESULT __stdcall SetMediaType(AM_MEDIA_TYPE* pMediaType) = 0;                       // 14
    virtual HRESULT __stdcall IsDiscontinuity() = 0;                                             // 15
    virtual HRESULT __stdcall SetDiscontinuity(BOOL bDiscontinuity) = 0;                         // 16
    virtual HRESULT __stdcall GetMediaTime(LONGLONG* pTimeStart, LONGLONG* pTimeEnd) = 0;        // 17
    virtual HRESULT __stdcall SetMediaTime(LONGLONG* pTimeStart, LONGLONG* pTimeEnd) = 0;        // 18
};
struct __declspec(novtable) IMediaSample2 : IMediaSample {
    virtual HRESULT __stdcall GetProperties(DWORD cbProperties, BYTE* pbProperties) = 0;         // 19
    virtual HRESULT __stdcall SetProperties(DWORD cbProperties, const BYTE* pbProperties) = 0;   // 20
};
struct __declspec(novtable) IMediaEventSink : IUnknown {
    virtual HRESULT __stdcall Notify(long EventCode, long EventParam1, long EventParam2) = 0;    // 3
};
extern "C" const IID IID_IMediaSample2;           // 0x012A9A64 (strmiids)
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
    HRESULT NotifyEvent(long EventCode, long EventParam1, long EventParam2);   // 0x00CA7930
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
class __declspec(novtable) CBaseOutputPin : public CBasePin {
protected:
    IMemAllocator* m_pAllocator;                  // +0x98
    IMemInputPin* m_pInputPin;                    // +0x9C
public:
    virtual HRESULT CompleteConnect(IPin* pReceivePin);
    virtual HRESULT DecideAllocator(IMemInputPin* pPin, IMemAllocator** pAlloc);                  // primary slot 14
    virtual HRESULT DecideBufferSize(IMemAllocator* pAlloc, ALLOCATOR_PROPERTIES* ppropInputRequest) = 0;   // 15
    virtual HRESULT GetDeliveryBuffer(IMediaSample** ppSample, REFERENCE_TIME* pStartTime, REFERENCE_TIME* pEndTime, DWORD dwFlags);   // 16
    virtual HRESULT Deliver(IMediaSample*);                                                       // 17
    virtual HRESULT InitAllocator(IMemAllocator** ppAlloc);                                       // 18
    HRESULT CheckConnect(IPin* pPin);
    HRESULT BreakConnect();
    HRESULT Active();
    HRESULT Inactive();
    HRESULT __stdcall EndOfStream();
    virtual HRESULT DeliverEndOfStream();                                                         // 19
    HRESULT __stdcall BeginFlush();
    HRESULT __stdcall EndFlush();
    virtual HRESULT DeliverBeginFlush();                                                          // 20
    virtual HRESULT DeliverEndFlush();                                                            // 21
    virtual HRESULT DeliverNewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate);   // 22
};
struct AM_SAMPLE2_PROPERTIES { DWORD cbData; DWORD dwTypeSpecificFlags; DWORD dwSampleFlags; LONG lActual; REFERENCE_TIME tStart; REFERENCE_TIME tStop; DWORD dwStreamId; AM_MEDIA_TYPE* pMediaType; BYTE* pbBuffer; LONG cbBuffer; };
class __declspec(novtable) CBaseInputPin : public CBasePin, public IMemInputPin {   // IMemInputPin +0x98
protected:
    IMemAllocator* m_pAllocator;                  // +0x9C
    BYTE m_bReadOnly;                             // +0xA0
    BYTE m_bFlushing;                             // +0xA1
    AM_SAMPLE2_PROPERTIES m_SampleProps;          // +0xA8
public:
    virtual ~CBaseInputPin();
    DECLARE_IUNKNOWN
    HRESULT __stdcall NonDelegatingQueryInterface(const IID& riid, void** ppv);
    HRESULT __stdcall GetAllocator(IMemAllocator** ppAllocator);
    HRESULT __stdcall NotifyAllocator(IMemAllocator* pAllocator, BOOL bReadOnly);
    HRESULT __stdcall Receive(IMediaSample* pSample);
    HRESULT __stdcall ReceiveMultiple(IMediaSample** pSamples, long nSamples, long* nSamplesProcessed);
    HRESULT __stdcall ReceiveCanBlock();
    HRESULT BreakConnect();
    BOOL IsReadOnly() { return m_bReadOnly; }
    BOOL IsFlushing() { return m_bFlushing; }
    AM_SAMPLE2_PROPERTIES* SampleProps() { return &m_SampleProps; }
    virtual HRESULT CheckStreaming();
    HRESULT __stdcall BeginFlush();
    HRESULT __stdcall EndFlush();
    HRESULT __stdcall Notify(IBaseFilter* pSender, Quality q);
    HRESULT Inactive();
    HRESULT __stdcall GetAllocatorRequirements(ALLOCATOR_PROPERTIES* pProps);
};
#define AM_SAMPLE_SPLICEPOINT 0x1
#define AM_SAMPLE_PREROLL 0x2
#define AM_SAMPLE_DATADISCONTINUITY 0x4
#define AM_SAMPLE_TIMEVALID 0x10
#define AM_SAMPLE_STOPVALID 0x100
#define AM_SAMPLE_TYPECHANGED 0x8
#define AM_STREAM_MEDIA 0
#define EC_COMPLETE 0x01
#define EC_ERRORABORT 0x03
#define VFW_E_INVALIDMEDIATYPE ((HRESULT)0x80040200L)
#define VFW_E_TYPE_NOT_ACCEPTED ((HRESULT)0x8004022AL)
#define NOERROR 0
#define S_OK 0
#define E_POINTER ((HRESULT)0x80004003L)
#define E_NOTIMPL ((HRESULT)0x80004001L)
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr) (((HRESULT)(hr)) < 0)

#define E_FAIL ((HRESULT)0x80004005L)
#define E_UNEXPECTED ((HRESULT)0x8000FFFFL)
#define VFW_E_SAMPLE_REJECTED ((HRESULT)0x8004022BL)
#define TRUE 1
#define FALSE 0
extern "C" __declspec(dllimport) BOOL __stdcall SetEvent(HANDLE hEvent);   // IAT 0x0143FDE0
class CAMEvent {                                  // wxutil.h
    CAMEvent(const CAMEvent&);
    CAMEvent& operator=(const CAMEvent&);
protected:
    HANDLE m_hEvent;
public:
    void Set() { SetEvent(m_hEvent); }
};
class CRendererPosPassThru {                      // ctlutil.h (only the member this body calls)
public:
    HRESULT RegisterMediaTime(IMediaSample* pMediaSample);   // 0x00CADC10
};
class CBaseRenderer;
class CRendererInputPin : public CBaseInputPin {  // renbase.h
protected:
    CBaseRenderer* m_pRenderer;
};
class __declspec(novtable) CBaseRenderer : public CBaseFilter {   // renbase.h
protected:
    CRendererPosPassThru* m_pPosition;            // +0x50
    CAMEvent m_RenderEvent;                       // +0x54
    CAMEvent m_ThreadSignal;                      // +0x58
    CAMEvent m_evComplete;                        // +0x5C
    BOOL m_bAbort;                                // +0x60
    BOOL m_bStreaming;                            // +0x64
    DWORD m_dwAdvise;                             // +0x68
    IMediaSample* m_pMediaSample;                 // +0x6C
    BOOL m_bEOS;                                  // +0x70
    BOOL m_bEOSDelivered;                         // +0x74
    CRendererInputPin* m_pInputPin;               // +0x78
    CCritSec m_InterfaceLock;                     // +0x7C
    CCritSec m_RendererLock;                      // +0x94
    IQualityControl* m_pQSink;                    // +0xAC
    BOOL m_bRepaintStatus;                        // +0xB0
    BOOL m_bInReceive;                            // +0xB4
    REFERENCE_TIME m_SignalTime;                  // +0xB8
public:
    virtual HRESULT GetMediaPositionInterface(const IID& riid, void** ppv);   // primary slot 9
    virtual HRESULT SourceThreadCanWait(BOOL bCanWait);                      // 10
    virtual HRESULT WaitForRenderTime();                                     // 11
    virtual HRESULT CompleteStateChange(FILTER_STATE OldState);              // 12
    virtual void OnReceiveFirstSample(IMediaSample* pMediaSample);           // 13
    virtual void OnRenderStart(IMediaSample* pMediaSample);                  // 14
    virtual void OnRenderEnd(IMediaSample* pMediaSample);                    // 15
    virtual HRESULT OnStartStreaming();                                      // 16
    virtual HRESULT OnStopStreaming();                                       // 17
    virtual void OnWaitStart();                                              // 18
    virtual void OnWaitEnd();                                                // 19
    virtual void PrepareRender();                                            // 20
    virtual BOOL ScheduleSample(IMediaSample* pMediaSample);                 // 21 (+0x54)
    virtual HRESULT GetSampleTimes(IMediaSample* pMediaSample, REFERENCE_TIME* pStartTime, REFERENCE_TIME* pEndTime);   // 22
    virtual HRESULT ShouldDrawSampleNow(IMediaSample* pMediaSample, REFERENCE_TIME* ptrStartTime, REFERENCE_TIME* ptrEndTime);   // 23
    virtual HRESULT SendEndOfStream();                                       // 24
    virtual HRESULT ResetEndOfStream();                                      // 25
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual HRESULT PrepareReceive(IMediaSample* pMediaSample);              // 38
    void Ready() { m_evComplete.Set(); }
    void SetRepaintStatus(BOOL bRepaint);                                    // 0x00CA4EA0
};

HRESULT CBaseRenderer::PrepareReceive(IMediaSample* pMediaSample)
{
    CAutoLock cInterfaceLock(&m_InterfaceLock);
    m_bInReceive = TRUE;

    // Check our flushing and filter state

    // This function must hold the interface lock because it calls
    // CBaseInputPin::Receive() and CBaseInputPin::Receive() uses
    // CBasePin::m_bRunTimeError.
    HRESULT hr = m_pInputPin->CBaseInputPin::Receive(pMediaSample);

    if (hr != NOERROR) {
        m_bInReceive = FALSE;
        return E_FAIL;
    }

    // Has the type changed on a media sample. We do all rendering
    // synchronously on the source thread, which has a side effect
    // that only one buffer is ever outstanding. Therefore when we
    // have Receive called we can go ahead and change the format
    // Since the format change can cause a SendMessage we just don't
    // lock
    if (m_pInputPin->SampleProps()->pMediaType) {
        hr = m_pInputPin->SetMediaType(
                (CMediaType*)m_pInputPin->SampleProps()->pMediaType);
        if (FAILED(hr)) {
            m_bInReceive = FALSE;
            return hr;
        }
    }


    CAutoLock cSampleLock(&m_RendererLock);

    // Return an error if we already have a sample waiting for rendering
    // source pins must serialise the Receive calls - we also check that
    // no data is being sent after the source signalled an end of stream

    if (m_pMediaSample || m_bEOS || m_bAbort) {
        Ready();
        m_bInReceive = FALSE;
        return E_UNEXPECTED;
    }

    // Store the media times from this sample
    if (m_pPosition) m_pPosition->RegisterMediaTime(pMediaSample);

    // Schedule the next sample if we are streaming

    if ((m_bStreaming == TRUE) && (ScheduleSample(pMediaSample) == FALSE)) {
        m_bInReceive = FALSE;
        return VFW_E_SAMPLE_REJECTED;
    }

    // Store the sample end time for EC_COMPLETE handling
    m_SignalTime = m_pInputPin->SampleProps()->tStop;

    // BEWARE we sometimes keep the sample even after returning the thread to
    // the source filter such as when we go into a stopped state (we keep it
    // to refresh the device with) so we must AddRef it to keep it safely. If
    // we start flushing the source thread is released and any sample waiting
    // will be released otherwise GetBuffer may never return (see BeginFlush)

    m_pMediaSample = pMediaSample;
    m_pMediaSample->AddRef();

    if (m_bStreaming == FALSE) {
        SetRepaintStatus(TRUE);
    }
    return NOERROR;
}