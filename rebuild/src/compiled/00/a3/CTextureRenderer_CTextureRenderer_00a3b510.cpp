// CTextureRenderer::CTextureRenderer(LPUNKNOWN, HRESULT*, CTexture*) -- DirectShow video renderer that draws into a texture (own texture when none is given)
// Leaf: ??0CTextureRenderer@@QAE@PAUIUnknown@@PAJPAVCTexture@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3B510, 113 bytes (+15 int3). thiscall, `ret 0Ch`. VIDEO/RENDER LAYER (DirectShow
//         strmbase + D3D9 texture; replacement candidate).
// Identity: Ego_r.pdb / FableWin.pdb ??0CTextureRenderer@@QAE@PAUIUnknown@@PAJPAVCTexture@@@Z; RTTI .?AVCTextureRenderer@@.
// Callees: CBaseVideoRenderer::CBaseVideoRenderer(REFCLSID, LPCTSTR, LPUNKNOWN, HRESULT*) 0x00CA7360 with CLSID 0x0129D150
//          and name L"Texture Renderer" 0x0129D160 (UNICODE strmbase). Vtables: 0x0129D08C +0x00 (CUnknown), 0x0129D04C +0x0C
//          (IBaseFilter), 0x0129D030 +0x10 (IAMovieSetup), 0x0129D008 +0xE0 (IQualProp), 0x0129CFF0 +0xE4 (IQualityControl).
// Shape: the five vtable stores, init list PVideoSys(0), CreateOwnTexture(false) through one `xor ecx,ecx`; the body stores
//        *phr = S_OK, Texture = texture and sets CreateOwnTexture when no texture was passed.
// Layout: the strmbase classes of CBaseVideoRenderer::CBaseVideoRenderer 0x00CA7360 (the chunk 704 model; sizeof 0x160); Ego_r.pdb
//         CTextureRenderer members from +0x160: PVideoSys, Texture +0x164, TextureFormat +0x168, Width +0x16C, Height +0x170,
//         Pitch +0x174, CreateOwnTexture +0x178.
// History: landed on CUnknownPart / CBaseFilterPart stand-ins with `struct GUID` (the vtable spelled ??_7CTextureRenderer@@6BCUnknownPart@@@,
//   the base ctor ...ABUGUID@@...); re-authored on the strmbase hierarchy in naming wave 32 (family dshow).
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
struct CVideoSys;
class CTexture;
enum _D3DFORMAT;
class CTextureRenderer : public CBaseVideoRenderer {
public:
    CVideoSys*  PVideoSys;                                  // +0x160
    CTexture*   Texture;                                    // +0x164
    _D3DFORMAT  TextureFormat;                              // +0x168
    int         Width;                                      // +0x16C
    int         Height;                                     // +0x170
    int         Pitch;                                      // +0x174
    bool        CreateOwnTexture;                           // +0x178
    CTextureRenderer(IUnknown* pUnk, HRESULT* phr, CTexture* texture);
};
extern "C" const GUID CLSID_TextureRenderer;                // 0x0129D150

CTextureRenderer::CTextureRenderer(IUnknown* pUnk, HRESULT* phr, CTexture* texture)
    : CBaseVideoRenderer(CLSID_TextureRenderer, L"Texture Renderer", pUnk, phr), PVideoSys(0), CreateOwnTexture(false)
{
    *phr = 0;
    Texture = texture;
    if (!texture)
        CreateOwnTexture = true;
}
