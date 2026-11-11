#ifndef FABLE_ENGINE_CRENDERTARGET_H
#define FABLE_ENGINE_CRENDERTARGET_H
// NEW (chunk 2008, row 0x00B665E0 CEngineSkyRenderer::GetEnvironmentMap). Ego_r.pdb CRenderTarget (class, 36 bytes): the CBaseClass vptr
// (RTTI .?AVCRenderTarget@@, vtable 0x0129C800), RenderToSurface +0x04 and ZBufferSurface +0x14 (both CSurface, 0x10 bytes).
// Members (Ego_r.pdb / FableWin.pdb spellings; retail addresses in the Ego_r donor order 0x00B7EE20..0x00B7EEA0 = retail
// 0x00A10E10..0x00A10E90): DetachTarget 0x00A10E10 (`if (RenderToSurface.PD3DSurface) RenderToSurface.Uninitialise()`),
// DetachZBuffer 0x00A10E20 (the same on ZBufferSurface), Uninitialise 0x00A10E30, AttachTarget 0x00A10E60, AttachZBuffer 0x00A10E90,
// the two-surface constructor ??0CRenderTarget@@QAE@AAVCSurface@@0@Z 0x00A10DC0. The default constructor (FableWin.pdb only) is
// inline in retail (0x00B66632: the out-of-line CBaseClass constructor, then the vtable and both CSurface constructors in place); the
// destructor is the implicit one (no own vtable store: 0x00B66A2B..0x00B66A56 destroys ZBufferSurface, RenderToSurface, then calls
// ~CBaseClass 0x0099A2E0). Needs FABLE_RETAIL_CSURFACE (+ FABLE_RETAIL_CSURFACE_COPYFROMMEMORY for the inline CSurface destructor).
#include "engine/CBaseClass.h"
#include "engine/CSurface.h"
class CRenderTarget : public CBaseClass {
public:
    CRenderTarget() { }
    CRenderTarget(CSurface& target, CSurface& zbuffer);                  // 0x00A10DC0
    void DetachTarget();                                                 // 0x00A10E10
    void DetachZBuffer();                                                // 0x00A10E20
    void Uninitialise();                                                 // 0x00A10E30
    void AttachTarget(const CSurface& target);                           // 0x00A10E60
    void AttachZBuffer(const CSurface& zbuffer);                         // 0x00A10E90
    CSurface* GetPRenderToSurface() { return &RenderToSurface; }
    CSurface* GetPZBuffer() { return &ZBufferSurface; }
private:
    CSurface RenderToSurface;                                            // +0x04
    CSurface ZBufferSurface;                                             // +0x14
};
FABLE_STATIC_ASSERT(sizeof(CRenderTarget) == 0x24);
#endif
