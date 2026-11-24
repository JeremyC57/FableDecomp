// CEnginePrimitiveRenderer::Render(const CEngineComponentRenderParams& params) -- render one layer when the camera allows it: inside a render-state capture block, prepare the layer, then per layer either render every scene, flush the matching sub-primitive lists (with the outline-glow / shadow components around them where enabled) or draw the occlusion pass; finally flush list 0 (and list 1 on request) and finish the layer
// Leaf: ?Render@CEnginePrimitiveRenderer@@UAEXABVCEngineComponentRenderParams@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Layout: local model -- the method overrides the CEngineComponent base at +0x268 (this arrives adjusted, the body works on
//   this-0x268); full-object fields: light-index mode +0x78, flags +0xbd/+0xbe, the scene vector at +0x338 (retail STLport 12 bytes).
#include "fable_compiler.h"
struct CTrackedRenderState;
struct SCapturedRenderState {
    CTrackedRenderState* m_pState;
    unsigned long        m_priorValue;
};
class CRenderStateManager
{
public:
    void RestoreCaptureBlock();   // 0x00A05840
};

struct CRenderManagerCoreView : public CRenderStateManager {
    unsigned char        _pad0000[0x8];
    SCapturedRenderState m_captured[0x400];        // +0x0008
    CTrackedRenderState* m_dirty[0x200];           // +0x2008
    long                 m_capturedCount;          // +0x2808
    long                 m_dirtyCount;             // +0x280C
    long                 m_unknown2810;            // +0x2810
    unsigned long        m_captureMask;            // +0x2814
    unsigned char        _pad2818[0x3a24 - 0x2818];
    unsigned char        m_captureBlockMarker[4];  // +0x3A24
    FABLE_FORCEINLINE void OpenCaptureBlock()
    {
        m_captured[m_capturedCount++].m_pState = (CTrackedRenderState*)m_captureBlockMarker;
        m_captureMask <<= 1;
    }
};
extern CRenderManagerCoreView* g_pRenderStateManager;  // 0x013CB508

struct SSceneRenderInfo {                          // returned by value (0x240 bytes)
    unsigned char _pad000[0x110];
    unsigned char Settings[0x130];                 // +0x110
};
struct CEngineSceneRenderSettings;
struct CEngineOcclusionRenderer {                  // 0x01436E60
    unsigned char _pad00[8];
    bool          Enabled;                         // +0x08
    unsigned char _pad09[0x334 - 9];
    void*         Target;                          // +0x334
    SSceneRenderInfo GetRenderInfo();              // 0x00B38890
};
extern CEngineOcclusionRenderer* g_pOcclusionRenderer;   // 0x01436E60
// FAM13: 0x01436E3C is the outline-glow component (CEngine::Initialise 0x00B26360 stores `new CEngineScreenEffectOutlineGlow` there;
// ctor 0x00B891B0, RTTI .?AVCEngineScreenEffectOutlineGlow@@), not a water renderer: shared header, PeekRenderEnabled (+0x08).
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_OUTLINERENDERING
#include "engine/CEngineScreenEffectOutlineGlow.h"
namespace NEngineInternal { extern CEngineScreenEffectOutlineGlow* GPScreenEffectOutlineGlow; }   // 0x01436E3C ?GPScreenEffectOutlineGlow@NEngineInternal@@3PAVCEngineScreenEffectOutlineGlow@@A
// NF45 (round 7): 0x01436E38 is NEngineInternal::GPScreenEffectDisplacementRenderer (Ego_r.pdb / FableWin.pdb
//   ?GPScreenEffectDisplacementRenderer@NEngineInternal@@3PAVCEngineScreenEffectDisplacementRenderer@@A, engine header) and the four
//   calls are its pass brackets Begin/FinishDisplacementRendering and Begin/FinishDisplacementStencilRendering (overlay opt-in
//   DISPLACEMENTPASSES); was a local CEngineShadowRenderer (BeginShadows / EndShadows / BeginShadowCasters / EndShadowCasters) on
//   g_pShadowRenderer. Not changed here: the 0x01436E60 (GPShadowRenderer) read, which still goes through the CEngineOcclusionRenderer
//   stand-in.
// NF95 (2026-11-19, q11254): 0x01436EA0 is NEngineInternal::GPCamera (Ego_d.pdb PUB32 ?GPCamera@NEngineInternal@@3PAVCEngineCamera@@A;
//   the landed 0x00BCA6B0 / 0x00BB6450 bind it so); it was g_pLightingManager_01436ea0. The two scene calls it feeds are the PDB
//   CEngineScene::RenderSceneAndCheckVisibility 0x00B942A0 and RenderScene 0x00B93F40 (Ego_r.pdb 0x00A47270 / 0x00A46EA0, FableWin.pdb
//   0x02E1E7E0 / 0x02E1DB90; their landed self rows carry those leaves): layer, the camera by reference, the occlusion layer, a bool.
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_DISPLACEMENTPASSES
#include "engine/CEngineScreenEffectDisplacementRenderer.h"
namespace NEngineInternal { extern CEngineScreenEffectDisplacementRenderer* GPScreenEffectDisplacementRenderer; }   // 0x01436E38
class CEngineSubPrimitiveRenderer { public:
    void RenderList(long list, bool sort);         // 0x00B849F0
    unsigned long GetListSize(long list);          // 0x00B842B0
    void ClearList(long list);                     // 0x00B847B0
};
extern CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer;   // 0x01436E74
class CEngineCamera;
namespace NEngineInternal { extern CEngineCamera* GPCamera; }   // 0x01436EA0
enum EEngineRenderLayer;
enum EEngineOcclusionLayer;

class CEngineScene { public:
    void RenderOcclusion(long layer, const unsigned char (&settings)[0x130]);   // 0x00B94060
    void RenderSceneAndCheckVisibility(EEngineRenderLayer layer, const CEngineCamera& camera, EEngineOcclusionLayer occlusion, bool flag);   // 0x00B942A0
    void RenderScene(EEngineRenderLayer layer, const CEngineCamera& camera);   // 0x00B93F40
};
#include "engine/CArray.h"

class CCamera { public:
    unsigned char _pad[0x78];
    unsigned char Flags;                           // +0x78
};
class CEngineComponentRenderParams { public:
    CCamera*      Camera;                          // +0x00
    long          Layer;                           // +0x04
    unsigned long Flags;                           // +0x08
};
struct CEngineRendererBase {                       // 0x268 bytes
    virtual void b00();
    unsigned char _pad004[0x78 - 4];
    long          LightIndexMode;                  // +0x78
    unsigned char _pad07c[0xbd - 0x7c];
    bool          ShadowsActive;                   // +0xbd
    bool          SelfShadowing;                   // +0xbe
    unsigned char _pad0bf[0x268 - 0xbf];
};
class CEnginePrimitiveRenderer : public CEngineRendererBase, public CEngineComponent { public:   // CEngineComponent: shared header (12 bytes at +0x268)
    unsigned char         _pad274[0x338 - 0x274];
    CArray<CEngineScene*> Scenes;                  // +0x338
    virtual void Render(const CEngineComponentRenderParams& params);
    void PreRenderLayer(long layer);               // 0x00B32AD0
    // NF386 (2026-11-24): ?RenderNonsortedPrimitives@CEnginePrimitiveRenderer@@QAEXXZ (Ego_r.pdb + FableWin.pdb; landed self 0x00B32610).
    void RenderNonsortedPrimitives();              // 0x00B32610
    void PostRenderLayer();                        // 0x00B32230
};

void CEnginePrimitiveRenderer::Render(const CEngineComponentRenderParams& params)
{
    if (!(params.Camera->Flags & 1))
        return;
    g_pRenderStateManager->OpenCaptureBlock();
    PreRenderLayer(params.Layer);
    switch (params.Layer) {
    case 0x20:
        RenderNonsortedPrimitives();
        break;
    case 2:
        if (g_pOcclusionRenderer->Enabled && g_pOcclusionRenderer->Target) {
            for (CEngineScene** it = Scenes.begin(); it != Scenes.end(); ++it) {
                CEngineScene* scene = *it;
                scene->RenderOcclusion(params.Layer, g_pOcclusionRenderer->GetRenderInfo().Settings);
            }
        }
        break;
    case 0x80:
        if (NEngineInternal::GPScreenEffectOutlineGlow->PeekRenderEnabled()) {
            g_pEngineSubPrimitiveRenderer->RenderList(0xe, true);
            if (g_pEngineSubPrimitiveRenderer->GetListSize(0xd) > 0) {
                NEngineInternal::GPScreenEffectOutlineGlow->BeginOutlineRendering();
                g_pEngineSubPrimitiveRenderer->RenderList(0xd, true);
                NEngineInternal::GPScreenEffectOutlineGlow->FinishOutlineRendering();
            }
        } else {
            g_pEngineSubPrimitiveRenderer->ClearList(0xe);
            g_pEngineSubPrimitiveRenderer->ClearList(0xd);
        }
        break;
    case 0x100:
        if (NEngineInternal::GPScreenEffectOutlineGlow->PeekRenderEnabled()) {
            g_pEngineSubPrimitiveRenderer->RenderList(8, true);
            g_pEngineSubPrimitiveRenderer->RenderList(0xa, true);
        } else {
            g_pEngineSubPrimitiveRenderer->ClearList(8);
            g_pEngineSubPrimitiveRenderer->ClearList(0xa);
        }
        break;
    case 0x200:
        if (NEngineInternal::GPScreenEffectOutlineGlow->PeekRenderEnabled())
            g_pEngineSubPrimitiveRenderer->RenderList(9, true);
        else
            g_pEngineSubPrimitiveRenderer->ClearList(9);
        break;
    case 0x400:
        g_pEngineSubPrimitiveRenderer->RenderList(0xb, true);
        break;
    case 0x1000:
        g_pEngineSubPrimitiveRenderer->RenderList(7, true);
        break;
    case 0x40000:
        g_pEngineSubPrimitiveRenderer->RenderList(0xc, true);
        break;
    case 0x8000000:
        if (g_pEngineSubPrimitiveRenderer->GetListSize(5) > 0) {
            if (NEngineInternal::GPScreenEffectDisplacementRenderer->BeginDisplacementRendering()) {
                g_pEngineSubPrimitiveRenderer->RenderList(5, true);
                NEngineInternal::GPScreenEffectDisplacementRenderer->FinishDisplacementRendering();
            }
            g_pEngineSubPrimitiveRenderer->ClearList(5);
        }
        break;
    case 0x10000000:
        if (g_pEngineSubPrimitiveRenderer->GetListSize(6) > 0) {
            NEngineInternal::GPScreenEffectDisplacementRenderer->BeginDisplacementStencilRendering();
            g_pEngineSubPrimitiveRenderer->RenderList(6, true);
            NEngineInternal::GPScreenEffectDisplacementRenderer->FinishDisplacementStencilRendering();
            g_pEngineSubPrimitiveRenderer->ClearList(6);
        }
        break;
    default:
        for (CEngineScene** it = Scenes.begin(); it != Scenes.end(); ++it) {
            CEngineScene* scene = *it;
            if ((params.Flags & 1) && ShadowsActive)
                scene->RenderSceneAndCheckVisibility((EEngineRenderLayer)params.Layer, *NEngineInternal::GPCamera, (EEngineOcclusionLayer)LightIndexMode, !SelfShadowing);
            else
                scene->RenderScene((EEngineRenderLayer)params.Layer, *NEngineInternal::GPCamera);
        }
        break;
    }
    g_pEngineSubPrimitiveRenderer->RenderList(0, true);
    if (params.Flags & 2)
        g_pEngineSubPrimitiveRenderer->RenderList(1, true);
    PostRenderLayer();
    g_pRenderStateManager->RestoreCaptureBlock();
}