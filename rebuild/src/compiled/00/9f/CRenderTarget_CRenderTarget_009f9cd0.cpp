// CRenderTarget::CRenderTarget() -- default constructor.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F9CD0, length 43 bytes.
// Manifest label "CAIStateGroup_HoverHornetQueen" is propagated; the vtable stored at +0, 0x0129C800, is
// CRenderTarget and the one stored at +4 and +0x14, 0x0122F84C, is CSurface (rtti-vtables.tsv): the
// render target embeds two surfaces (colour and depth). Callee: CBase::CBase @ 0x0099A2D0.
//
// Shape: base ctor, own vtable, then the two embedded CSurface members at +4 and +0x14 constructed
// inline (vtable, two null pointers each; the fourth dword of each surface is not initialised). The
// scheduler shares one zero register and one vtable register across both. Member names are descriptive.
namespace N_0099a2d0 { struct CBase {
    virtual void v00();
    CBase();                                    // 0x0099A2D0
}; } using N_0099a2d0::CBase;

class CSurface { public:                               // vtable 0x0122F84C, 0x10 bytes
    virtual void v00();
    void* m_pSurface;                           // +0x04
    void* m_pTexture;                           // +0x08
    long m_format;                              // +0x0c (uninitialised here)
    CSurface() : m_pSurface(0), m_pTexture(0) { }
};

class CRenderTarget : public CBase { public:
    CSurface m_colour;                          // +0x04
    CSurface m_depth;                           // +0x14
    virtual void v00();
    CRenderTarget();
};

CRenderTarget::CRenderTarget()
    : CBase()
    , m_colour()
    , m_depth()
{
}