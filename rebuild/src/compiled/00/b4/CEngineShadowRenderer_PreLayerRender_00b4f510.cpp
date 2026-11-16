// CEngineShadowRenderer::PreLayerRender(const CEngineComponentRenderParams& params) -- before the shadow-buffer layer (2), when a shadow technique is active: open the shadow capture block, set the depth-bias states from the shadow renderer's z offsets, bind the shadow render target and clear it to white (colour only for technique 1, which also clears +0x2840; colour+depth otherwise), set the window to the shadow texture's size, set states +0x28cc=1 / +0x28e0=0x80, an identity view and the shadow projection
// Leaf: PreLayerRender
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4F510, length 976 bytes, `ret 4` (virtual).
// Callees: CDisplayManager::SetRenderTarget 0x009BF160 / ClearRenderTarget(const CRGBColour*, ulong) 0x009BE420 on 0x01436E1C,
//   CTexture::GetDimensions(long) 0x009F9DB0 (C2DExtentsI by value), CRenderManagerCore::SetAWindow(const C2DBoxF*) 0x00A0AA80,
//   CShaderRenderManager::SetViewTransform 0x00988350 / SetProjectionTransform 0x00988540 on 0x01436E14.
// The z offsets are read through the shadow renderer global 0x01436E60, not `this`.
// Layout: engine/CEngineShadowRenderer.h (ShadowBufferRenderStateCaptureBlock +0x24c, RenderTarget +0x2c4,
//   ShadowBufferTexture +0x2e8, RenderingTechnique +0x334); the projection matrix sits in SceneData at +0x4c.
#define FABLE_RETAIL_CENGINESHADOWRENDERER_SHADOWBUFFERRENDERSTATECAPTUREBLOCK
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_CAPTURE_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETDEPTHOFFSET_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETSLOPESCALEDDEPTHOFFSET_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CTEXTURE_EXTENTS_INLINE
#include "fable_compiler.h"
#include "engine/CEngineShadowRenderer.h"

// NF45 (round 7): the render core is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h): the local CRenderManagerCoreView / SetState /
//   ChangeState model is the shared inline CRenderStateManager setters: +0x2B60 DepthOffset / +0x2B74 DepthOffsetSlope
//   (SetDepthOffset / SetSlopeScaledDepthOffset, overlay inline opt-ins, the float's bits), +0x2840 ColourWriteEnable(false) (repo
//   OUTLINE_INLINE), +0x28CC AlphaTestEnable, +0x28E0 AlphaRef. The window is CRenderManagerCore::SetAWindow(const C2DBoxF&)
//   0x00A0AA80 (was a pointer overload on the stand-in). The capture push on 0x013CB508 is
//   ShadowBufferRenderStateCaptureBlock.Capture() (CRenderStateCaptureBlock::StateManager, repo CAPTURE_INLINE; overlay opt-in types
//   the +0x24C member; was g_pRenderStateManager). The shadow-buffer extents are the shared CTexture::GetWidth / GetHeight (repo
//   EXTENTS_INLINE; GetDimensions 0x009F9DB0) and C2DExtentsI (was a local CTextureView / C2DExtentsI).
#include "engine/CRenderManager.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18
class C2DBoxF { public: float X0, Y0, X1, Y1; };
class CMatrix3x4 { public: float M[12]; };
class CMatrix4x4 { public: float M[16]; };
class CRGBColour { public:
    unsigned char B, G, R, A;
};
class CRenderTarget;
class CDisplayManager { public:
    void SetRenderTarget(CRenderTarget* target);        // 0x009BF160
    void ClearRenderTarget(const CRGBColour* colour, unsigned long flags);   // 0x009BE420
};
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }   // 0x01436E1C ?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A
class CShaderRenderManager {
public:
    void SetViewTransform(const CMatrix3x4& view);      // 0x00988350
    void SetProjectionTransform(const CMatrix4x4& projection);   // 0x00988540
};
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }    // 0x01436E14
namespace NEngineInternal { extern CEngineShadowRenderer* GPShadowRenderer; }        // 0x01436E60
class CEngineComponentRenderParams { public:
    void*         Params;                               // +0x00
    long          RenderLayer;                          // +0x04
    unsigned long RenderLayerFlags;                     // +0x08
};

static FABLE_FORCEINLINE void SetWindowSize(C2DBoxF& window, unsigned long width, unsigned long height)
{
    window.X0 = 0.0f;
    window.Y0 = 0.0f;
    window.X1 = (float)width;
    window.Y1 = (float)height;
}
struct CEngineShadowRenderer_Methods : CEngineShadowRenderer {
    void PreLayerRender(const CEngineComponentRenderParams& params);
};

void CEngineShadowRenderer_Methods::PreLayerRender(const CEngineComponentRenderParams& params)
{
    if (*(long*)RenderingTechnique != 0 && params.RenderLayer == 2) {
        ShadowBufferRenderStateCaptureBlock.Capture();
        NEngineInternal::GPRenderManager->SetDepthOffset(NEngineInternal::GPShadowRenderer->ShadowProjectionZOffset);
        NEngineInternal::GPRenderManager->SetSlopeScaledDepthOffset(NEngineInternal::GPShadowRenderer->ShadowProjectionZSlopeOffset);
        NEngineInternal::GPDisplayManager->SetRenderTarget((CRenderTarget*)RenderTarget);
        if (*(long*)RenderingTechnique == 1) {
            CRGBColour white;
            white.R = 0xff;
            white.G = 0xff;
            white.B = 0xff;
            white.A = 0xff;
            NEngineInternal::GPDisplayManager->ClearRenderTarget(&white, 1);
            NEngineInternal::GPRenderManager->SetColourWriteEnable(false);
        } else {
            CRGBColour white;
            white.R = 0xff;
            white.G = 0xff;
            white.B = 0xff;
            white.A = 0xff;
            NEngineInternal::GPDisplayManager->ClearRenderTarget(&white, 5);
        }
        C2DBoxF window;
        SetWindowSize(window, reinterpret_cast<const CTexture*>(ShadowBufferTexture)->GetWidth(0),
                      reinterpret_cast<const CTexture*>(ShadowBufferTexture)->GetHeight(0));
        NEngineInternal::GPRenderManager->SetAWindow(window);
        NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
        NEngineInternal::GPRenderManager->SetAlphaRef(0x80);
        CMatrix3x4 identity;
        identity.M[11] = 0.0f;
        identity.M[10] = 0.0f;
        identity.M[9] = 0.0f;
        identity.M[7] = 0.0f;
        identity.M[6] = 0.0f;
        identity.M[5] = 0.0f;
        identity.M[3] = 0.0f;
        identity.M[2] = 0.0f;
        identity.M[1] = 0.0f;
        identity.M[8] = 1.0f;
        identity.M[4] = 1.0f;
        identity.M[0] = 1.0f;
        NEngineInternal::GPShaderRenderManager->SetViewTransform(identity);
        NEngineInternal::GPShaderRenderManager->SetProjectionTransform(*(const CMatrix4x4*)(SceneData + 0x40));
    }
}