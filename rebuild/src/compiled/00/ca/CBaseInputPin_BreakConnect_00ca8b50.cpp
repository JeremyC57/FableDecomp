// CBaseInputPin::BreakConnect() -- the DirectShow base-class pin method linked into Fable for CTextureRenderer's input pin: when an allocator is held (+0x9c), Decommit it (returning a failure code unchanged), then Release and clear it; S_OK otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA8B50, length 49 bytes.
// Leaf: BreakConnect
// Callees: IMemAllocator::Decommit (COM slot 6, `call [ecx+18h]`) and Release (slot 2), `this` pushed.
// Shape: `__fastcall`, `ret`; `this` in esi, the Decommit result tested with `jl` (FAILED) straight to the
// shared `pop esi ; ret` with hr still in eax, the member re-read for the Release, the immediate zero store,
// `xor eax,eax`. The manifest label "CSurface::GetRefCount" is propagated; the strmbase classes are not in the
// game PDB, so the pin is modelled by the one member the body touches.
typedef long HRESULT_T;
struct IMemAllocatorLike {                         // IUnknown + IMemAllocator slots
    virtual HRESULT_T __stdcall QueryInterface(const void* iid, void** object) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;                 // slot 2 (+0x08)
    virtual HRESULT_T __stdcall SetProperties(void* request, void* actual) = 0;
    virtual HRESULT_T __stdcall GetProperties(void* props) = 0;
    virtual HRESULT_T __stdcall Commit() = 0;
    virtual HRESULT_T __stdcall Decommit() = 0;                    // slot 6 (+0x18)
};
// DirectShow CBaseInputPin: the CBasePin part (CUnknown, IPin, IQualityControl, name, connection, media type,
// times, ...) and the IMemInputPin vptr fill the first 0x9c bytes; only the allocator pointer is touched here.
class CBaseInputPin {
    unsigned char      _base[0x9c];                // +0x00 (CBasePin + IMemInputPin)
protected:
    IMemAllocatorLike* m_pAllocator;               // +0x9c
public:
    HRESULT_T BreakConnect();
};
HRESULT_T CBaseInputPin::BreakConnect()
{
    if (m_pAllocator) {
        HRESULT_T hr = m_pAllocator->Decommit();
        if (hr < 0)
            return hr;
        m_pAllocator->Release();
        m_pAllocator = 0;
    }
    return 0;
}