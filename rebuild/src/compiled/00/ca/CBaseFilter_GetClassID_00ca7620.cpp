// CBaseFilter::GetClassID(CLSID* pClsID) -- DirectShow BaseClasses IPersist::GetClassID: E_POINTER for a null out-pointer, else copies the filter's m_clsid and returns NOERROR
// Leaf: ?GetClassID@CBaseFilter
// Retail: Fable.exe @ 0x00CA7620, 50 bytes. IBaseFilter vtable 0x0129D04C slot 3 (and two more filter vtables).
// Label: the manifest's "CTextureRenderer::vfunc_3". No callees.
// Identification: Fable links Microsoft's DirectShow BaseClasses (amfilter / renbase / combase) for its movie renderer
// CTextureRenderer (the DX9 SDK Texture3D9 sample class, primary vtable 0x0129D08C); the primary-vtable slot of this body
// and its callers' slots line up with the BaseClasses declaration order (CUnknown 0-3, CBaseFilter StreamTime 4,
// CBaseRenderer SourceThreadCanWait 10, OnRenderStart/OnRenderEnd 14/15, ScheduleSample 21, GetSampleTimes 22,
// ShouldDrawSampleNow 23, Render 42, DoRenderSample 43, CheckMediaType 44) and with the member offsets (CBaseFilter:
// m_pClock +0x18, m_tStart +0x20, m_clsid +0x28; CBaseRenderer: m_ThreadSignal +0x58, m_bStreaming +0x64).
// The method exists only in the IBaseFilter base (at +0xc of CBaseFilter), so the STDMETHODIMP override is `__stdcall` with
// `this` = that sub-object: m_clsid (CBaseFilter +0x28) is read at +0x1c from it (`add ecx,1Ch`).
// Shape: combase CheckPointer (`if (p == NULL) return E_POINTER;`, early `ret 8`), the 16-byte GUID assigned as four dwords.
struct GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};
typedef GUID CLSID;
typedef __int64 REFERENCE_TIME;

struct INonDelegatingUnknown {
    virtual long __stdcall NonDelegatingQueryInterface(const GUID& iid, void** object);
    virtual unsigned long __stdcall NonDelegatingAddRef();
    virtual unsigned long __stdcall NonDelegatingRelease();
};
class CUnknown : public INonDelegatingUnknown {
public:
    void*         m_pUnknown;                      // +0x04
    volatile long m_cRef;                          // +0x08
};
struct IBaseFilter {                              // IUnknown + IPersist + IMediaFilter + IBaseFilter (only IPersist used)
    virtual long __stdcall QueryInterface(const GUID& iid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall GetClassID(CLSID* pClassID);   // slot 3
};
struct IAMovieSetup {
    virtual long __stdcall QueryInterface(const GUID& iid, void** object);
};

class CBaseFilter : public CUnknown, public IBaseFilter, public IAMovieSetup {
public:
    long             m_State;                      // +0x14 FILTER_STATE
    void*            m_pClock;                     // +0x18 IReferenceClock*
    REFERENCE_TIME   m_tStart;                     // +0x20 CRefTime
    CLSID            m_clsid;                      // +0x28
    long __stdcall GetClassID(CLSID* pClsID);
};

#define NOERROR   0L
#define E_POINTER ((long)0x80004003L)

long __stdcall CBaseFilter::GetClassID(CLSID* pClsID)
{
    if (pClsID == 0)
        return E_POINTER;
    *pClsID = m_clsid;
    return NOERROR;
}