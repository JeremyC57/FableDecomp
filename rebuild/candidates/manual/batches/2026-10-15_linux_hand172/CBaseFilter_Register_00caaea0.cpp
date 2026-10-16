// CBaseFilter::Register() -- DirectShow base class: register the filter's setup data with the filter mapper
// Leaf: ?Register@CBaseFilter
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAAEA0, 107 bytes (+5 int3). __stdcall (STDMETHODIMP, `ret 4`).
// Class: catalog "CTextureRenderer::vfunc_3" -- slot 3 of CTextureRenderer's IAMovieSetup vtable 0x0129D030; the body is
//        strmbase amfilter.cpp CBaseFilter::Register with `this` at the IAMovieSetup subobject (+0x10; GetSetupData is
//        primary-vtable slot 8 reached through `this - 0x10`). VIDEO LAYER (DirectShow strmbase, replacement candidate).
// Callees: CoInitialize (IAT 0x01440644), CoCreateInstance (0x01440640) with CLSID_FilterMapper 0x012AB184 and
//          IID_IFilterMapper 0x012A99E4, AMovieSetupRegisterFilter(const AMOVIESETUP_FILTER*, IFilterMapper*, BOOL)
//          0x00CAA4A0 (strmbase dllsetup.cpp), CoFreeUnusedLibraries (0x01440634), CoUninitialize (0x01440648).
// Shape: S_FALSE (1) without setup data; the mapper pointer lives in the dead `this` slot; NOERROR at the end.
struct GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int n);
#pragma intrinsic(memcmp)
inline int IsEqualGUID(const GUID& a, const GUID& b) { return !memcmp(&a, &b, sizeof(GUID)); }
inline int operator==(const GUID& a, const GUID& b) { return IsEqualGUID(a, b); }
extern "C" const GUID IID_IQualProp;             // 0x012AAB04 {1BD0ECB0-F8E2-11CE-AAC6-0020AF0B99A3}
extern "C" const GUID IID_IQualityControl;       // 0x012A99B4 {56A868A5-0AD4-11CE-B03A-0020AF0BA770}
struct IUnknown {
    virtual long __stdcall QueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};
long __stdcall GetInterface(IUnknown* unknown, void** ppv);   // 0x00CACBE0 (strmbase combase.cpp)
struct IQualProp : IUnknown {
    virtual long __stdcall get_FramesDroppedInRenderer(int* frames) = 0;
};
struct IQualityControl : IUnknown {
    virtual long __stdcall Notify(void* sender, long q) = 0;
};
struct INonDelegatingUnknown {
    virtual long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};
struct CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE10
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
struct IReferenceClock : IUnknown { };
struct IEnumPins : IUnknown { };
struct IPin;
struct IFilterGraph;
struct IMediaEventSink;
struct IFilterMapper : IUnknown { };
struct AMOVIESETUP_FILTER;
struct FILTER_INFO;
struct IBaseFilter : IUnknown {                  // IPersist + IMediaFilter + IBaseFilter
    virtual long __stdcall GetClassID(GUID* clsid) = 0;                        // 3
    virtual long __stdcall Stop() = 0;                                         // 4
    virtual long __stdcall Pause() = 0;                                        // 5
    virtual long __stdcall Run(__int64 start) = 0;                             // 6
    virtual long __stdcall GetState(unsigned long timeout, long* state) = 0;   // 7
    virtual long __stdcall SetSyncSource(IReferenceClock* clock) = 0;          // 8 (+0x20)
    virtual long __stdcall GetSyncSource(IReferenceClock** clock) = 0;         // 9 (+0x24)
    virtual long __stdcall EnumPins(IEnumPins** ppEnum) = 0;                   // 10 (+0x28)
    virtual long __stdcall FindPin(const unsigned short* id, IPin** pin) = 0;  // 11
    virtual long __stdcall QueryFilterInfo(FILTER_INFO* info) = 0;             // 12
    virtual long __stdcall JoinFilterGraph(IFilterGraph* graph, const unsigned short* name) = 0;   // 13
    virtual long __stdcall QueryVendorInfo(unsigned short** info) = 0;         // 14
};
struct IAMovieSetup : IUnknown {
    virtual long __stdcall Register() = 0;       // 3 (+0x0c)
    virtual long __stdcall Unregister() = 0;     // 4 (+0x10)
};
struct CUnknown : INonDelegatingUnknown {        // strmbase combase.h (CBaseObject is empty in release)
    IUnknown* m_pUnknown;                        // +0x04
    long m_cRef;                                 // +0x08
    virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
};
class CBaseFilter : public CUnknown, public IBaseFilter, public IAMovieSetup {   // IBaseFilter +0x0C, IAMovieSetup +0x10
public:
    long m_State;                                // +0x14 FILTER_STATE
    IReferenceClock* m_pClock;                   // +0x18
    __int64 m_tStart;                            // +0x20 CRefTime
    GUID m_clsid;                                // +0x28
    CCritSec* m_pLock;                           // +0x38
    unsigned short* m_pName;                     // +0x3c
    IFilterGraph* m_pGraph;                      // +0x40
    IMediaEventSink* m_pSink;                    // +0x44
    long m_PinVersion;                           // +0x48
    virtual AMOVIESETUP_FILTER* GetSetupData();  // primary slot 8 (+0x20)
    long __stdcall SetSyncSource(IReferenceClock* clock);
    long __stdcall GetSyncSource(IReferenceClock** clock);
    long __stdcall EnumPins(IEnumPins** ppEnum);
    long __stdcall Register();
    long __stdcall Unregister();
};
extern "C" const GUID CLSID_FilterMapper;        // 0x012AB184
extern "C" const GUID IID_IFilterMapper;         // 0x012A99E4
extern "C" __declspec(dllimport) long __stdcall CoInitialize(void* reserved);
extern "C" __declspec(dllimport) long __stdcall CoCreateInstance(const GUID& clsid, IUnknown* outer, unsigned long context, const GUID& iid, void** ppv);
extern "C" __declspec(dllimport) void __stdcall CoFreeUnusedLibraries();
extern "C" __declspec(dllimport) void __stdcall CoUninitialize();
long __stdcall AMovieSetupRegisterFilter(const AMOVIESETUP_FILTER* setup, IFilterMapper* mapper, int doRegister);   // 0x00CAA4A0
long __stdcall CBaseFilter::Register()
{
    AMOVIESETUP_FILTER* psetupdata = GetSetupData();
    if (0 == psetupdata) return 1;
    long hr = CoInitialize(0);
    IFilterMapper* pIFM;
    hr = CoCreateInstance(CLSID_FilterMapper, 0, 1, IID_IFilterMapper, (void**)&pIFM);
    if (hr >= 0) {
        hr = AMovieSetupRegisterFilter(psetupdata, pIFM, 1);
        pIFM->Release();
    }
    CoFreeUnusedLibraries();
    CoUninitialize();
    return 0;
}
