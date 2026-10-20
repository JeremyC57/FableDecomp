// CBaseRenderer::Stop() -- DirectShow base class (strmbase renbase.cpp): under the interface lock, leave a stopped filter alone, stop an unconnected one by state only, else run CBaseFilter::Stop, decommit the input pin's allocator, flag a repaint, stop streaming, wake the source thread, reset end-of-stream and the clock advise, signal completion and clear the abort flag
// Leaf: Stop
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA66D0, 219 bytes (+5 int3). __stdcall (STDMETHODIMP, `ret 4`), /EHsc frame
//         (handler 0x01216A78).
// Class: catalog "CTextureRenderer::vfunc_4" -- slot 4 of CTextureRenderer's IBaseFilter vtable 0x0129D04C (IMediaFilter::Stop);
//        the body is strmbase CBaseRenderer::Stop compiled with `this` at the IBaseFilter subobject (+0x0C), so m_State reads
//        [this+8], m_pInputPin [this+0x6C], m_InterfaceLock is this+0x70 and the primary-vtable virtuals run on `this-0x0C`.
//        VIDEO LAYER (DirectShow strmbase, replacement candidate).
// Callees: EnterCriticalSection / LeaveCriticalSection through the IAT (0x0143FE0C / 0x0143FE10; the Leave import held in ebx
//          once it is used twice), CBaseFilter::Stop 0x00CAAB30 (`__stdcall`, the IBaseFilter subobject pushed; manifest label
//          propagated), IMemAllocator::Decommit (slot 6), SetEvent through the IAT 0x0143FDE0 (the inline CAMEvent::Set of
//          Ready()), CBaseRenderer::WaitForReceiveToComplete 0x00CA47D0 (manifest "ThreadMsg_PumpAndWaitForCompletion" is
//          propagated); primary-vtable virtuals StopStreaming (slot 32), SourceThreadCanWait(bool) (10), ResetEndOfStream (25),
//          CancelNotification (27).
// Shape: the outer CAutoLock takes EH state 0 after its Lock (its pointer kept in ebp and in the frame); the two early returns
//        unlock and jump to the shared `xor eax,eax` epilogue; SetRepaintStatus(TRUE) is inlined (the renderer lock enters and
//        leaves around one dword store, no EH state of its own since nothing in between can throw).
// Layout: strmbase CBaseFilter: CUnknown 0x0C, IBaseFilter +0x0C, IAMovieSetup +0x10, m_State +0x14 ... 0x48; CBaseRenderer:
//         m_evComplete +0x5C, m_bAbort +0x60, m_pInputPin +0x78, m_InterfaceLock +0x7C, m_RendererLock +0x94,
//         m_bRepaintStatus +0xB0; CBasePin::m_Connected +0x18, CBaseInputPin::m_pAllocator +0x9C.
struct GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
struct IUnknown {
    virtual long __stdcall QueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
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
class CAMEvent {                                 // strmbase wxutil.h
    void* m_hEvent;
public:
    void Set() { SetEvent(m_hEvent); }
};
struct IReferenceClock; struct IEnumPins; struct IPin; struct IFilterGraph; struct IMediaEventSink; struct FILTER_INFO; struct AMOVIESETUP_FILTER;
struct IBaseFilter : IUnknown {                  // IPersist + IMediaFilter + IBaseFilter
    virtual long __stdcall GetClassID(GUID* clsid) = 0;                        // 3
    virtual long __stdcall Stop() = 0;                                         // 4 (+0x10)
    virtual long __stdcall Pause() = 0;                                        // 5
    virtual long __stdcall Run(__int64 start) = 0;                             // 6
    virtual long __stdcall GetState(unsigned long timeout, long* state) = 0;   // 7
    virtual long __stdcall SetSyncSource(IReferenceClock* clock) = 0;          // 8
    virtual long __stdcall GetSyncSource(IReferenceClock** clock) = 0;         // 9
    virtual long __stdcall EnumPins(IEnumPins** ppEnum) = 0;                   // 10
    virtual long __stdcall FindPin(const unsigned short* id, IPin** pin) = 0;  // 11
    virtual long __stdcall QueryFilterInfo(FILTER_INFO* info) = 0;             // 12
    virtual long __stdcall JoinFilterGraph(IFilterGraph* graph, const unsigned short* name) = 0;   // 13
    virtual long __stdcall QueryVendorInfo(unsigned short** info) = 0;         // 14
};
struct IAMovieSetup : IUnknown {
    virtual long __stdcall Register() = 0;       // 3
    virtual long __stdcall Unregister() = 0;     // 4
};
struct IMemAllocator : IUnknown {
    virtual long __stdcall SetProperties(void* request, void* actual) = 0;     // 3
    virtual long __stdcall GetProperties(void* props) = 0;                     // 4
    virtual long __stdcall Commit() = 0;                                       // 5
    virtual long __stdcall Decommit() = 0;                                     // 6 (+0x18)
};
struct CBasePin {
    unsigned char _pad00[0x18];
    IPin* m_Connected;                           // +0x18
};
struct CRendererInputPin : CBasePin {            // CBaseInputPin
    unsigned char _pad1c[0x9c - 0x1c];
    IMemAllocator* m_pAllocator;                 // +0x9c
    int IsConnected() { return m_Connected != 0; }
    IMemAllocator* Allocator() const { return m_pAllocator; }
};
struct CUnknown : INonDelegatingUnknown {        // strmbase combase.h (CBaseObject is empty in release)
    IUnknown* m_pUnknown;                        // +0x04
    long m_cRef;                                 // +0x08
    virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
};
class CBaseFilter : public CUnknown, public IBaseFilter, public IAMovieSetup {   // IBaseFilter +0x0C, IAMovieSetup +0x10
public:
    long m_State;                                // +0x14 FILTER_STATE (0 = State_Stopped)
    IReferenceClock* m_pClock;                   // +0x18
    __int64 m_tStart;                            // +0x20 CRefTime
    GUID m_clsid;                                // +0x28
    CCritSec* m_pLock;                           // +0x38
    unsigned short* m_pName;                     // +0x3c
    IFilterGraph* m_pGraph;                      // +0x40
    IMediaEventSink* m_pSink;                    // +0x44
    long m_PinVersion;                           // +0x48
    virtual AMOVIESETUP_FILTER* GetSetupData();  // primary slot 8 (+0x20)
    virtual void v09();
    long __stdcall Stop();                       // 0x00CAAB30
};
class CBaseRenderer : public CBaseFilter {
public:
    virtual long SourceThreadCanWait(int canWait);   // primary slot 10 (+0x28)
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void v23(); virtual void v24();
    virtual long ResetEndOfStream();             // primary slot 25 (+0x64)
    virtual void v26();
    virtual long CancelNotification();           // primary slot 27 (+0x6c)
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual long StopStreaming();                // primary slot 32 (+0x80)
    unsigned char _pad50[0x5c - 0x50];              // CBaseFilter is 0x50 (8-aligned by m_tStart)
    CAMEvent m_evComplete;                       // +0x5c
    int m_bAbort;                                // +0x60
    unsigned char _pad64[0x78 - 0x64];
    CRendererInputPin* m_pInputPin;              // +0x78
    CCritSec m_InterfaceLock;                    // +0x7c
    CCritSec m_RendererLock;                     // +0x94
    unsigned char _padac[0xb0 - 0xac];
    int m_bRepaintStatus;                        // +0xb0
    void SetRepaintStatus(int repaint)
    {
        CAutoLock cRendererLock(&m_RendererLock);
        m_bRepaintStatus = repaint;
    }
    void Ready() { m_evComplete.Set(); }
    void WaitForReceiveToComplete();             // 0x00CA47D0
    long __stdcall Stop();
};
long __stdcall CBaseRenderer::Stop()
{
    CAutoLock cRendererLock(&m_InterfaceLock);
    if (m_State == 0) {
        return 0;
    }
    if (m_pInputPin->IsConnected() == 0) {
        m_State = 0;
        return 0;
    }
    CBaseFilter::Stop();
    if (m_pInputPin->Allocator()) {
        m_pInputPin->Allocator()->Decommit();
    }
    SetRepaintStatus(1);
    StopStreaming();
    SourceThreadCanWait(0);
    ResetEndOfStream();
    CancelNotification();
    Ready();
    WaitForReceiveToComplete();
    m_bAbort = 0;
    return 0;
}
