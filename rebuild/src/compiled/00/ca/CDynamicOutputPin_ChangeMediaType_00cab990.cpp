// CDynamicOutputPin::ChangeMediaType(const CMediaType*) -- DirectShow BaseClasses amfilter.cpp: VFW_E_NOT_CONNECTED when unconnected; a downstream IPinConnection that accepts the type dynamically gets ChangeMediaTypeHelper (failure returned, else S_OK); otherwise DynamicReconnect (inlined: IGraphConfig::Reconnect with the stop event, E_FAIL without the graph config or event)
// Leaf: ?ChangeMediaType@CDynamicOutputPin@@QAEJPBVCMediaType@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAB990, 305 bytes, thiscall `ret 4`; /EHsc frame: the QzCComPtr<IPinConnection> is EH state
//   0 (its destructor is inline on the normal paths and out of line, 0x00CAA6E0, in the unwind funclet).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 449 "likely 0xCAB990 = CDynamicOutputPin::ChangeMediaType", verified): the IID at
//   0x012A93F4 is {4a9a62d3-27d4-403d-91e9-89f540e55534} = IID_IPinConnection (slot 3 DynamicQueryAccept); 0x80040209 is
//   VFW_E_NOT_CONNECTED; m_Connected +0x18 is queried; the reconnect goes through m_pGraphConfig +0xd4 slot 3 (IGraphConfig::
//   Reconnect) with the IPin sub-object (+0x0c), NULL, pmt, NULL, m_hStopEvent +0xd0 and 2 =
//   AM_GRAPH_CONFIG_RECONNECT_CACHE_REMOVED_FILTERS; the dynamic leg calls 0x00CA9B70 (ChangeMediaTypeHelper). Its caller
//   0x00CAC4E4 is ChangeOutputFormat 0x00CAC4A0. DynamicReconnect is a plain amfilter.cpp member that VC7.1 /O2 expands here
//   (checked on the lane with a non-inline definition); it is spelled `inline` so this row does not also emit its body.
//   CDynamicOutputPin's primary vtable 0x012BF7A8 (stored by its constructors at 0x00CA96A4 / 0x00CAB7CC / 0x00CAC35C) lines
//   up with amfilter.h's declaration order: 9 SetMediaType 0x00CA80A0, 10 CheckConnect, 11 BreakConnect 0x00CA8660,
//   12 CompleteConnect, 14..22 CBaseOutputPin's DecideAllocator .. DeliverNewSegment (15 DecideBufferSize = _purecall), 23
//   StartUsingOutputPin 0x00CA98E0, 24 StopUsingOutputPin 0x00CA9A00, 25 StreamingThreadUsingOutputPin 0x00CA9A40 (+0x64);
//   members after CBaseOutputPin (m_pAllocator +0x98, m_pInputPin +0x9c) and the IPinFlowControl vptr (+0xa0):
//   m_BlockStateLock +0xa4, m_hUnblockOutputPinEvent +0xbc, m_hNotifyCallerPinBlockedEvent +0xc0, m_BlockState +0xc4,
//   m_dwBlockCallerThreadID +0xc8, m_hStopEvent +0xd0, m_pGraphConfig +0xd4.
// Label: the landed name is a propagated manifest label / by-address placeholder; the real name is the strmbase member.
// ---- strmbase model (DirectShow BaseClasses, DX9 SDK era: combase.h / wxutil.h / mtype.h / amfilter.h; UNICODE, release build) ----
typedef struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; } GUID;
typedef __int64 REFERENCE_TIME;
typedef void* HANDLE;
#define NOERROR 0L
#define S_OK 0L
#define FAILED(hr) (((long)(hr)) < 0)
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
// combase.h CBaseObject (no data in release; counts live objects), in the namespace the landed strmbase rows use so
// its destructor's symbol stays apart from the game's own CBaseObject (0x0099A380 / 0x0099A430) in the relocation bindings.
namespace N_00cacaf0 { class CBaseObject {
public:
    CBaseObject(const unsigned short* pName);
    ~CBaseObject();                              // 0x00CACAF0
}; } using N_00cacaf0::CBaseObject;
class __declspec(novtable) CUnknown : public INonDelegatingUnknown, public CBaseObject {   // combase.h (AM_NOVTABLE)
    IUnknown* const m_pUnknown;                  // +0x04 owner of this object
protected:
    long m_cRef;                                 // +0x08 (combase.h qualifies it; no access here)
public:
    CUnknown(const unsigned short* pName, IUnknown* pUnk);
    virtual ~CUnknown() { }                      // primary slot 3
    long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv);
    unsigned long __stdcall NonDelegatingAddRef();
    unsigned long __stdcall NonDelegatingRelease();
};
class CRefTime {                                 // reftime.h: exactly one REFERENCE_TIME, no virtuals
public:
    REFERENCE_TIME m_time;
};
struct AM_MEDIA_TYPE {                           // strmif.h, 0x48 bytes
    GUID majortype; GUID subtype; int bFixedSizeSamples; int bTemporalCompression; unsigned long lSampleSize;
    GUID formattype; IUnknown* pUnk; unsigned long cbFormat; unsigned char* pbFormat;
};
class CMediaType : public AM_MEDIA_TYPE {        // mtype.h (no virtuals)
public:
    ~CMediaType();                               // 0x00CB1CC0
    long Set(const CMediaType& rt);              // 0x00CB2020
};
class CBaseFilter;
struct IMediaSample;
struct ALLOCATOR_PROPERTIES;
struct IEnumMediaTypes;
struct IQualityControl : IUnknown {
    virtual long __stdcall Notify(struct IBaseFilter* pSelf, __int64 q) = 0;   // 3
    virtual long __stdcall SetSink(IQualityControl* piqc) = 0;                 // 4
};
struct IPin : IUnknown {
    virtual long __stdcall Connect(IPin* pReceivePin, const AM_MEDIA_TYPE* pmt) = 0;            // 3
    virtual long __stdcall ReceiveConnection(IPin* pConnector, const AM_MEDIA_TYPE* pmt) = 0;   // 4
    virtual long __stdcall Disconnect() = 0;                                                    // 5
    virtual long __stdcall ConnectedTo(IPin** pPin) = 0;                                        // 6
    virtual long __stdcall ConnectionMediaType(AM_MEDIA_TYPE* pmt) = 0;                         // 7
    virtual long __stdcall QueryPinInfo(void* pInfo) = 0;                                       // 8
    virtual long __stdcall QueryDirection(long* pPinDir) = 0;                                   // 9
    virtual long __stdcall QueryId(unsigned short** Id) = 0;                                    // 10
    virtual long __stdcall QueryAccept(const AM_MEDIA_TYPE* pmt) = 0;                           // 11
    virtual long __stdcall EnumMediaTypes(IEnumMediaTypes** ppEnum) = 0;                        // 12
    virtual long __stdcall QueryInternalConnections(IPin** apPin, unsigned long* nPin) = 0;     // 13
    virtual long __stdcall EndOfStream() = 0;                                                   // 14
    virtual long __stdcall BeginFlush() = 0;                                                    // 15
    virtual long __stdcall EndFlush() = 0;                                                      // 16
    virtual long __stdcall NewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate) = 0;   // 17
};
class __declspec(novtable) CBasePin : public CUnknown, public IPin, public IQualityControl {   // amfilter.h: IPin +0x0C, IQualityControl +0x10
protected:
    unsigned short* m_pName;                     // +0x14 WCHAR*
    IPin* m_Connected;                           // +0x18
    long m_dir;                                  // +0x1c PIN_DIRECTION
    CCritSec* m_pLock;                           // +0x20
    bool m_bRunTimeError;                        // +0x24
    bool m_bCanReconnectWhenActive;              // +0x25
    bool m_bTryMyTypesFirst;                     // +0x26
    CBaseFilter* m_pFilter;                      // +0x28
    IQualityControl* m_pQSink;                   // +0x2c
    long m_TypeVersion;                          // +0x30
    CMediaType m_mt;                             // +0x34
    CRefTime m_tStart;                           // +0x80
    CRefTime m_tStop;                            // +0x88
    double m_dRate;                              // +0x90
public:
    CBasePin(const unsigned short* pObjectName, CBaseFilter* pFilter, CCritSec* pLock, long* phr,
             const unsigned short* pName, long dir);
    virtual ~CBasePin();                         // primary slot 3
    // DECLARE_IUNKNOWN
    long __stdcall QueryInterface(const GUID& riid, void** ppv);
    unsigned long __stdcall AddRef();
    unsigned long __stdcall Release();
    long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv);
    unsigned long __stdcall NonDelegatingRelease();
    unsigned long __stdcall NonDelegatingAddRef();
    int IsConnected() { return (m_Connected != 0); }
    IPin* GetConnected() { return m_Connected; }
    virtual long GetMediaTypeVersion();                                     // 4 (+0x10)
    virtual long Active();                                                  // 5
    virtual long Inactive();                                                // 6
    virtual long Run(REFERENCE_TIME tStart);                                // 7
    virtual long CheckMediaType(const CMediaType*) = 0;                     // 8
    virtual long SetMediaType(const CMediaType*);                           // 9 (+0x24)
    virtual long CheckConnect(IPin*);                                       // 10
    virtual long BreakConnect();                                            // 11 (+0x2c)
    virtual long CompleteConnect(IPin* pReceivePin);                        // 12
    virtual long GetMediaType(int iPosition, CMediaType* pMediaType);       // 13
};
struct IMemAllocator : IUnknown {
    virtual long __stdcall SetProperties(ALLOCATOR_PROPERTIES* pRequest, ALLOCATOR_PROPERTIES* pActual) = 0;   // 3
    virtual long __stdcall GetProperties(ALLOCATOR_PROPERTIES* pProps) = 0;                                    // 4
    virtual long __stdcall Commit() = 0;                                                                       // 5
    virtual long __stdcall Decommit() = 0;                                                                     // 6 (+0x18)
};
struct IMemInputPin : IUnknown { };
class __declspec(novtable) CBaseOutputPin : public CBasePin {   // amfilter.h (AM_NOVTABLE)
protected:
    IMemAllocator* m_pAllocator;                 // +0x98
    IMemInputPin* m_pInputPin;                   // +0x9c interface on the downstream input pin
public:
    CBaseOutputPin(const unsigned short* pObjectName, CBaseFilter* pFilter, CCritSec* pLock, long* phr,
                   const unsigned short* pName);
    virtual long CompleteConnect(IPin* pReceivePin);
    virtual long DecideAllocator(IMemInputPin* pPin, IMemAllocator** pAlloc);                     // 14
    virtual long DecideBufferSize(IMemAllocator* pAlloc, ALLOCATOR_PROPERTIES* ppropInputRequest) = 0;   // 15
    virtual long GetDeliveryBuffer(IMediaSample** ppSample, REFERENCE_TIME* pStartTime,
                                   REFERENCE_TIME* pEndTime, unsigned long dwFlags);              // 16
    virtual long Deliver(IMediaSample*);                                                          // 17
    virtual long InitAllocator(IMemAllocator** ppAlloc);                                          // 18
    long CheckConnect(IPin* pPin);
    long BreakConnect();
    long Active();
    long Inactive();
    long __stdcall EndOfStream();
    virtual long DeliverEndOfStream();                                                            // 19
    long __stdcall BeginFlush();
    long __stdcall EndFlush();
    virtual long DeliverBeginFlush();                                                             // 20
    virtual long DeliverEndFlush();                                                               // 21
    virtual long DeliverNewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate);    // 22
};
struct IPinFlowControl : IUnknown {
    virtual long __stdcall Block(unsigned long dwBlockFlags, HANDLE hEvent) = 0;   // 3
};
struct IGraphConfig : IUnknown {
    virtual long __stdcall Reconnect(IPin* pOutputPin, IPin* pInputPin, const AM_MEDIA_TYPE* pmtFirstConnection,
                                     struct IBaseFilter* pUsingFilter, HANDLE hAbortEvent, unsigned long dwFlags) = 0;   // 3
};
class CDynamicOutputPin : public CBaseOutputPin, public IPinFlowControl {   // amfilter.h: IPinFlowControl +0xa0, primary vtable 0x012BF7A8
public:
    CDynamicOutputPin(const unsigned short* pObjectName, CBaseFilter* pFilter, CCritSec* pLock, long* phr,
                      const unsigned short* pName);
    ~CDynamicOutputPin();
    // DECLARE_IUNKNOWN
    long __stdcall QueryInterface(const GUID& riid, void** ppv);
    unsigned long __stdcall AddRef();
    unsigned long __stdcall Release();
    long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv);
    long __stdcall Disconnect();
    long __stdcall Block(unsigned long dwBlockFlags, HANDLE hEvent);
    void SetConfigInfo(IGraphConfig* pGraphConfig, HANDLE hStopEvent);
    long DeliverBeginFlush();
    long DeliverEndFlush();
    long Inactive();
    long Active();
    virtual long CompleteConnect(IPin* pReceivePin);
    virtual long StartUsingOutputPin();                                     // 23 (+0x5c)
    virtual void StopUsingOutputPin();                                      // 24 (+0x60)
    virtual bool StreamingThreadUsingOutputPin();                           // 25 (+0x64)
    long ChangeOutputFormat(const AM_MEDIA_TYPE* pmt, REFERENCE_TIME tSegmentStart, REFERENCE_TIME tSegmentStop,
                            double dSegmentRate);
    long ChangeMediaType(const CMediaType* pmt);
    long DynamicReconnect(const CMediaType* pmt);
protected:
    long SynchronousBlockOutputPin();
    long AsynchronousBlockOutputPin(HANDLE hNotifyCallerPinBlockedEvent);
    long UnblockOutputPin();
    void BlockOutputPin();
    void ResetBlockState();
    static long WaitEvent(HANDLE hEvent);
    enum BLOCK_STATE { NOT_BLOCKED, PENDING, BLOCKED };
    CCritSec m_BlockStateLock;                   // +0xa4
    HANDLE m_hUnblockOutputPinEvent;             // +0xbc manual-reset, signalled while the pin is not blocked
    HANDLE m_hNotifyCallerPinBlockedEvent;       // +0xc0 duplicated from the Block() caller
    BLOCK_STATE m_BlockState;                    // +0xc4
    unsigned long m_dwBlockCallerThreadID;       // +0xc8
    unsigned long m_dwNumOutstandingOutputPinUsers;   // +0xcc
    HANDLE m_hStopEvent;                         // +0xd0
    IGraphConfig* m_pGraphConfig;                // +0xd4
    int m_bPinUsesReadOnlyAllocator;             // +0xd8
private:
    long Initialize();
    long ChangeMediaTypeHelper(const CMediaType* pmt);   // 0x00CA9B70
};

template <class T> class QzCComPtr {             // combase.h
public:
    QzCComPtr() { p = 0; }
    ~QzCComPtr() { if (p) p->Release(); }
    operator T*() { return (T*)p; }
    T** operator&() { return &p; }
    T* operator->() { return p; }
    T* p;
};
struct IPinConnection : IUnknown {
    virtual long __stdcall DynamicQueryAccept(const AM_MEDIA_TYPE* pmt) = 0;   // 3 (+0x0c)
    virtual long __stdcall NotifyEndOfStream(HANDLE hNotifyEvent) = 0;        // 4
    virtual long __stdcall IsEndPin() = 0;                                     // 5
    virtual long __stdcall DynamicDisconnect() = 0;                            // 6
};
extern "C" const GUID IID_IPinConnection;        // 0x012A93F4
#define VFW_E_NOT_CONNECTED ((long)0x80040209L)
#define E_FAIL ((long)0x80004005L)
#define AM_GRAPH_CONFIG_RECONNECT_CACHE_REMOVED_FILTERS 0x00000002

inline long CDynamicOutputPin::DynamicReconnect(const CMediaType* pmt)
{
    // The caller should call StartUsingOutputPin() before calling this
    // method.
    if ((m_pGraphConfig == 0) || (0 == m_hStopEvent)) {
        return E_FAIL;
    }

    long hr = m_pGraphConfig->Reconnect(this,
                                        0,
                                        pmt,
                                        0,
                                        m_hStopEvent,
                                        AM_GRAPH_CONFIG_RECONNECT_CACHE_REMOVED_FILTERS);

    return hr;
}

long CDynamicOutputPin::ChangeMediaType(const CMediaType* pmt)
{
    // This function assumes the filter graph is running.
    if (!IsConnected()) {
        return VFW_E_NOT_CONNECTED;
    }

    /*  First check if the downstream pin will accept a dynamic
        format change
    */
    QzCComPtr<IPinConnection> pConnection;

    m_Connected->QueryInterface(IID_IPinConnection, (void**)&pConnection);
    if (pConnection != 0) {

        if (S_OK == pConnection->DynamicQueryAccept(pmt)) {

            long hr = ChangeMediaTypeHelper(pmt);
            if (FAILED(hr)) {
                return hr;
            }

            return S_OK;
        }
    }

    /*  Can't do the dynamic connection */
    return DynamicReconnect(pmt);
}
