// CBaseFilter::EnumPins(IEnumPins**) -- DirectShow base class: hand out a new ref-counted pin enumerator
// Leaf: EnumPins
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAC890, 134 bytes (+10 int3). __stdcall (STDMETHODIMP, `ret 8`), /EHsc frame.
// Class: catalog "CTextureRenderer::vfunc_10" -- slot 10 of CTextureRenderer's IBaseFilter vtable 0x0129D04C; the body is
//        strmbase amfilter.cpp CBaseFilter::EnumPins with `this` at the IBaseFilter subobject (+0x0C; `add ecx,-0Ch` to pass
//        the CBaseFilter*). VIDEO LAYER (DirectShow strmbase, replacement candidate).
// Callees: operator new(size_t) 0x00BFEA1A (0x30 bytes); CEnumPins::CEnumPins(CBaseFilter*, CEnumPins*) 0x00CABE00.
// Shape: CheckPointer(ppEnum, E_POINTER); `*ppEnum = new CEnumPins(this, NULL);` (EH state 0 around the ctor, the new
//        result kept in the dead ppEnum slot); `return *ppEnum == NULL ? E_OUTOFMEMORY : NOERROR;` from the register
//        (`setne dl ; dec edx ; and edx,8007000Eh`).
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
void* __cdecl operator new(unsigned int size);   // 0x00BFEA1A
void __cdecl operator delete(void* p);           // 0x00BFE9BC
class CEnumPins : public IEnumPins {             // strmbase amfilter.h, 0x30 bytes
    unsigned char _body[0x30 - 4];
public:
    CEnumPins(CBaseFilter* filter, CEnumPins* enumPins);   // 0x00CABE00 (catalog "CVsyncCallbackTimer" is a byte twin)
    long __stdcall QueryInterface(const GUID& riid, void** ppv);
    unsigned long __stdcall AddRef();
    unsigned long __stdcall Release();
};
long __stdcall CBaseFilter::EnumPins(IEnumPins** ppEnum)
{
    if (ppEnum == 0)
        return (long)0x80004003;
    *ppEnum = new CEnumPins(this, 0);
    return *ppEnum == 0 ? (long)0x8007000E : 0;
}
