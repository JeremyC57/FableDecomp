// CMediaSample::GetTime(REFERENCE_TIME* pTimeStart, REFERENCE_TIME* pTimeEnd) -- DirectShow amfilter.cpp: report the sample's start/stop times: VFW_E_SAMPLE_TIME_NOT_SET with no time, start and start+1 with VFW_S_NO_STOP_TIME when only the start is valid
// Leaf: ?GetTime@CMediaSample
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA9130, 104 bytes, STDMETHODIMP (__stdcall, `this` on the stack), `ret 0Ch`.
// Identity: slot 5 (cell 0x012BF6F4) of CMediaSample's IMediaSample2 vtable 0x012BF6E0 (QueryInterface 0x00CA9000, the landed Release 0x00CA9080 and
//   SetMediaType 0x00CA93F0, scalar deleting dtor 0x00CAB6D0 at slot 21); amfilter.cpp's body. The catalog label is a placeholder.
// Layout: m_dwFlags +0x04 (Sample_TimeValid 0x10, Sample_StopValid 0x100), m_Start +0x20, m_End +0x28.
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
struct __declspec(novtable) IMediaSample : IUnknown {
    virtual HRESULT __stdcall GetPointer(BYTE** ppBuffer) = 0;                                   // 3
    virtual long __stdcall GetSize() = 0;                                                        // 4
    virtual HRESULT __stdcall GetTime(REFERENCE_TIME* pTimeStart, REFERENCE_TIME* pTimeEnd) = 0; // 5
    virtual HRESULT __stdcall SetTime(REFERENCE_TIME* pTimeStart, REFERENCE_TIME* pTimeEnd) = 0; // 6
    virtual HRESULT __stdcall IsSyncPoint() = 0;                                                 // 7
    virtual HRESULT __stdcall SetSyncPoint(BOOL bIsSyncPoint) = 0;                               // 8
    virtual HRESULT __stdcall IsPreroll() = 0;                                                   // 9
    virtual HRESULT __stdcall SetPreroll(BOOL bIsPreroll) = 0;                                   // 10
    virtual long __stdcall GetActualDataLength() = 0;                                            // 11
    virtual HRESULT __stdcall SetActualDataLength(long lLen) = 0;                                // 12
    virtual HRESULT __stdcall GetMediaType(AM_MEDIA_TYPE** ppMediaType) = 0;                     // 13
    virtual HRESULT __stdcall SetMediaType(AM_MEDIA_TYPE* pMediaType) = 0;                       // 14 (+0x38)
    virtual HRESULT __stdcall IsDiscontinuity() = 0;                                             // 15
    virtual HRESULT __stdcall SetDiscontinuity(BOOL bDiscontinuity) = 0;                         // 16
    virtual HRESULT __stdcall GetMediaTime(LONGLONG* pTimeStart, LONGLONG* pTimeEnd) = 0;        // 17
    virtual HRESULT __stdcall SetMediaTime(LONGLONG* pTimeStart, LONGLONG* pTimeEnd) = 0;        // 18
};
struct __declspec(novtable) IMediaSample2 : IMediaSample {
    virtual HRESULT __stdcall GetProperties(DWORD cbProperties, BYTE* pbProperties) = 0;         // 19
    virtual HRESULT __stdcall SetProperties(DWORD cbProperties, const BYTE* pbProperties) = 0;   // 20
};
class CBaseAllocator;
class CMediaSample : public IMediaSample2 {      // amfilter.h
protected:
    friend class CBaseAllocator;
    enum { Sample_SyncPoint = 0x01, Sample_Preroll = 0x02, Sample_Discontinuity = 0x04, Sample_TypeChanged = 0x08,
           Sample_TimeValid = 0x10, Sample_MediaTimeValid = 0x20, Sample_TimeDiscontinuity = 0x40, Sample_StopValid = 0x100,
           Sample_ValidFlags = 0x1FF };
    DWORD m_dwFlags;                              // +0x04
    DWORD m_dwTypeSpecificFlags;                  // +0x08
    BYTE* m_pBuffer;                              // +0x0C
    LONG m_lActual;                               // +0x10
    LONG m_cbBuffer;                              // +0x14
    CBaseAllocator* m_pAllocator;                 // +0x18
    CMediaSample* m_pNext;                        // +0x1C
    REFERENCE_TIME m_Start;                       // +0x20
    REFERENCE_TIME m_End;                         // +0x28
    LONGLONG m_MediaStart;                        // +0x30
    LONG m_MediaEnd;                              // +0x38
    AM_MEDIA_TYPE* m_pMediaType;                  // +0x3C
    DWORD m_dwStreamId;                           // +0x40
public:
    LONG m_cRef;                                  // +0x44
    virtual ~CMediaSample();
    HRESULT __stdcall QueryInterface(const IID& riid, void** ppv);
    ULONG __stdcall AddRef();
    ULONG __stdcall Release();
    HRESULT SetPointer(BYTE* ptr, LONG cBytes);
    HRESULT __stdcall GetPointer(BYTE** ppBuffer);
    long __stdcall GetSize();
    HRESULT __stdcall GetTime(REFERENCE_TIME* pTimeStart, REFERENCE_TIME* pTimeEnd);
    HRESULT __stdcall SetTime(REFERENCE_TIME* pTimeStart, REFERENCE_TIME* pTimeEnd);
    HRESULT __stdcall IsSyncPoint();
    HRESULT __stdcall SetSyncPoint(BOOL bIsSyncPoint);
    HRESULT __stdcall IsPreroll();
    HRESULT __stdcall SetPreroll(BOOL bIsPreroll);
    long __stdcall GetActualDataLength();
    HRESULT __stdcall SetActualDataLength(long lActual);
    HRESULT __stdcall GetMediaType(AM_MEDIA_TYPE** ppMediaType);
    HRESULT __stdcall SetMediaType(AM_MEDIA_TYPE* pMediaType);
    HRESULT __stdcall IsDiscontinuity();
    HRESULT __stdcall SetDiscontinuity(BOOL bDiscontinuity);
    HRESULT __stdcall GetMediaTime(LONGLONG* pTimeStart, LONGLONG* pTimeEnd);
    HRESULT __stdcall SetMediaTime(LONGLONG* pTimeStart, LONGLONG* pTimeEnd);
    HRESULT __stdcall GetProperties(DWORD cbProperties, BYTE* pbProperties);
    HRESULT __stdcall SetProperties(DWORD cbProperties, const BYTE* pbProperties);
};
class __declspec(novtable) CBaseAllocator : public CUnknown, public IMemAllocatorCallbackTemp, public CCritSec {
    // IMemAllocatorCallbackTemp +0x0C, CCritSec +0x10
    class CSampleList;
    friend class CSampleList;
    static CMediaSample*& NextSample(CMediaSample* pSample) { return pSample->m_pNext; }
    class CSampleList {
    public:
        CSampleList() : m_List(0), m_nOnList(0) {}
        CMediaSample* Head() const { return m_List; }
        CMediaSample* Next(CMediaSample* pSample) const { return CBaseAllocator::NextSample(pSample); }
        int GetCount() const { return m_nOnList; }
        void Add(CMediaSample* pSample) { CBaseAllocator::NextSample(pSample) = m_List; m_List = pSample; m_nOnList++; }
        CMediaSample* RemoveHead() {
            CMediaSample* pSample = m_List;
            if (pSample != 0) { m_List = CBaseAllocator::NextSample(m_List); m_nOnList--; }
            return pSample;
        }
        void Remove(CMediaSample* pSample);
    public:
        CMediaSample* m_List;
        int m_nOnList;
    };
protected:
    CSampleList m_lFree;                          // +0x28
    HANDLE m_hSem;                                // +0x30
    long m_lWaiting;                              // +0x34
    long m_lCount;                                // +0x38
    long m_lAllocated;                            // +0x3C
    long m_lSize;                                 // +0x40
    long m_lAlignment;                            // +0x44
    long m_lPrefix;                               // +0x48
    BOOL m_bChanged;                              // +0x4C
    BOOL m_bCommitted;                            // +0x50
    BOOL m_bDecommitInProgress;                   // +0x54
    IMemAllocatorNotifyCallbackTemp* m_pNotify;   // +0x58
    BOOL m_fEnableReleaseCallback;                // +0x5C
    virtual void Free() = 0;                      // primary slot 4
    virtual HRESULT Alloc();                      // primary slot 5
public:
    virtual ~CBaseAllocator();
    DECLARE_IUNKNOWN
    HRESULT __stdcall NonDelegatingQueryInterface(const IID& riid, void** ppv);
    HRESULT __stdcall SetProperties(ALLOCATOR_PROPERTIES* pRequest, ALLOCATOR_PROPERTIES* pActual);
    HRESULT __stdcall GetProperties(ALLOCATOR_PROPERTIES* pProps);
    HRESULT __stdcall Commit();
    HRESULT __stdcall Decommit();
    HRESULT __stdcall GetBuffer(IMediaSample** ppBuffer, REFERENCE_TIME* pStartTime, REFERENCE_TIME* pEndTime, DWORD dwFlags);
    HRESULT __stdcall ReleaseBuffer(IMediaSample* pBuffer);
    HRESULT __stdcall SetNotify(IMemAllocatorNotifyCallbackTemp* pNotify);
    HRESULT __stdcall GetFreeCount(LONG* plBuffersFree);
    void NotifySample();
    void SetWaiting() { m_lWaiting++; }
};
#define NOERROR 0L
#define S_OK 0L
#define S_FALSE 1L
#define E_POINTER ((HRESULT)0x80004003L)
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define CheckPointer(p, ret) { if ((p) == 0) return (ret); }
#define ValidateReadPtr(p, cb) 0
#define ValidateReadWritePtr(p, cb) 0
#define ASSERT(x)
#define UNREFERENCED_PARAMETER(P) (P)

HRESULT __stdcall CMediaSample::GetTime(REFERENCE_TIME* pTimeStart, REFERENCE_TIME* pTimeEnd)
{
    ValidateReadWritePtr(pTimeStart, sizeof(REFERENCE_TIME));
    ValidateReadWritePtr(pTimeEnd, sizeof(REFERENCE_TIME));

    if (!(m_dwFlags & Sample_StopValid)) {
        if (!(m_dwFlags & Sample_TimeValid)) {
            return (HRESULT)0x80040249L;   // VFW_E_SAMPLE_TIME_NOT_SET
        } else {
            *pTimeStart = m_Start;

            //  Make sure old stuff works
            *pTimeEnd = m_Start + 1;
            return (HRESULT)0x00040270L;   // VFW_S_NO_STOP_TIME
        }
    }

    *pTimeStart = m_Start;
    *pTimeEnd = m_End;
    return NOERROR;
}