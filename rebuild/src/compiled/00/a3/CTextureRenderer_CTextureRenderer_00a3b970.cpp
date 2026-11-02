// CTextureRenderer::~CTextureRenderer() -- DirectShow texture renderer filter destructor: delete the texture when the renderer owns it (inline ~CTexture -> Uninitialise), then ~CBaseVideoRenderer
// Leaf: ??1CTextureRenderer@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3B970, int3 padding before and after, thiscall, tail `jmp ~CBaseVideoRenderer` 0x00CA7310.
// Identity (proven, two sources): (1) it stores vtable 0x0129D08C (RTTI .?AVCTextureRenderer@@ : CBaseVideoRenderer : CBaseRenderer
//   : CBaseFilter : CUnknown ...) and its sole caller is the scalar deleting destructor 0x00A3BDC0 at slot 3 of that vtable (the
//   CUnknown slot of the virtual dtor), which this TU's ??_G matches (RELOCATION_MATCH); (2) Ego_r.pdb 0x00B96330 / FableWin.pdb
//   0x0320C610 ??1CTextureRenderer@@ with Ego_r members Texture / CreateOwnTexture. Spelling decision: both PDBs spell it QAE
//   (non-virtual) because in those builds CTextureRenderer was a 28-byte class with no base; in retail it derives from
//   CBaseVideoRenderer, whose virtual dtor makes this one virtual, and VC7.1 mangles an implicitly virtual dtor UAE (test-compiled).
// Callees: CTexture::Uninitialise 0x009F9F70 (?Uninitialise@CTexture@@QAEXXZ, via the inline ~CTexture), operator delete
//   0x00BFE9BC, ??1CBaseVideoRenderer@@UAE@XZ 0x00CA7310 (tail).
// Shape: `if (Texture && CreateOwnTexture) delete Texture;`, then the base dtor.
// Layout: strmbase (DirectShow SDK baseclasses, AM_NOVTABLE MIDL interfaces) as landed 0x00A3B510 with CBaseVideoRenderer at its
//   retail 0x160 bytes (the landed 0x00CA7310 model truncates it at 0x128); members PVideoSys +0x160 .. CreateOwnTexture +0x178,
//   sizeof 0x180 (8-aligned; the only new, 0x00A3BA79, pushes 180h). CTexture is engine/CTexture.h FABLE_RETAIL_CTEXTURE (chunk 715 overlay: inline ~CTexture() { Uninitialise(); }).
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

class CMyBaseObject {                                       // strmbase CBaseObject, renamed in retail: RTTI .?AVCMyBaseObject@@ at +4 of
public:                                                     //   CTextureRenderer 0x0129D08C (avoids the engine CBaseObject); release build: no members
    ~CMyBaseObject();
};
class AM_NOVTABLE CUnknown : public INonDelegatingUnknown, public CMyBaseObject {
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
class CVideoSys;
#define FABLE_RETAIL_CTEXTURE
#include "engine/CTexture.h"
enum _D3DFORMAT;
class CTextureRenderer : public CBaseVideoRenderer {        // vtable 0x0129D08C ??_7CTextureRenderer@@6BCUnknown@@@ (landed 0x00A3B510)
public:
    CTextureRenderer(IUnknown* pUnk, HRESULT* phr, CTexture* texture);   // 0x00A3B510 ??0CTextureRenderer@@QAE@PAUIUnknown@@PAJPAVCTexture@@@Z
    ~CTextureRenderer();                                    // virtual through ~CBaseVideoRenderer: UAE (see header)
    CVideoSys*  PVideoSys;                                  // +0x160
    CTexture*   Texture;                                    // +0x164
    _D3DFORMAT  TextureFormat;                              // +0x168
    int         Width;                                      // +0x16C
    int         Height;                                     // +0x170
    int         Pitch;                                      // +0x174
    bool        CreateOwnTexture;                           // +0x178
};
FABLE_STATIC_ASSERT(sizeof(CTextureRenderer) == 0x180);       // retail `push 180h` before the ctor call 0x00A3BA98

CTextureRenderer::~CTextureRenderer()
{
    if (Texture && CreateOwnTexture)
        delete Texture;
}