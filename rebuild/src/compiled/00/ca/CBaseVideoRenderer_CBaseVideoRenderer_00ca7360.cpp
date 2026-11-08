// CBaseVideoRenderer::CBaseVideoRenderer(REFCLSID, LPCTSTR, LPUNKNOWN, HRESULT*) -- DirectShow strmbase video renderer constructor: CBaseRenderer from the four arguments, this class's five vtables, the three counters cleared, then ResetStreamingTimes()
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/renbase.cpp) ??0CBaseVideoRenderer@@QAE@ABU_GUID@@PAGPAUIUnknown@@PAJ@Z -- 103 bytes verified against Fable.exe 0x00CA7360
// Leaf: ??0CBaseVideoRenderer@@QAE@ABU_GUID@@PBGPAUIUnknown@@PAJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA7360, 103 bytes, thiscall, `ret 10h`; no EH frame (default flags).
// Identity (proven): the DX9 SDK strmbase renbase.cpp constructor. (1) It stores the same five vtables as the chunk 704 ~CBaseVideoRenderer
//   0x00CA7310 (0x012BF5D8 +0x00, 0x012BF598 +0x0C, 0x012BF584 +0x10, 0x012BF560 +0xE0, 0x012BF54C +0xE4). (2) Its base call 0x00CA61D0
//   stores CBaseRenderer's vtables 0x012BF498 / 458 / 444 (the landed CBaseFilter::QueryInterface 0x00CA62D0 names 0x012BF458 CBaseRenderer's
//   IBaseFilter vtable) and is the SDK CBaseRenderer ctor body. (3) The body is renbase.cpp's: init list m_bSupplierHandlingQuality(FALSE)
//   +0xEC, m_cFramesDropped(0) +0x120, m_cFramesDrawn(0) +0x124 (emitted in declaration order), then ResetStreamingTimes() -- virtual
//   (slot 47 of 0x012BF5D8), called directly as in any constructor. (4) Its only caller is CTextureRenderer::CTextureRenderer 0x00A3B510
//   (CLSID 0x0129D150, name L"Texture Renderer": UNICODE strmbase, LPCTSTR = const WCHAR*). The catalog label "CSpellContainerList::
//   CSpellContainerList" and the CVolumeDerived_00ca7360 model were wrong.
// Callees: CBaseRenderer::CBaseRenderer 0x00CA61D0, CBaseVideoRenderer::ResetStreamingTimes 0x00CA52A0.
// Layout: the chunk 704 ~CBaseVideoRenderer model (CBaseRenderer to +0xE0, IQualProp +0xE0, IQualityControl +0xE4); the CBaseVideoRenderer
//   members are renbase.h's (DX9, no PERF): m_nNormal +0xE8 .. m_tStreamingStart +0x158, sizeof 0x160 (CTextureRenderer's members start at
//   +0x160, Ego_r.pdb).
// History: landed as CVolumeDerived_00ca7360 (classes named by address); re-authored on the strmbase hierarchy in naming wave 32 (dshow).
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
    CBaseRenderer(const GUID& RenderClass, const WCHAR* pName, IUnknown* pUnk, HRESULT* phr);   // 0x00CA61D0
    virtual ~CBaseRenderer();                               // 0x00CA6370
};
class CBaseVideoRenderer : public CBaseRenderer, public IQualProp, public IQualityControl {
protected:
    int            m_nNormal;                               // +0xE8
    BOOL           m_bSupplierHandlingQuality;              // +0xEC
    int            m_trThrottle;                            // +0xF0
    int            m_trRenderAvg;                           // +0xF4
    int            m_trRenderLast;                          // +0xF8
    int            m_tRenderStart;                          // +0xFC
    int            m_trEarliness;                           // +0x100
    int            m_trTarget;                              // +0x104
    int            m_trWaitAvg;                             // +0x108
    int            m_trFrameAvg;                            // +0x10C
    int            m_trDuration;                            // +0x110
    REFERENCE_TIME m_trRememberStampForPerf;                // +0x118
    int            m_cFramesDropped;                        // +0x120
    int            m_cFramesDrawn;                          // +0x124
    __int64        m_iTotAcc;                               // +0x128
    __int64        m_iSumSqAcc;                             // +0x130
    REFERENCE_TIME m_trLastDraw;                            // +0x138
    __int64        m_iSumSqFrameTime;                       // +0x140
    __int64        m_iSumFrameTime;                         // +0x148
    int            m_trLate;                                // +0x150
    int            m_trFrame;                               // +0x154
    int            m_tStreamingStart;                       // +0x158
public:
    CBaseVideoRenderer(const GUID& RenderClass, const WCHAR* pName, IUnknown* pUnk, HRESULT* phr);
    virtual ~CBaseVideoRenderer();                          // 0x00CA7310
    virtual HRESULT ResetStreamingTimes();                  // 0x00CA52A0 (slot 47)
};

CBaseVideoRenderer::CBaseVideoRenderer(const GUID& RenderClass, const WCHAR* pName, IUnknown* pUnk, HRESULT* phr)
    : CBaseRenderer(RenderClass, pName, pUnk, phr),
      m_cFramesDropped(0),
      m_cFramesDrawn(0),
      m_bSupplierHandlingQuality(FALSE)
{
    ResetStreamingTimes();
}
