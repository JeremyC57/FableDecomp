// CBaseRenderer::~CBaseRenderer() -- DirectShow strmbase renderer destructor: CBaseRenderer's three vtables, StopStreaming, ClearPendingSample, delete the position pass-through and the input pin, then the members and ~CBaseFilter
// Leaf: ??1CBaseRenderer@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA6370, 216 bytes, thiscall; built under /EHsc (state 6 for the body, 2-0 for the CAMEvent members);
//   seven unwind funclets 0x012169F0 .. 0x01216A32 (landed rows that include this source).
// Identity (proven, chunk 704 label): the DX9 SDK strmbase renbase.cpp destructor. It stores CBaseRenderer's vtables 0x012BF498 / 458 /
//   444 (see CBaseRenderer::CBaseRenderer 0x00CA61D0), ~CBaseVideoRenderer 0x00CA7310 tail-jumps here, and the body is the SDK's:
//   StopStreaming() 0x00CA4E30 and ClearPendingSample() 0x00CA4BF0 (slots 32 / 28 of 0x012BF498, called directly as in any destructor),
//   delete m_pPosition (+0x50; the CUnknown subobject of CRendererPosPassThru is at +8, so the scalar deleting destructor is called
//   through [m_pPosition+8]) and delete m_pInputPin (+0x78, CUnknown at +0), then the CCritSec members (DeleteCriticalSection inline;
//   the funclets call its out-of-line copy 0x00CA4870), the three CAMEvent members (~CAMEvent 0x00CB01B0) and ~CBaseFilter 0x00CAAAC0.
// Layout: the chunk 704 ~CBaseVideoRenderer model of CUnknown / CBaseFilter / CBaseRenderer (AM_NOVTABLE bases, novtable interfaces); the
//   ctlutil.h CPosPassThru (IMediaSeeking, CMediaPosition = IMediaPosition + CUnknown) and amfilter.h CBasePin (CUnknown, IPin,
//   IQualityControl) prefixes for the two deletes.
// History: landed as CVolume_00ca6370 (classes named by address); re-authored on the strmbase hierarchy in naming wave 32 (family dshow).
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
    virtual HRESULT ClearPendingSample();                   // 0x00CA4BF0 (slot 28)
    virtual HRESULT StopStreaming();                        // 0x00CA4E30 (slot 32)
};

struct AM_NOVTABLE IMediaSeeking : public IUnknown { };     // strmif.h (slots not used here)
struct AM_NOVTABLE IDispatch : public IUnknown { };         // oaidl.h
struct AM_NOVTABLE IMediaPosition : public IDispatch { };   // control.h
class AM_NOVTABLE CMediaPosition : public IMediaPosition, public CUnknown { };       // ctlutil.h
class CPosPassThru : public IMediaSeeking, public CMediaPosition { };                // ctlutil.h: CUnknown at +8
class CRendererPosPassThru : public CPosPassThru { };                                // ctlutil.h
struct AM_NOVTABLE IPin : public IUnknown { };              // strmif.h
struct AM_NOVTABLE IMemInputPin : public IUnknown { };      // strmif.h
class AM_NOVTABLE CBasePin : public CUnknown, public IPin, public IQualityControl { };   // amfilter.h: CUnknown at +0
class AM_NOVTABLE CBaseInputPin : public CBasePin, public IMemInputPin { };           // amfilter.h
class CRendererInputPin : public CBaseInputPin { };                                  // renbase.h

CBaseRenderer::~CBaseRenderer()
{
    StopStreaming();
    ClearPendingSample();

    // Delete any IMediaPosition implementation
    if (m_pPosition) {
        delete m_pPosition;
        m_pPosition = 0;
    }

    // Delete any input pin created
    if (m_pInputPin) {
        delete m_pInputPin;
        m_pInputPin = 0;
    }
}
