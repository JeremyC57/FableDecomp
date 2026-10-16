// CTextureRenderer::CTextureRenderer(LPUNKNOWN, HRESULT*, CTexture*) -- DirectShow video renderer that draws into a texture (own texture when none is given)
// Leaf: ??0CTextureRenderer@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3B510, 113 bytes (+15 int3). __thiscall, `ret 0Ch`. VIDEO/RENDER LAYER (DirectShow
//         strmbase + D3D9 texture; replacement candidate).
// Callees: CBaseVideoRenderer::CBaseVideoRenderer(REFCLSID, TCHAR*, LPUNKNOWN, HRESULT*) 0x00CA7360 with CLSID 0x0129D150
//          and name L"Texture Renderer" 0x0129D160 (UNICODE strmbase). Vtables (RTTI .?AVCTextureRenderer@@): 0x0129D08C +0x00 (INonDelegatingUnknown/CUnknown),
//          0x0129D04C +0x0C (IBaseFilter), 0x0129D030 +0x10 (IAMovieSetup), 0x0129D008 +0xE0 (IQualProp),
//          0x0129CFF0 +0xE4 (IQualityControl).
// Shape: the five vtable stores, init list PVideoSys(0), CreateOwnTexture(false) through one `xor ecx,ecx`; the body stores
//        *phr = S_OK, Texture = texture and sets CreateOwnTexture when no texture was passed.
// Layout: strmbase bases to +0x160; Ego_r.pdb CTextureRenderer members from +0x160: PVideoSys, Texture +0x164,
//         TextureFormat +0x168, Width +0x16C, Height +0x170, Pitch +0x174, CreateOwnTexture +0x178.
struct GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int n);
#pragma intrinsic(memcmp)
inline int IsEqualGUID(const GUID& a, const GUID& b) { return !memcmp(&a, &b, sizeof(GUID)); }
inline int operator==(const GUID& a, const GUID& b) { return IsEqualGUID(a, b); }
extern "C" const GUID IID_IQualProp;             // 0x012AAB04 {1BD0ECB0-F8E2-11CE-AAC6-0020AF0B99A3}
extern "C" const GUID IID_IQualityControl;       // 0x012A99B4 {56A868A5-0AD4-11CE-B03A-0020AF0BA770}
struct IUnknown {
    virtual long __stdcall QueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};
long __stdcall GetInterface(IUnknown* unknown, void** ppv);   // 0x00CACBE0 (strmbase combase.cpp)
struct IQualProp : IUnknown {
    virtual long __stdcall get_FramesDroppedInRenderer(int* frames) = 0;
};
struct IQualityControl : IUnknown {
    virtual long __stdcall Notify(void* sender, long q) = 0;
};
struct INonDelegatingUnknown {
    virtual long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};
struct IBaseFilter : IUnknown { virtual long __stdcall GetClassID(GUID* clsid) = 0; };
struct IAMovieSetup : IUnknown { virtual long __stdcall Register() = 0; };
struct CUnknownPart : INonDelegatingUnknown {   // strmbase CUnknown (+CBaseObject): m_pUnknown, m_cRef -> 0x0C
    void* m_pUnknown;                            // +0x04
    long m_cRef;                                 // +0x08
};
struct CBaseFilterPart : CUnknownPart, IBaseFilter, IAMovieSetup {   // IBaseFilter vptr +0x0C, IAMovieSetup +0x10
    unsigned char _pad14[0xe0 - 0x14];           // rest of CBaseFilter / CBaseRenderer
};
struct CBaseRenderer : CBaseFilterPart {
    long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv);   // 0x00CA4780
};
struct CBaseVideoRenderer : CBaseRenderer, IQualProp, IQualityControl {   // IQualProp +0xE0, IQualityControl +0xE4
    unsigned char _pad_e8[0x160 - 0xe8];
    CBaseVideoRenderer(const GUID& clsid, const unsigned short* name, IUnknown* unknown, long* phr);   // 0x00CA7360
    long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv);   // 0x00CA6080
};
struct CVideoSys;
class CTexture;
struct CTextureRenderer : CBaseVideoRenderer {   // vtables 0x0129D08C / 0x0129D04C / 0x0129D030 / 0x0129D008 / 0x0129CFF0
    CVideoSys* PVideoSys;                        // +0x160
    CTexture* Texture;                           // +0x164
    long TextureFormat;                          // +0x168 _D3DFORMAT
    int Width;                                   // +0x16c
    int Height;                                  // +0x170
    int Pitch;                                   // +0x174
    bool CreateOwnTexture;                       // +0x178
    CTextureRenderer(IUnknown* unknown, long* phr, CTexture* texture);
    long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv);
};
extern "C" const GUID CLSID_TextureRenderer;     // 0x0129D150
CTextureRenderer::CTextureRenderer(IUnknown* unknown, long* phr, CTexture* texture)
    : CBaseVideoRenderer(CLSID_TextureRenderer, L"Texture Renderer", unknown, phr), PVideoSys(0), CreateOwnTexture(false)
{
    *phr = 0;
    Texture = texture;
    if (!texture)
        CreateOwnTexture = true;
}
