// CBaseFilter::JoinFilterGraph(IFilterGraph* pGraph, LPCWSTR pName) -- DirectShow BaseClasses amfilter.cpp: under the filter lock, remember the graph (unreferenced), fetch its IMediaEventSink without keeping a reference, and replace the stored filter name with a copy of pName
// Leaf: ?JoinFilterGraph@CBaseFilter@@
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA7850, 205 bytes (+3 int3). __stdcall (STDMETHODIMP, IBaseFilter slot 13, `ret 0Ch`):
//   `this` arrives at the IBaseFilter sub-object (+0x0C), so members read 0x0C low (m_pLock [ebx+2Ch], m_pName +30h,
//   m_pGraph +34h, m_pSink +38h). EH frame (state 0) for the CAutoLock.
// Callees: EnterCriticalSection / LeaveCriticalSection (KERNEL32 IAT 0x0143FE0C / 0x0143FE10) through CAutoLock;
//   IFilterGraph::QueryInterface (slot 0) for IID_IMediaEventSink 0x012A9974, IMediaEventSink::Release (slot 2);
//   lstrlenW (KERNEL32 IAT 0x0143FE48); MSVCR71 scalar operator delete 0x00BFE9BC and scalar operator new 0x00BFEA1A (the
//   `delete[]` / `new WCHAR[]` of the source: strmbase declares no array operators, so VC7.1 binds the scalar ones);
//   CopyMemory = the memcpy intrinsic (`rep movsd ; rep movsb`).
// Shape: amfilter.cpp verbatim (release: the ASSERTs are empty).
// Label: manifest "Audio_SetDeviceSource" is a descriptive placeholder; the real name is the strmbase member.
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

extern "C" __declspec(dllimport) int __stdcall lstrlenW(const unsigned short* lpString);   // IAT 0x0143FE48
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int n);
#pragma intrinsic(memcpy)
#define CopyMemory(Destination, Source, Length) memcpy((Destination), (Source), (Length))
extern "C" const GUID IID_IMediaEventSink;       // 0x012A9974 {56A868A2-0AD4-11CE-B03A-0020AF0BA770}
#define FAILED(hr) (((long)(hr)) < 0)
#define ASSERT(_x_) ((void)0)
typedef unsigned short WCHAR;
typedef unsigned long DWORD;

long __stdcall CBaseFilter::JoinFilterGraph(IFilterGraph* pGraph, const unsigned short* pName)
{
    CAutoLock cObjectLock(m_pLock);

    // NOTE: we no longer hold references on the graph (m_pGraph, m_pSink)

    m_pGraph = pGraph;
    if (m_pGraph) {
        long hr = m_pGraph->QueryInterface(IID_IMediaEventSink,
                        (void**) &m_pSink);
        if (FAILED(hr)) {
            ASSERT(m_pSink == 0);
        }
        else m_pSink->Release();        // we do NOT keep a reference on it.
    } else {
        // if graph pointer is null, then we should
        // also release the IMediaEventSink on the same object - we don't
        // refcount it, so just set it to null
        m_pSink = 0;
    }

    if (m_pName) {
        delete[] m_pName;
        m_pName = 0;
    }

    if (pName) {
        DWORD nameLen = lstrlenW(pName)+1;
        m_pName = new WCHAR[nameLen];
        if (m_pName) {
            CopyMemory(m_pName, pName, nameLen*sizeof(WCHAR));
        } else {
            // !!! error here?
            ASSERT(0);
        }
    }

    return NOERROR;
}