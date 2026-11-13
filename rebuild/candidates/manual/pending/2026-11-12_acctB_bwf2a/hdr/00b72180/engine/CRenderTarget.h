#ifndef FABLE_ENGINE_CRENDERTARGET_H
#define FABLE_ENGINE_CRENDERTARGET_H
// New header (bwf2, row 0x00B72180). CRenderTarget (Ego_r.pdb layout, 0x24 bytes): CBaseClass, RenderToSurface +0x04,
// ZBufferSurface +0x14; vtable 0x0129C800 (??_ECRenderTarget = virtual dtor). Default ctor / dtor inline in retail
// (0x00B72D0F: ??0CBaseClass called, vptr and both CSurface members built in place). Needs FABLE_RETAIL_CSURFACE.
#include "rebuild_abi.h"
#include "engine/CBaseClass.h"
#include "engine/CSurface.h"
class CRenderTarget : public CBaseClass {
public:
    CRenderTarget() {}
    CRenderTarget(CSurface& target, CSurface& zbuffer);                  // 0x00A10DC0
    void DetachTarget();                                                 // 0x00A10E10
    void DetachZBuffer();                                                // 0x00A10E20
    void Uninitialise();                                                 // 0x00A10E30
    void AttachTarget(const CSurface& surface);                          // 0x00A10E60
    void AttachZBuffer(const CSurface& surface);                         // 0x00A10E90
private:
    CSurface RenderToSurface;                                            // +0x04
    CSurface ZBufferSurface;                                             // +0x14
};
FABLE_STATIC_ASSERT(sizeof(CRenderTarget) == 0x24);
#endif
