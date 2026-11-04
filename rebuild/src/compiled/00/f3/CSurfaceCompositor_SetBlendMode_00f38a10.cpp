// CSurfaceCompositor::SetBlendMode(ECompositeBlendType blend) -- when the composite blend changes, switch the render state manager's alpha-blend enable / source / dest states (2 alpha, 1 additive, 3 opaque, 4 multiplicative) and remember it; then, for an alpha target, set stage 0's colour / alpha ops for the base layer (layer 0) or the alpha op and an RGB-only colour write for the second layer
// Leaf: ?SetBlendMode@CSurfaceCompositor@@IAEXW4ECompositeBlendType@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F38A10, 1504 bytes incl. the 4-entry case jump table at 0x00F38FE0, thiscall `ret 4`.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?SetBlendMode@CSurfaceCompositor@@IAEXW4ECompositeBlendType@@@Z (protected), and
//   the landed CSurfaceCompositor::CompositeQuad 0x00F39A50 calls 0x00F38A10 with its ECompositeBlendType argument.
// Callees: GFGetSystemManager 0x009A4EC0 (?GFGetSystemManager@@YIPAVCSystemManager@@XZ); everything else is the inline
//   CRenderStateManager setters (engine/CRenderStateManager.h retail branch) on the render manager core
//   (system +0x60 PDisplayManager -> +0x08 PRenderManager): SetAlphaBlendEnable (+0x28B8), SetSourceAlphaBlendMode (+0x291C),
//   SetDestAlphaBlendMode (+0x2930) through D3DAlphaBlendModeTable 0x01396F68, SetTextureAlphaOperation(0) (+0x32A4) /
//   SetTextureOperation(0) (+0x3204) through D3DTextureOperationModeTable 0x01396E70, and the 4-bool SetColourWriteEnable
//   (+0x2840, folded mask 7) -- new inline under FABLE_RETAIL_CRENDERSTATEMANAGER_COLOURWRITE4 (no retail / Ego_r copy).
// Layout: PDB CSurfaceCompositor (0x30, the landed ctor 0x00F38990): CurrentBlend +0x04, LayerCount +0x28, AlphaTarget +0x2C.
//   ECompositeBlendType has no recoverable enumerator names (cases written as numbers).
// Shape: the cases in retail's block order (2, 1, 3, 4 -- VC7.1 lays case blocks out in source order); with case 1 first the
//   additive arm reloads its dest value instead of threading the unchanged-source path straight into the shared dest block.
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRENDERSTATEMANAGER_COLOURWRITE4
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXF
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CBaseClass.h"
#include "engine/CRenderStateCaptureBlock.h"

enum ECompositeBlendType { };
template <class T> class CCountedPointer;
class CRenderTarget;
class CSurfaceCompositor : public CBaseClass {             // vtable 0x01301CF0, 0x30 bytes
public:
    virtual ~CSurfaceCompositor();
protected:
    void SetBlendMode(ECompositeBlendType blend);
    ECompositeBlendType      CurrentBlend;                 // +0x04
    CRenderStateCaptureBlock StateCaptureBlock;            // +0x08
    bool                     WasRendering;                 // +0x09
    CSurface*                DestSurface;                  // +0x0c
    unsigned char            RenderTarget[8];              // +0x10 CCountedPointer<CRenderTarget>
    C2DBoxF                  OldWindow;                    // +0x18
    long                     LayerCount;                   // +0x28
    bool                     AlphaTarget;                  // +0x2c
};

void CSurfaceCompositor::SetBlendMode(ECompositeBlendType blend)
{
    CRenderManager& rm = GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
    if (blend != CurrentBlend) {
        switch (blend) {
        case 2:
            rm.SetAlphaBlendEnable(true);
            rm.SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
            rm.SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
            break;
        case 1:
            rm.SetAlphaBlendEnable(true);
            rm.SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
            rm.SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
            break;
        case 3:
            rm.SetAlphaBlendEnable(false);
            rm.SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
            rm.SetDestAlphaBlendMode(ALPHA_BLEND_ZERO);
            break;
        case 4:
            rm.SetAlphaBlendEnable(true);
            rm.SetSourceAlphaBlendMode(ALPHA_BLEND_ZERO);
            rm.SetDestAlphaBlendMode(ALPHA_BLEND_SRC_COLOR);
            break;
        }
        CurrentBlend = blend;
    }
    if (AlphaTarget) {
        if (LayerCount == 0) {
            rm.SetTextureAlphaOperation(0, TEXTURE_OP_SELECT_ARG1);
            rm.SetTextureOperation(0, TEXTURE_OP_SELECT_ARG1);
        }
        if (LayerCount == 1) {
            rm.SetTextureAlphaOperation(0, TEXTURE_OP_SELECT_ARG2);
            rm.SetColourWriteEnable(true, true, true, false);
        }
    }
}