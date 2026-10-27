// CPosPassThru::NonDelegatingQueryInterface(const IID& riid, void** ppv) -- DirectShow ctlutil.cpp: clear *ppv; expose IMediaSeeking (-8) and, through the inlined CMediaPosition::NonDelegatingQueryInterface, IMediaPosition (-4); else defer to CUnknown
// Leaf: ?NonDelegatingQueryInterface@CPosPassThru
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAD380, 121 bytes, STDMETHODIMP (__stdcall) on the CUnknown base (+0x08), `ret 0Ch`.
//   Callees: GetInterface 0x00CACBE0, CUnknown::NonDelegatingQueryInterface 0x00CACCB0 (tail jump); IID_IMediaSeeking
//   0x012A9A04, IID_IMediaPosition 0x012AA064.
// Identity: primary slot 0 of both pass-through vtables (cells 0x012BF8E4 / 0x012BF994: NonDelegatingQueryInterface,
//   CUnknown::NonDelegatingAddRef/Release 0x00CACD00/0x00CACD20, dtor); ctlutil.cpp's body. Placeholder label.
// Shape: CMediaPosition::NonDelegatingQueryInterface is defined first in this TU (as in ctlutil.cpp) and inlined at the
//   qualified call; the static_casts are unconditional (`add eax,-8` / `add ecx,-4`, no null test) since `this` is a
//   CUnknown sub-object reached through the INonDelegatingUnknown vtable.
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
// ---- oaidl.h / ctlutil.h model: automation types, CBaseDispatch, CDispParams ----
typedef unsigned int UINT;
typedef unsigned short WORD;
typedef DWORD LCID;
typedef WCHAR OLECHAR;
typedef OLECHAR* BSTR;
typedef OLECHAR* LPOLESTR;
typedef long DISPID;
typedef double REFTIME;
struct IDispatch;
enum VARENUM { VT_EMPTY = 0, VT_I4 = 3, VT_BSTR = 8, VT_DISPATCH = 9, VT_UNKNOWN = 13 };
struct VARIANT {                                  // oaidl.h, 16 bytes
    unsigned short vt;                            // +0x00
    WORD wReserved1, wReserved2, wReserved3;
    union {                                       // +0x08
        LONG lVal;
        double dblVal;
        BSTR bstrVal;
        IUnknown* punkVal;
        IDispatch* pdispVal;
    };
};
struct DISPPARAMS {                               // oaidl.h
    VARIANT* rgvarg;                              // +0x00
    DISPID* rgdispidNamedArgs;                    // +0x04
    UINT cArgs;                                   // +0x08
    UINT cNamedArgs;                              // +0x0C
};
struct EXCEPINFO;
struct __declspec(novtable) ITypeInfo : IUnknown {                                             // oaidl.h
    virtual HRESULT __stdcall GetTypeAttr(void** ppTypeAttr) = 0;                               // 3
    virtual void GetTypeComp_() = 0;                                                            // 4  (not used here)
    virtual void GetFuncDesc_() = 0;                                                            // 5
    virtual void GetVarDesc_() = 0;                                                             // 6
    virtual void GetNames_() = 0;                                                               // 7
    virtual void GetRefTypeOfImplType_() = 0;                                                   // 8
    virtual void GetImplTypeFlags_() = 0;                                                       // 9
    virtual void GetIDsOfNames_() = 0;                                                          // 10
    virtual HRESULT __stdcall Invoke(void* pvInstance, DISPID memid, WORD wFlags, DISPPARAMS* pDispParams,
                                     VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) = 0;   // 11 (+0x2C)
};
struct __declspec(novtable) ITypeLib : IUnknown {
    virtual UINT __stdcall GetTypeInfoCount() = 0;                                              // 3
    virtual HRESULT __stdcall GetTypeInfo(UINT index, ITypeInfo** ppTInfo) = 0;                 // 4
    virtual HRESULT __stdcall GetTypeInfoType(UINT index, int* pTKind) = 0;                     // 5
    virtual HRESULT __stdcall GetTypeInfoOfGuid(const GUID& guid, ITypeInfo** ppTinfo) = 0;     // 6 (+0x18)
};
struct __declspec(novtable) IDispatch : IUnknown {
    virtual HRESULT __stdcall GetTypeInfoCount(UINT* pctinfo) = 0;                              // 3
    virtual HRESULT __stdcall GetTypeInfo(UINT itinfo, LCID lcid, ITypeInfo** pptinfo) = 0;    // 4
    virtual HRESULT __stdcall GetIDsOfNames(const IID& riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid) = 0;   // 5
    virtual HRESULT __stdcall Invoke(DISPID dispidMember, const IID& riid, LCID lcid, WORD wFlags, DISPPARAMS* pdispparams,
                                     VARIANT* pvarResult, EXCEPINFO* pexcepinfo, UINT* puArgErr) = 0;   // 6
};
class CBaseDispatch {                             // ctlutil.h
    ITypeInfo* m_pti;                             // +0x00
public:
    CBaseDispatch() : m_pti(0) {}
    ~CBaseDispatch();
    HRESULT __stdcall GetTypeInfoCount(UINT* pctinfo);
    HRESULT __stdcall GetTypeInfo(const IID& riid, UINT itinfo, LCID lcid, ITypeInfo** pptinfo);
    HRESULT __stdcall GetIDsOfNames(const IID& riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid);
};
class CDispParams : public DISPPARAMS {           // ctlutil.h
public:
    CDispParams(UINT nArgs, VARIANT* pArgs, HRESULT* phr = 0);
    ~CDispParams();
};
// ---- control.h / strmif.h / ctlutil.h model: IMediaPosition, IMediaSeeking, CMediaPosition, CPosPassThru ----
struct __declspec(novtable) IMediaPosition : IDispatch {
    virtual HRESULT __stdcall get_Duration(REFTIME* plength) = 0;                               // 7
    virtual HRESULT __stdcall put_CurrentPosition(REFTIME llTime) = 0;                          // 8
    virtual HRESULT __stdcall get_CurrentPosition(REFTIME* pllTime) = 0;                        // 9
    virtual HRESULT __stdcall get_StopTime(REFTIME* pllTime) = 0;                               // 10
    virtual HRESULT __stdcall put_StopTime(REFTIME llTime) = 0;                                 // 11
    virtual HRESULT __stdcall get_PrerollTime(REFTIME* pllTime) = 0;                            // 12
    virtual HRESULT __stdcall put_PrerollTime(REFTIME llTime) = 0;                              // 13
    virtual HRESULT __stdcall put_Rate(double dRate) = 0;                                       // 14
    virtual HRESULT __stdcall get_Rate(double* pdRate) = 0;                                     // 15
    virtual HRESULT __stdcall CanSeekForward(LONG* pCanSeekForward) = 0;                        // 16
    virtual HRESULT __stdcall CanSeekBackward(LONG* pCanSeekBackward) = 0;                      // 17
};
struct __declspec(novtable) IMediaSeeking : IUnknown {
    virtual HRESULT __stdcall GetCapabilities(DWORD* pCapabilities) = 0;                        // 3
    virtual HRESULT __stdcall CheckCapabilities(DWORD* pCapabilities) = 0;                      // 4
    virtual HRESULT __stdcall IsFormatSupported(const GUID* pFormat) = 0;                       // 5
    virtual HRESULT __stdcall QueryPreferredFormat(GUID* pFormat) = 0;                          // 6
    virtual HRESULT __stdcall GetTimeFormat(GUID* pFormat) = 0;                                 // 7
    virtual HRESULT __stdcall IsUsingTimeFormat(const GUID* pFormat) = 0;                       // 8
    virtual HRESULT __stdcall SetTimeFormat(const GUID* pFormat) = 0;                           // 9
    virtual HRESULT __stdcall GetDuration(LONGLONG* pDuration) = 0;                             // 10
    virtual HRESULT __stdcall GetStopPosition(LONGLONG* pStop) = 0;                             // 11 (+0x2C)
    virtual HRESULT __stdcall GetCurrentPosition(LONGLONG* pCurrent) = 0;                       // 12 (+0x30)
    virtual HRESULT __stdcall ConvertTimeFormat(LONGLONG* pTarget, const GUID* pTargetFormat, LONGLONG Source, const GUID* pSourceFormat) = 0;   // 13
    virtual HRESULT __stdcall SetPositions(LONGLONG* pCurrent, DWORD dwCurrentFlags, LONGLONG* pStop, DWORD dwStopFlags) = 0;   // 14
    virtual HRESULT __stdcall GetPositions(LONGLONG* pCurrent, LONGLONG* pStop) = 0;            // 15
    virtual HRESULT __stdcall GetAvailable(LONGLONG* pEarliest, LONGLONG* pLatest) = 0;         // 16
    virtual HRESULT __stdcall SetRate(double dRate) = 0;                                        // 17
    virtual HRESULT __stdcall GetRate(double* pdRate) = 0;                                      // 18
    virtual HRESULT __stdcall GetPreroll(LONGLONG* pllPreroll) = 0;                             // 19
};
class __declspec(novtable) CMediaPosition : public IMediaPosition, public CUnknown {   // CUnknown +0x04
    CBaseDispatch m_basedisp;                     // +0x10
public:
    CMediaPosition(const WCHAR* pName, IUnknown* pUnk);
    CMediaPosition(const WCHAR* pName, IUnknown* pUnk, HRESULT* phr);
    DECLARE_IUNKNOWN
    HRESULT __stdcall NonDelegatingQueryInterface(const IID& riid, void** ppv);
    HRESULT __stdcall GetTypeInfoCount(UINT* pctinfo);
    HRESULT __stdcall GetTypeInfo(UINT itinfo, LCID lcid, ITypeInfo** pptinfo);
    HRESULT __stdcall GetIDsOfNames(const IID& riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid);
    HRESULT __stdcall Invoke(DISPID dispidMember, const IID& riid, LCID lcid, WORD wFlags, DISPPARAMS* pdispparams,
                             VARIANT* pvarResult, EXCEPINFO* pexcepinfo, UINT* puArgErr);
};
class CPosPassThru : public IMediaSeeking, public CMediaPosition {   // IMediaSeeking +0x00, CMediaPosition +0x04
    IPin* m_pPin;                                 // +0x18
    HRESULT GetPeer(IMediaPosition** ppMP);       // 0x00CAD400
    HRESULT GetPeerSeeking(IMediaSeeking** ppMS); // 0x00CAD460
public:
    CPosPassThru(const WCHAR* pName, IUnknown* pUnk, HRESULT* phr, IPin* pPin);
    DECLARE_IUNKNOWN
    HRESULT ForceRefresh() { return 0; }
    virtual HRESULT GetMediaTime(LONGLONG* pStartTime, LONGLONG* pEndTime) { return (HRESULT)0x80004005L; }   // primary slot 20 (+0x50)
    HRESULT __stdcall NonDelegatingQueryInterface(const IID& riid, void** ppv);
    HRESULT __stdcall GetCapabilities(DWORD* pCapabilities);
    HRESULT __stdcall CheckCapabilities(DWORD* pCapabilities);
    HRESULT __stdcall SetTimeFormat(const GUID* pFormat);
    HRESULT __stdcall GetTimeFormat(GUID* pFormat);
    HRESULT __stdcall IsUsingTimeFormat(const GUID* pFormat);
    HRESULT __stdcall IsFormatSupported(const GUID* pFormat);
    HRESULT __stdcall QueryPreferredFormat(GUID* pFormat);
    HRESULT __stdcall ConvertTimeFormat(LONGLONG* pTarget, const GUID* pTargetFormat, LONGLONG Source, const GUID* pSourceFormat);
    HRESULT __stdcall SetPositions(LONGLONG* pCurrent, DWORD CurrentFlags, LONGLONG* pStop, DWORD StopFlags);
    HRESULT __stdcall GetPositions(LONGLONG* pCurrent, LONGLONG* pStop);
    HRESULT __stdcall GetCurrentPosition(LONGLONG* pCurrent);
    HRESULT __stdcall GetStopPosition(LONGLONG* pStop);
    HRESULT __stdcall SetRate(double dRate);
    HRESULT __stdcall GetRate(double* pdRate);
    HRESULT __stdcall GetDuration(LONGLONG* pDuration);
    HRESULT __stdcall GetAvailable(LONGLONG* pEarliest, LONGLONG* pLatest);
    HRESULT __stdcall GetPreroll(LONGLONG* pllPreroll);
    HRESULT __stdcall get_Duration(REFTIME* plength);
    HRESULT __stdcall put_CurrentPosition(REFTIME llTime);
    HRESULT __stdcall get_StopTime(REFTIME* pllTime);
    HRESULT __stdcall put_StopTime(REFTIME llTime);
    HRESULT __stdcall get_PrerollTime(REFTIME* pllTime);
    HRESULT __stdcall put_PrerollTime(REFTIME llTime);
    HRESULT __stdcall get_Rate(double* pdRate);
    HRESULT __stdcall put_Rate(double dRate);
    HRESULT __stdcall get_CurrentPosition(REFTIME* pllTime);
    HRESULT __stdcall CanSeekForward(LONG* pCanSeekForward);
    HRESULT __stdcall CanSeekBackward(LONG* pCanSeekBackward);
private:
    HRESULT GetSeekingLongLong(HRESULT (__stdcall IMediaSeeking::*pMethod)(LONGLONG*), LONGLONG* pll);
};
class CRendererPosPassThru : public CPosPassThru {
    CCritSec m_PositionLock;                      // +0x1C
    LONGLONG m_StartMedia;                        // +0x38
    LONGLONG m_EndMedia;                          // +0x40
    BOOL m_bReset;                                // +0x48
public:
    CRendererPosPassThru(const WCHAR* pName, IUnknown* pUnk, HRESULT* phr, IPin* pPin);
    HRESULT RegisterMediaTime(IMediaSample* pMediaSample);
    HRESULT RegisterMediaTime(LONGLONG StartTime, LONGLONG EndTime);
    HRESULT GetMediaTime(LONGLONG* pStartTime, LONGLONG* pEndTime);
    HRESULT ResetMediaTime();
    HRESULT EOS();
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
extern "C" const IID IID_IMediaSeeking;           // 0x012A9A04
extern "C" const IID IID_IMediaPosition;          // 0x012AA064

HRESULT __stdcall CMediaPosition::NonDelegatingQueryInterface(const IID& riid, void** ppv)
{
    ValidateReadWritePtr(ppv, sizeof(void*));
    if (riid == IID_IMediaPosition) {
        return GetInterface((IMediaPosition*)this, ppv);
    } else {
        return CUnknown::NonDelegatingQueryInterface(riid, ppv);
    }
}

HRESULT __stdcall CPosPassThru::NonDelegatingQueryInterface(const IID& riid, void** ppv)
{
    CheckPointer(ppv, E_POINTER);
    *ppv = 0;

    if (riid == IID_IMediaSeeking) {
        return GetInterface(static_cast<IMediaSeeking*>(this), ppv);
    }
    // let base class handle other interfaces...
    return CMediaPosition::NonDelegatingQueryInterface(riid, ppv);
}