// CEnginePrimitiveRenderer::Render(const CEngineComponentRenderParams& params) -- render one layer when the camera allows it: inside a render-state capture block, prepare the layer, then per layer either render every scene, flush the matching sub-primitive lists (with the water/shadow renderers around them where enabled) or draw the occlusion pass; finally flush list 0 (and list 1 on request) and finish the layer
// Leaf: ?Render@CEnginePrimitiveRenderer@@
// Layout: local model -- the method overrides the CEngineComponent base at +0x268 (this arrives adjusted, the body works on
//   this-0x268); full-object fields: light-index mode +0x78, flags +0xbd/+0xbe, the scene vector at +0x338 (retail STLport 12 bytes).
struct CTrackedRenderState;
struct SCapturedRenderState {
    CTrackedRenderState* m_pState;
    unsigned long        m_priorValue;
};
struct CRenderManagerCoreView {
    unsigned char        _pad0000[0x8];
    SCapturedRenderState m_captured[0x400];        // +0x0008
    CTrackedRenderState* m_dirty[0x200];           // +0x2008
    long                 m_capturedCount;          // +0x2808
    long                 m_dirtyCount;             // +0x280C
    long                 m_unknown2810;            // +0x2810
    unsigned long        m_captureMask;            // +0x2814
    unsigned char        _pad2818[0x3a24 - 0x2818];
    unsigned char        m_captureBlockMarker[4];  // +0x3A24
    __forceinline void OpenCaptureBlock()
    {
        m_captured[m_capturedCount++].m_pState = (CTrackedRenderState*)m_captureBlockMarker;
        m_captureMask <<= 1;
    }
    void RestoreCaptureBlock();                    // 0x00A05840
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
struct CEngineWaterRenderer {                      // 0x01436E3C
    unsigned char _pad00[8];
    bool          Enabled;                         // +0x08
    void BeginReflection();                        // 0x00B89A20
    void EndReflection();                          // 0x00B89140
};
extern CEngineWaterRenderer* g_pWaterRenderer;     // 0x01436E3C
struct CEngineShadowRenderer {                     // 0x01436E38
    bool BeginShadows();                           // 0x00B87680
    void EndShadows();                             // 0x00B869A0
    void BeginShadowCasters();                     // 0x00B871B0
    void EndShadowCasters();                       // 0x00B86BB0
};
extern CEngineShadowRenderer* g_pShadowRenderer;   // 0x01436E38
struct CEngineSubPrimitiveRenderer {
    void RenderList(long list, bool sort);         // 0x00B849F0
    unsigned long GetListSize(long list);          // 0x00B842B0
    void ClearList(long list);                     // 0x00B847B0
};
extern CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer;   // 0x01436E74
struct CEngineLightingManager;
extern CEngineLightingManager* g_pLightingManager_01436ea0;   // 0x01436EA0

struct CEngineScene {
    void RenderOcclusion(long layer, const unsigned char (&settings)[0x130]);   // 0x00B94060
    void RenderLit(long layer, CEngineLightingManager* lighting, long mode, bool flag);   // 0x00B942A0
    void Render(long layer, CEngineLightingManager* lighting);   // 0x00B93F40
};
template <class T> struct CArray {
    T* First;
    T* Last;
    T* End;
    T* begin() { return First; }
    T* end() { return Last; }
};

struct CCamera {
    unsigned char _pad[0x78];
    unsigned char Flags;                           // +0x78
};
struct CEngineComponentRenderParams {
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
struct CEngineComponent {
    virtual void c00();
    virtual void Render(const CEngineComponentRenderParams& params);
};
struct CEnginePrimitiveRenderer : CEngineRendererBase, CEngineComponent {
    unsigned char         _pad26c[0x338 - 0x26c];
    CArray<CEngineScene*> Scenes;                  // +0x338
    virtual void Render(const CEngineComponentRenderParams& params);
    void PreRenderLayer(long layer);               // 0x00B32AD0
    void RenderOcclusionLayer();                   // 0x00B32610
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
        RenderOcclusionLayer();
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
        if (g_pWaterRenderer->Enabled) {
            g_pEngineSubPrimitiveRenderer->RenderList(0xe, true);
            if (g_pEngineSubPrimitiveRenderer->GetListSize(0xd) > 0) {
                g_pWaterRenderer->BeginReflection();
                g_pEngineSubPrimitiveRenderer->RenderList(0xd, true);
                g_pWaterRenderer->EndReflection();
            }
        } else {
            g_pEngineSubPrimitiveRenderer->ClearList(0xe);
            g_pEngineSubPrimitiveRenderer->ClearList(0xd);
        }
        break;
    case 0x100:
        if (g_pWaterRenderer->Enabled) {
            g_pEngineSubPrimitiveRenderer->RenderList(8, true);
            g_pEngineSubPrimitiveRenderer->RenderList(0xa, true);
        } else {
            g_pEngineSubPrimitiveRenderer->ClearList(8);
            g_pEngineSubPrimitiveRenderer->ClearList(0xa);
        }
        break;
    case 0x200:
        if (g_pWaterRenderer->Enabled)
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
            if (g_pShadowRenderer->BeginShadows()) {
                g_pEngineSubPrimitiveRenderer->RenderList(5, true);
                g_pShadowRenderer->EndShadows();
            }
            g_pEngineSubPrimitiveRenderer->ClearList(5);
        }
        break;
    case 0x10000000:
        if (g_pEngineSubPrimitiveRenderer->GetListSize(6) > 0) {
            g_pShadowRenderer->BeginShadowCasters();
            g_pEngineSubPrimitiveRenderer->RenderList(6, true);
            g_pShadowRenderer->EndShadowCasters();
            g_pEngineSubPrimitiveRenderer->ClearList(6);
        }
        break;
    default:
        for (CEngineScene** it = Scenes.begin(); it != Scenes.end(); ++it) {
            CEngineScene* scene = *it;
            if ((params.Flags & 1) && ShadowsActive)
                scene->RenderLit(params.Layer, g_pLightingManager_01436ea0, LightIndexMode, !SelfShadowing);
            else
                scene->Render(params.Layer, g_pLightingManager_01436ea0);
        }
        break;
    }
    g_pEngineSubPrimitiveRenderer->RenderList(0, true);
    if (params.Flags & 2)
        g_pEngineSubPrimitiveRenderer->RenderList(1, true);
    PostRenderLayer();
    g_pRenderStateManager->RestoreCaptureBlock();
}