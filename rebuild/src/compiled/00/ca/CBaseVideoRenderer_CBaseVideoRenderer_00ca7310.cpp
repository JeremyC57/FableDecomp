// CBaseVideoRenderer::~CBaseVideoRenderer() -- DirectShow base-class video renderer destructor: the empty SDK body (its ASSERT compiles away) stores this class's five vtables, then the CBaseRenderer destructor runs
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/renbase.cpp) ??1CBaseVideoRenderer@@UAE@XZ -- 45 bytes verified against Fable.exe 0x00CA7310
// Leaf: ??1CBaseVideoRenderer@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA7310, 45 bytes, thiscall, tail jump to ~CBaseRenderer.
// Identity (proven): the DirectShow strmbase renbase.cpp (DX9 SDK) CBaseVideoRenderer destructor. (1) The five vtables it stores
//   are CBaseVideoRenderer's: 0x012BF5D8 (+0x00) has slot 0 = the landed CBaseVideoRenderer::NonDelegatingQueryInterface 0x00CA6080
//   and slot 3 = this class's ??_G 0x00CA73D0 (the caller that inlines nothing but this dtor + delete); 0x012BF560 (+0xE0, the
//   IQualProp subobject) holds the landed CBaseVideoRenderer::get_FramesDroppedInRenderer 0x00CA5D50 .. getDevSyncOffset 0x00CA6020;
//   0x012BF54C (+0xE4) is IQualityControl. (2) The base it chains to, 0x00CA6370, is the SDK ~CBaseRenderer body (StopStreaming
//   0x00CA4E30, ClearPendingSample 0x00CA4BF0, delete m_pPosition / m_pInputPin, the three CAMEvent and three CCritSec members).
//   The ctor 0x00CA7360 stores the same five vtables after CBaseRenderer's ctor 0x00CA61D0. The catalog label "CTCStealth::OnCreate"
//   is wrong; the derived Fable renderer's dtor (0x00A3B9C4, CTextureRenderer) tail-jumps here too.
// Callee: CBaseRenderer::~CBaseRenderer() 0x00CA6370 (virtual through CUnknown's virtual destructor).
// Layout: the SDK classes: CUnknown {vptr, m_pUnknown, m_cRef} (CBaseObject is empty in a release build), IBaseFilter vptr +0x0C,
//   IAMovieSetup vptr +0x10, CBaseFilter members to +0x4C (8-aligned by CRefTime), CBaseRenderer members +0x50..+0xDC (sizeof 0xE0),
//   IQualProp +0xE0, IQualityControl +0xE4, then the CBaseVideoRenderer statistics (+0xE8 on; only their extent matters here).
//   The COM interfaces are MIDL structs with __declspec(novtable); CUnknown and CBaseFilter are AM_NOVTABLE classes.
// Layout: CBaseObject here is the DirectShow strmbase class (combase.h), not the engine class of engine/CBaseObject.h: the names
//   collide, the engine header does not apply.
#define AM_NOVTABLE __declspec(novtable)
struct GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};
typedef long HRESULT;
typedef long BOOL;
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
    ~CBaseFilter();
};
class CAMEvent {
protected:
    void* m_hEvent;                                         // HANDLE
public:
    ~CAMEvent();
};
class CCritSec {
    unsigned char m_CritSec[0x18];                          // CRITICAL_SECTION
public:
    ~CCritSec();
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
public:
    virtual ~CBaseRenderer();                               // 0x00CA6370
};
class CBaseVideoRenderer : public CBaseRenderer, public IQualProp, public IQualityControl {
protected:
    int  m_nNormal;                                         // +0xE8
    BOOL m_bSupplierHandlingQuality;                        // +0xEC
    unsigned char m_stats[0x128 - 0xF0];                    // +0xF0 the timing statistics (m_cFramesDropped +0x120, m_cFramesDrawn +0x124)
public:
    virtual ~CBaseVideoRenderer();
};

CBaseVideoRenderer::~CBaseVideoRenderer()
{
}