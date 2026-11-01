// CBaseVideoRenderer::QueryInterface(REFIID riid, void** ppv) -- DirectShow BaseClasses DECLARE_IUNKNOWN: hand the request to the owning (outer) IUnknown
// Leaf: ?QueryInterface@CBaseVideoRenderer@@UAGJABU_GUID@@PAPAX@Z
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
extern const CLSID CLSID_TextureRenderer;
class CTextureRenderer : public CBaseVideoRenderer {
public:
    CTextureRenderer(LPUNKNOWN pUnk, HRESULT* phr, CTexture* pTexture);
};
CTextureRenderer::CTextureRenderer(LPUNKNOWN pUnk, HRESULT* phr, CTexture* pTexture)
    : CBaseVideoRenderer(CLSID_TextureRenderer, 0, pUnk, phr)
{
}
