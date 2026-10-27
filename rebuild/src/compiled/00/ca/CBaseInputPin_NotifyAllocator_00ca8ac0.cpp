// CBaseInputPin::NotifyAllocator(IMemAllocator* pAllocator, BOOL bReadOnly) -- DirectShow amfilter.cpp IMemInputPin::NotifyAllocator: E_POINTER on a null allocator; under the pin lock AddRef the new allocator, store it, Release the old one, and record the read-only flag; S_OK
// Leaf: ?NotifyAllocator@CBaseInputPin@@UAGJPAUIMemAllocator@@H@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA8AC0, 144 bytes, STDMETHODIMP `ret 0Ch`; `this` on the stack is the IMemInputPin subobject
// (+0x98): m_pLock -0x78, m_pAllocator +0x04, m_bReadOnly (BYTE) +0x08. EH frame (state 0 = the CAutoLock).
// Identity: IMemInputPin slot 4 (NotifyAllocator) of the renderer input pin's IMemInputPin table 0x012BF388 (reference at 0x012BF398),
// not overridden by CRendererInputPin. Catalogued as gap-scan sub_00ca8ac0.
// Callees: Enter/LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10); IMemAllocator AddRef / Release (slots 1 / 2).
// Model: the landed CBaseInputPin / CBasePin model (0x00CA8CF0) pasted verbatim.
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
struct AM_MEDIA_TYPE {                          // strmif.h _AMMediaType, spelled like the landed CopyMediaType 0x00CB1BD0 binding
    GUID majortype; GUID subtype; BOOL bFixedSizeSamples; BOOL bTemporalCompression; ULONG lSampleSize;
    GUID formattype; IUnknown* pUnk; ULONG cbFormat; BYTE* pbFormat;      // 0x48 bytes
};
class CMediaType : public AM_MEDIA_TYPE {          // mtype.h (no virtuals)
public:
    ~CMediaType();                                // 0x00CB1CC0
    CMediaType();                                 // 0x00CB1D00
    void InitMediaType();                         // 0x00CB1AC0
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
    virtual HRESULT CheckStreaming();
    HRESULT __stdcall BeginFlush();
    HRESULT __stdcall EndFlush();
    HRESULT __stdcall Notify(IBaseFilter* pSender, Quality q);
    HRESULT Inactive();
    HRESULT __stdcall GetAllocatorRequirements(ALLOCATOR_PROPERTIES* pProps);
};
struct __declspec(novtable) IPinFlowControl : IUnknown {
    virtual HRESULT __stdcall Block(DWORD dwBlockFlags, HANDLE hEvent) = 0;   // 3
};
struct IGraphConfig;
class CDynamicOutputPin : public CBaseOutputPin, public IPinFlowControl {   // IPinFlowControl +0xA0
public:
    ~CDynamicOutputPin();
    DECLARE_IUNKNOWN
    HRESULT __stdcall NonDelegatingQueryInterface(const IID& riid, void** ppv);
    HRESULT __stdcall Disconnect();
    HRESULT __stdcall Block(DWORD dwBlockFlags, HANDLE hEvent);
    HRESULT DeliverBeginFlush();
    HRESULT DeliverEndFlush();
    HRESULT Inactive();
    HRESULT Active();
    virtual HRESULT CompleteConnect(IPin* pReceivePin);
    virtual HRESULT StartUsingOutputPin();                                   // primary slot 23
    virtual void StopUsingOutputPin();                                       // 24
    virtual bool StreamingThreadUsingOutputPin();                            // 25
    HRESULT ChangeMediaType(const CMediaType* pmt);
    HRESULT DynamicReconnect(const CMediaType* pmt);
protected:
    HRESULT SynchronousBlockOutputPin();                                     // 0x00CAC400
    HRESULT AsynchronousBlockOutputPin(HANDLE hNotifyCallerPinBlockedEvent); // 0x00CAB870
    HRESULT UnblockOutputPin();                                              // 0x00CA9860
    void BlockOutputPin();                                                   // 0x00CA9820
    void ResetBlockState();
    HRESULT ChangeMediaTypeHelper(const CMediaType* pmt);                    // 0x00CA9B70
    enum BLOCK_STATE { NOT_BLOCKED, PENDING, BLOCKED };
    CCritSec m_BlockStateLock;                    // +0xA4
    HANDLE m_hUnblockOutputPinEvent;              // +0xBC
    HANDLE m_hNotifyCallerPinBlockedEvent;        // +0xC0
    BLOCK_STATE m_BlockState;                     // +0xC4
    DWORD m_dwBlockCallerThreadID;                // +0xC8
    DWORD m_dwNumOutstandingOutputPinUsers;       // +0xCC
    HANDLE m_hStopEvent;                          // +0xD0
    IGraphConfig* m_pGraphConfig;                 // +0xD4
    BOOL m_bPinUsesReadOnlyAllocator;             // +0xD8
};
HRESULT __stdcall CBaseInputPin::NotifyAllocator(IMemAllocator* pAllocator, BOOL bReadOnly)
{
    if (pAllocator == 0) return (HRESULT)0x80004003L;   // CheckPointer(pAllocator, E_POINTER)
    CAutoLock cObjectLock(m_pLock);

    IMemAllocator* pOldAllocator = m_pAllocator;
    pAllocator->AddRef();
    m_pAllocator = pAllocator;

    if (pOldAllocator != 0) {
        pOldAllocator->Release();
    }

    // the readonly flag indicates whether samples from this allocator should
    // be regarded as readonly - if true, then inplace transforms will not be
    // allowed.
    m_bReadOnly = (BYTE)bReadOnly;
    return 0;
}