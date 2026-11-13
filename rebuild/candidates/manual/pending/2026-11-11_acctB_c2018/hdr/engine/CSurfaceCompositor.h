#ifndef FABLE_ENGINE_CSURFACECOMPOSITOR_H
#define FABLE_ENGINE_CSURFACECOMPOSITOR_H
// New header (chunk 2018, row 0x00B2B180 CEngineSurfaceCompositionManager::RenderToTexture). Ego_r.pdb CSurfaceCompositor (class, 0x30
// bytes; RTTI .?AVCSurfaceCompositor@@, vtable 0x01301CF0): the CBaseClass vptr, CurrentBlend +0x04, StateCaptureBlock +0x08,
// WasRendering +0x09, DestSurface +0x0C, RenderTarget +0x10 (CCountedPointer<CRenderTarget>), OldWindow +0x18 (C2DBoxF), LayerCount
// +0x28, AlphaTarget +0x2C -- the model the landed ctor 0x00F38990, ~CSurfaceCompositor 0x00B2E6C0 and SetBlendMode 0x00F38A10 restate
// locally. The nested CVertexType (Ego_r.pdb, 0x18 bytes: X, Y, Z, DiffuseColour, U1, V1) with the FableWin.pdb inline setters
// ?SetPosition@CVertexType@CSurfaceCompositor@@QAEXABVC3DVector@@@Z / ?SetU1V1@..@@QAEXMM@Z.
// Functions (Ego_r.pdb / FableWin.pdb spellings): ??0CSurfaceCompositor@@QAE@AAVCSurface@@@Z 0x00F38990,
// ?StartComposition@CSurfaceCompositor@@QAEX_N@Z 0x00F39730, ?EndComposition@CSurfaceCompositor@@QAEXXZ 0x00F38930,
// ?CompositeQuad@CSurfaceCompositor@@QAEXABVCTexture@@QBVCVertexType@1@W4ECompositeBlendType@@@Z 0x00F39A50 (all out of line). The
// destructor is the implicit one (no own vtable store; RenderToTexture 0x00B2B5F5..0x00B2B655 expands it: the counted render target,
// the capture block's inline destructor, then ~CBaseClass 0x0099A2E0). Needs FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE_DTOR for
// that expansion.
#include "rebuild_abi.h"
#include "engine/CBaseClass.h"
#include "engine/CCountedPointer.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CCompositeBlendDesc.h"
#include "engine/C2DBoxF.h"
#include "engine/C3DVector.h"
class CSurface;
class CTexture;
class CRenderTarget;
class CSurfaceCompositor : public CBaseClass {
public:
    class CVertexType {
    public:
        void SetPosition(const C3DVector& position) { X = position.X; Y = position.Y; Z = position.Z; }
        void SetU1V1(float u1, float v1) { U1 = u1; V1 = v1; }
        float         X;                                     // +0x00
        float         Y;                                     // +0x04
        float         Z;                                     // +0x08
        unsigned long DiffuseColour;                         // +0x0C
        float         U1;                                    // +0x10
        float         V1;                                    // +0x14
    };
    CSurfaceCompositor(CSurface& dest_surface);                          // 0x00F38990
    void StartComposition(bool alpha_target);                            // 0x00F39730
    void EndComposition();                                               // 0x00F38930
    void CompositeQuad(const CTexture& texture, const CVertexType* const vertices, ECompositeBlendType blend);   // 0x00F39A50
private:
    ECompositeBlendType            CurrentBlend;             // +0x04
    CRenderStateCaptureBlock       StateCaptureBlock;        // +0x08
    bool                           WasRendering;             // +0x09
    CSurface*                      DestSurface;              // +0x0C
    CCountedPointer<CRenderTarget> RenderTarget;             // +0x10
    C2DBoxF                        OldWindow;                // +0x18
    long                           LayerCount;               // +0x28
    bool                           AlphaTarget;              // +0x2C
};
FABLE_STATIC_ASSERT(sizeof(CSurfaceCompositor::CVertexType) == 0x18);
FABLE_STATIC_ASSERT(sizeof(CSurfaceCompositor) == 0x30);
#endif // FABLE_ENGINE_CSURFACECOMPOSITOR_H
