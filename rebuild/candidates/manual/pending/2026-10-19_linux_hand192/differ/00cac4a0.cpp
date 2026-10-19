// Grade: DIFFER(194v194) -- register choice only: retail forms the CMediaType address for the setter with `lea ecx,[esp+8] ; push ecx`, ours with eax (tried CMediaType*, const CMediaType*, const AM_MEDIA_TYPE* parameters); flags /EHsc /GS
// CEngineScreenEffectRenderer::OnRenderEffect(const AM_MEDIA_TYPE*, long, long, long, long, double) -- copy the media type into a CMediaType local, apply it through the object's type setter and, when that succeeds, hand the remaining arguments to virtual slot 22; returns the first failing HRESULT, else S_OK
// Leaf: OnRenderEffect
// Flags: /EHsc /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAC4A0, 194 bytes (+ int3 padding), `ret 1Ch`; EH frame (handler 0x012170E8) and a /GS
//   cookie (0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Identity: the catalog label is propagated and unproven (no direct call or vtable reference reaches 0x00CAC4A0); the body is
//   DirectShow base-class code linked into Fable.exe (the 0x00CAxxxx strmbase region): CMediaType::CMediaType(const
//   AM_MEDIA_TYPE&, HRESULT*) 0x00CB1D60 (CopyMediaType 0x00CB1BD0, the HRESULT stored through phr on failure) and
//   CMediaType::~CMediaType 0x00CB1CC0 (FreeMediaType). The type setter 0x00CAB990 (on `this`) takes the CMediaType*.
// Shape: the 0x48-byte CMediaType local (EH state 0) is destroyed on each return path; `if (FAILED(hr)) return hr;` twice,
//   then `return S_OK` (`xor eax,eax`).
struct AM_MEDIA_TYPE {                                 // 0x48 bytes
    unsigned char _body[0x48];
};
struct CMediaType : AM_MEDIA_TYPE {
    CMediaType(const AM_MEDIA_TYPE& rt, long* phr = 0);    // 0x00CB1D60
    ~CMediaType();                                     // 0x00CB1CC0
};

struct CMediaEffectSink {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual long RenderEffect(long a, long b, long c, long d, double time);   // slot 22 (+0x58)
};

struct CEngineScreenEffectRenderer : CMediaEffectSink {
    long SetMediaType(CMediaType* type);               // 0x00CAB990
    long OnRenderEffect(const AM_MEDIA_TYPE* type, long a, long b, long c, long d, double time);
};

long CEngineScreenEffectRenderer::OnRenderEffect(const AM_MEDIA_TYPE* type, long a, long b, long c, long d, double time)
{
    CMediaType mediaType(*type, 0);
    long hr = SetMediaType(&mediaType);
    if (hr < 0)
        return hr;
    hr = RenderEffect(a, b, c, d, time);
    if (hr < 0)
        return hr;
    return 0;
}
