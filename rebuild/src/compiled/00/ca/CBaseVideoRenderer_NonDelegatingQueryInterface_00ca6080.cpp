// CBaseVideoRenderer::NonDelegatingQueryInterface(REFIID, void**) -- DirectShow base class: hand out IQualProp / IQualityControl, else defer to CBaseRenderer
// Leaf: NonDelegatingQueryInterface
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA6080, 129 bytes. __stdcall (STDMETHODIMP, `ret 0Ch`).
// Class: catalog "CTextureRenderer::vfunc_0" -- the body is the DirectShow strmbase (renbase.cpp)
//        CBaseVideoRenderer::NonDelegatingQueryInterface, inherited into slot 0 of CTextureRenderer's primary vtable
//        0x0129D08C (RTTI .?AVCTextureRenderer@@; Fable's movie-to-texture renderer, the DX SDK Texture3D9 pattern).
//        VIDEO/RENDER LAYER (DirectShow strmbase, a replacement candidate).
// Callees: GetInterface(LPUNKNOWN, void**) 0x00CACBE0 (strmbase); CBaseRenderer::NonDelegatingQueryInterface 0x00CA4780 (tail
//          jump). GUIDs: IID_IQualProp 0x012AAB04, IID_IQualityControl 0x012A99B4 (`riid == IID_x` is IsEqualGUID = memcmp
//          intrinsic, `mov ecx,4 ; repe cmpsd`).
// Shape: `(IQualProp*)this` / `(IQualityControl*)this` are null-tested casts to the subobjects at +0xE0 / +0xE4 (the null
//        arm of both shares one GetInterface(0, ppv) block).
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
struct CBaseRenderer : INonDelegatingUnknown {   // only the parts this body touches; size to +0xE0
    unsigned char _pad04[0xe0 - 0x04];
    long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv);   // 0x00CA4780
};
struct CBaseVideoRenderer : CBaseRenderer, IQualProp, IQualityControl {   // IQualProp +0xE0, IQualityControl +0xE4
    long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv);
};
long __stdcall CBaseVideoRenderer::NonDelegatingQueryInterface(const GUID& riid, void** ppv)
{
    if (riid == IID_IQualProp) {
        return GetInterface((IQualProp*)this, ppv);
    } else if (riid == IID_IQualityControl) {
        return GetInterface((IQualityControl*)this, ppv);
    }
    return CBaseRenderer::NonDelegatingQueryInterface(riid, ppv);
}