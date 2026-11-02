// CBaseRenderer::CBaseRenderer(REFCLSID, LPCTSTR, LPUNKNOWN, HRESULT*) -- DirectShow strmbase renderer constructor: CBaseFilter on the interface lock, CBaseRenderer's three vtables, the init list, the three critical sections, then Ready()
// Leaf: ??0CBaseRenderer@@QAE@ABU_GUID@@PBGPAUIUnknown@@PAJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA61D0, 248 bytes, thiscall, `ret 10h`; built under /EHsc (states 0-2 for the three CAMEvent members).
// Identity (proven, chunk 704 suggestion): the DX9 SDK strmbase renbase.cpp constructor. (1) The proven CBaseVideoRenderer ctor 0x00CA7360
//   calls it as its base (CBaseVideoRenderer : CBaseRenderer). (2) It stores 0x012BF498 (+0x00) / 0x012BF458 (+0x0C) / 0x012BF444 (+0x10):
//   0x012BF458 is CBaseRenderer's IBaseFilter vtable (the landed CBaseFilter::QueryInterface 0x00CA62D0: slots CBaseRenderer::Stop /
//   Pause / Run / GetState / FindPin) and 0x012BF498 slot 0 is CBaseRenderer::NonDelegatingQueryInterface 0x00CA4780; the destructor
//   storing them is ~CBaseRenderer 0x00CA6370 (chunk 704). (3) The body is the SDK's: CBaseFilter(pName, pUnk, &m_InterfaceLock (+0x7C),
//   RenderClass), m_RenderEvent / m_ThreadSignal(TRUE) / m_evComplete(TRUE), the zeroed state, m_bRepaintStatus(TRUE) +0xB0, the three
//   CCritSec members (InitializeCriticalSection, the import cached in a register) and Ready() (SetEvent on m_evComplete +0x5C).
// Callees: CBaseFilter::CBaseFilter 0x00CAC830, CAMEvent::CAMEvent 0x00CB0190, InitializeCriticalSection / SetEvent (imports).
// Layout: the chunk 704 ~CBaseVideoRenderer model of CUnknown / CBaseFilter / CBaseRenderer (AM_NOVTABLE bases, novtable interfaces).
// History: landed as CVolume_00ca61d0 (classes named by address, Leaf ??0CHeroExperienceDef@); re-authored on the strmbase hierarchy in
//   naming wave 32 (family dshow).
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(void* section);
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);
#define TRUE 1
#define AM_NOVTABLE __declspec(novtable)
typedef struct _GUID {                                      // guiddef.h: REFCLSID = const IID& = const _GUID&
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;
#define FALSE 0                                             // windef.h
typedef unsigned short WCHAR;                               // VC7.1 wchar_t (no /Zc:wchar_t); UNICODE strmbase: LPCTSTR = const WCHAR*
typedef long HRESULT;
typedef int BOOL;                                           // windef.h
typedef __int64 REFERENCE_TIME;
struct IPin;
struct IEnumPins;
struct IFilterGraph;
struct IReferenceClock;
struct IMediaEventSink;
struct IMediaSample;
struct IQualityControlSink;
class CCritSec;
class CRendererPosPassThru;
class CRendererInputPin;

struct AM_NOVTABLE IUnknown {
    virtual HRESULT __stdcall QueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};
struct AM_NOVTABLE IPersist : public IUnknown {
    virtual HRESULT __stdcall GetClassID(GUID* pClassID) = 0;
};
struct AM_NOVTABLE IMediaFilter : public IPersist {
    virtual HRESULT __stdcall Stop() = 0;
    virtual HRESULT __stdcall Pause() = 0;
    virtual HRESULT __stdcall Run(REFERENCE_TIME tStart) = 0;
    virtual HRESULT __stdcall GetState(unsigned long dwMilliSecsTimeout, int* State) = 0;
    virtual HRESULT __stdcall SetSyncSource(IReferenceClock* pClock) = 0;
    virtual HRESULT __stdcall GetSyncSource(IReferenceClock** pClock) = 0;
};
struct AM_NOVTABLE IBaseFilter : public IMediaFilter {
    virtual HRESULT __stdcall EnumPins(IEnumPins** ppEnum) = 0;
    virtual HRESULT __stdcall FindPin(const unsigned short* Id, IPin** ppPin) = 0;
    virtual HRESULT __stdcall QueryFilterInfo(void* pInfo) = 0;
    virtual HRESULT __stdcall JoinFilterGraph(IFilterGraph* pGraph, const unsigned short* pName) = 0;
    virtual HRESULT __stdcall QueryVendorInfo(unsigned short** pVendorInfo) = 0;
};
struct AM_NOVTABLE IAMovieSetup : public IUnknown {
    virtual HRESULT __stdcall Register() = 0;
    virtual HRESULT __stdcall Unregister() = 0;
};
struct AM_NOVTABLE IQualProp : public IUnknown {
    virtual HRESULT __stdcall get_FramesDroppedInRenderer(int* pcFrames) = 0;
    virtual HRESULT __stdcall get_FramesDrawn(int* pcFramesDrawn) = 0;
    virtual HRESULT __stdcall get_AvgFrameRate(int* piAvgFrameRate) = 0;
    virtual HRESULT __stdcall get_Jitter(int* iJitter) = 0;
    virtual HRESULT __stdcall get_AvgSyncOffset(int* piAvg) = 0;
    virtual HRESULT __stdcall get_DevSyncOffset(int* piDev) = 0;
};
struct AM_NOVTABLE IQualityControl : public IUnknown {
    virtual HRESULT __stdcall Notify(void* pSelf, int q) = 0;
    virtual HRESULT __stdcall SetSink(IQualityControl* piqc) = 0;
};
struct AM_NOVTABLE INonDelegatingUnknown {
    virtual HRESULT __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};

class CBaseObject {                                         // release build: no members
public:
    ~CBaseObject();
};
class AM_NOVTABLE CUnknown : public INonDelegatingUnknown, public CBaseObject {
private:
    IUnknown* const m_pUnknown;                             // +0x04
protected:
    long m_cRef;                                            // +0x08
public:
    virtual ~CUnknown() { }
};
class CRefTime {
public:
    REFERENCE_TIME m_time;
};
class AM_NOVTABLE CBaseFilter : public CUnknown, public IBaseFilter, public IAMovieSetup {
protected:
    int              m_State;                               // +0x14 FILTER_STATE
    IReferenceClock* m_pClock;                              // +0x18
    CRefTime         m_tStart;                              // +0x20
    GUID             m_clsid;                               // +0x28
    CCritSec*        m_pLock;                               // +0x38
    unsigned short*  m_pName;                               // +0x3C
    IFilterGraph*    m_pGraph;                              // +0x40
    IMediaEventSink* m_pSink;                               // +0x44
    long             m_PinVersion;                          // +0x48
public:
    CBaseFilter(const WCHAR* pName, IUnknown* pUnk, CCritSec* pLock, const GUID& clsid);   // 0x00CAC830
    ~CBaseFilter();
};
class CAMEvent {                                            // wxutil.h (DX9: one-argument ctor)
protected:
    void* m_hEvent;                                         // HANDLE
public:
    CAMEvent(BOOL fManualReset = FALSE);                    // 0x00CB0190
    ~CAMEvent();                                            // 0x00CB01B0
    void Set() { SetEvent(m_hEvent); }                      // EXECUTE_ASSERT(SetEvent(m_hEvent)) in a release build
};
class CCritSec {                                            // wxutil.h
    unsigned char m_CritSec[0x18];                          // CRITICAL_SECTION
public:
    CCritSec() { InitializeCriticalSection(m_CritSec); }
    ~CCritSec() { DeleteCriticalSection(m_CritSec); }
};
class CBaseRenderer : public CBaseFilter {
protected:
    CRendererPosPassThru* m_pPosition;                      // +0x50
    CAMEvent              m_RenderEvent;                    // +0x54
    CAMEvent              m_ThreadSignal;                   // +0x58
    CAMEvent              m_evComplete;                     // +0x5C
    BOOL                  m_bAbort;                         // +0x60
    BOOL                  m_bStreaming;                     // +0x64
    unsigned long         m_dwAdvise;                       // +0x68
    IMediaSample*         m_pMediaSample;                   // +0x6C
    BOOL                  m_bEOS;                           // +0x70
    BOOL                  m_bEOSDelivered;                  // +0x74
    CRendererInputPin*    m_pInputPin;                      // +0x78
    CCritSec              m_InterfaceLock;                  // +0x7C
    CCritSec              m_RendererLock;                   // +0x94
    IQualityControl*      m_pQSink;                         // +0xAC
    BOOL                  m_bRepaintStatus;                 // +0xB0
    BOOL                  m_bInReceive;                     // +0xB4
    REFERENCE_TIME        m_SignalTime;                     // +0xB8
    unsigned int          m_EndOfStreamTimer;               // +0xC0
    CCritSec              m_ObjectCreationLock;             // +0xC4
    void Ready() { m_evComplete.Set(); }
public:
    CBaseRenderer(const GUID& RenderClass, const WCHAR* pName, IUnknown* pUnk, HRESULT* phr);   // 0x00CA61D0
    virtual ~CBaseRenderer();                               // 0x00CA6370
};

CBaseRenderer::CBaseRenderer(const GUID& RenderClass, const WCHAR* pName, IUnknown* pUnk, HRESULT* phr)
    : CBaseFilter(pName, pUnk, &m_InterfaceLock, RenderClass),
      m_evComplete(TRUE),
      m_bAbort(FALSE),
      m_pPosition(0),
      m_ThreadSignal(TRUE),
      m_bStreaming(FALSE),
      m_bEOS(FALSE),
      m_bEOSDelivered(FALSE),
      m_pMediaSample(0),
      m_dwAdvise(0),
      m_pQSink(0),
      m_pInputPin(0),
      m_bRepaintStatus(TRUE),
      m_SignalTime(0),
      m_bInReceive(FALSE),
      m_EndOfStreamTimer(0)
{
    Ready();
}
