// CBaseVideoRenderer::QueryInterface(REFIID riid, void** ppv) -- DirectShow BaseClasses DECLARE_IUNKNOWN: forward the request to the owning (outer) IUnknown, GetOwner()->QueryInterface(riid, ppv)
// Leaf: ?QueryInterface@CBaseVideoRenderer@@UAGJABU_GUID@@PAPAX@Z
// Layout: engine/CBaseObject.h is Lionhead's CBaseObject, a different class from DirectShow's combase.h CBaseObject modelled
//   here (an empty, non-polymorphic base of CUnknown in the release build).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3BD00, 15 bytes, __stdcall (STDMETHODIMP, `this` on the stack): `mov eax,[esp+4] ;
//   mov eax,[eax-8] ; mov ecx,[eax] ; mov [esp+4],eax ; jmp [ecx]` -- `this` is the IBaseFilter subobject (+0x0C of the
//   filter), [this-8] is CUnknown::m_pUnk (+0x04), and the outer unknown's slot 0 (QueryInterface) is tail-called with the
//   caller's riid / ppv left in place.
// Identity: NOT a scalar deleting destructor. Slot 0 of the IUnknown-derived secondary vtable 0x0129D04C of CTextureRenderer
//   (RTTI .?AVCTextureRenderer@@) and of CBaseVideoRenderer's own (COL-less) vtable 0x012BF598; its neighbours 0x00A3BD10 /
//   0x00A3BD20 are the same forwarders for slot 1 AddRef (`jmp [ecx+4]`) and slot 2 Release (`jmp [ecx+8]`), and the
//   adjustor thunks 0x00A3BD30 (`sub [esp+4],4`), 0x00A3BD60 (0xD4) and 0x00A3BD90 (0xD8) jump to it from the IAMovieSetup
//   (+0x10), IQualProp (+0xE0) and IQualityControl (+0xE4) vtables. One body serving IBaseFilter, IAMovieSetup, IQualProp and
//   IQualityControl is the final overrider declared by DECLARE_IUNKNOWN in CBaseVideoRenderer (renbase.h; CTextureRenderer
//   declares none, and CBaseFilter's own copy cannot override the IQualProp / IQualityControl branches). The compiled TU emits
//   exactly retail's thunk set for it: ?QueryInterface@CBaseVideoRenderer@@W3AG.. (-4), ..WNE@AG.. (-0xD4), ..WNI@AG.. (-0xD8).
//   The COMDAT sits in the CTextureRenderer TU (0x00A3Bxxx) because that object precedes strmbase's renbase.obj at link time.
// Shape: DirectShow BaseClasses (DX9 SDK combase.h / amfilter.h / renbase.h, UNICODE release build) modelled as in the landed
//   strmbase rows (CBaseRenderer::NonDelegatingQueryInterface 0x00CA4780, CBaseVideoRenderer::NonDelegatingQueryInterface
//   0x00CA6080: CBaseRenderer 0xE0 bytes, IQualProp +0xE0, IQualityControl +0xE4). The inline member is emitted through the
//   vtable of the TU's class: the CTextureRenderer constructor (FableWin.pdb ??0CTextureRenderer@@QAE@PAUIUnknown@@PAJPAVCTexture@@@Z)
//   is defined with an empty body only to emit the vtable (not graded; retail's body is not reconstructed here).
typedef struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; } GUID;
typedef GUID IID;
typedef GUID CLSID;
typedef const IID& REFIID;
typedef const CLSID& REFCLSID;
typedef long HRESULT;
typedef __int64 REFERENCE_TIME;
struct IUnknown {
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppv) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};
typedef IUnknown* LPUNKNOWN;
struct INonDelegatingUnknown {
    virtual HRESULT __stdcall NonDelegatingQueryInterface(REFIID riid, void** ppv) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};
class CBaseObject {
public:
    CBaseObject(const unsigned short* pName);
    ~CBaseObject();
private:
    CBaseObject(const CBaseObject&);
    void operator=(const CBaseObject&);
    static long m_cObjects;
};
class CUnknown : public INonDelegatingUnknown, public CBaseObject {
private:
    const LPUNKNOWN m_pUnk;
protected:
    long m_cRef;
public:
    CUnknown(const unsigned short* pName, LPUNKNOWN pUnk);
    virtual ~CUnknown() {}
    HRESULT __stdcall NonDelegatingQueryInterface(REFIID, void**);
    unsigned long __stdcall NonDelegatingAddRef();
    unsigned long __stdcall NonDelegatingRelease();
    LPUNKNOWN GetOwner() const { return m_pUnk; }
};
#define DECLARE_IUNKNOWN \
    HRESULT __stdcall QueryInterface(REFIID riid, void** ppv) { return GetOwner()->QueryInterface(riid, ppv); } \
    unsigned long __stdcall AddRef() { return GetOwner()->AddRef(); } \
    unsigned long __stdcall Release() { return GetOwner()->Release(); }
struct IReferenceClock;
struct IEnumPins; struct IPin; struct FILTER_INFO; struct IFilterGraph;
struct IPersist : IUnknown { virtual HRESULT __stdcall GetClassID(CLSID* pClassID) = 0; };
struct IMediaFilter : IPersist {
    virtual HRESULT __stdcall Stop() = 0;
    virtual HRESULT __stdcall Pause() = 0;
    virtual HRESULT __stdcall Run(REFERENCE_TIME tStart) = 0;
    virtual HRESULT __stdcall GetState(unsigned long dwMilliSecsTimeout, long* State) = 0;
    virtual HRESULT __stdcall SetSyncSource(IReferenceClock* pClock) = 0;
    virtual HRESULT __stdcall GetSyncSource(IReferenceClock** pClock) = 0;
};
struct IBaseFilter : IMediaFilter {
    virtual HRESULT __stdcall EnumPins(IEnumPins** ppEnum) = 0;
    virtual HRESULT __stdcall FindPin(const unsigned short* Id, IPin** ppPin) = 0;
    virtual HRESULT __stdcall QueryFilterInfo(FILTER_INFO* pInfo) = 0;
    virtual HRESULT __stdcall JoinFilterGraph(IFilterGraph* pGraph, const unsigned short* pName) = 0;
    virtual HRESULT __stdcall QueryVendorInfo(unsigned short** pVendorInfo) = 0;
};
struct IAMovieSetup : IUnknown {
    virtual HRESULT __stdcall Register() = 0;
    virtual HRESULT __stdcall Unregister() = 0;
};
class CBaseFilter : public CUnknown, public IBaseFilter, public IAMovieSetup {
public:
    DECLARE_IUNKNOWN
};
class CBaseRenderer : public CBaseFilter {
public:
    CBaseRenderer(REFCLSID RenderClass, const unsigned short* pName, LPUNKNOWN pUnk, HRESULT* phr);
    unsigned char _body[0xe0 - 0x14];
};
struct IQualProp : IUnknown {
    virtual HRESULT __stdcall get_FramesDroppedInRenderer(int* pcFrames) = 0;
};
struct IQualityControl : IUnknown {
    virtual HRESULT __stdcall Notify(IBaseFilter* pSelf, long q) = 0;
};
class CBaseVideoRenderer : public CBaseRenderer, public IQualProp, public IQualityControl {
public:
    CBaseVideoRenderer(REFCLSID RenderClass, const unsigned short* pName, LPUNKNOWN pUnk, HRESULT* phr);
    DECLARE_IUNKNOWN
};
class CTexture;
struct __declspec(uuid("71771540-2017-11cf-ae26-0020afd79767")) CLSID_TextureRenderer;   // retail GUID @ 0x0129D150 (DX SDK Texture3D9 sample)
class CTextureRenderer : public CBaseVideoRenderer {
public:
    CTextureRenderer(LPUNKNOWN pUnk, HRESULT* phr, CTexture* pTexture);
};
CTextureRenderer::CTextureRenderer(LPUNKNOWN pUnk, HRESULT* phr, CTexture* pTexture)
    : CBaseVideoRenderer(__uuidof(CLSID_TextureRenderer), L"Texture Renderer", pUnk, phr)   // name @ 0x0129D160
{
}