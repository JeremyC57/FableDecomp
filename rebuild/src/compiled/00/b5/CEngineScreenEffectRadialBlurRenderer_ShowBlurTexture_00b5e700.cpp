// CEngineScreenEffectRadialBlurRenderer::ShowBlurTexture() -- debug view: draw the blur texture picked by ShowDebug (1: the first 256 target, else the second) over the back buffer inside a render-state capture block, with vertex constant layout 2 and the vertex/pixel shaders forced on, all restored afterwards
// Leaf: ShowBlurTexture
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5E700, length 816 bytes (body 0x323 + int3 padding), plain `ret`.
// Twin of ShowGlowTexture 0x00B61690 plus a layout save: CShaderRenderManager::SetVertexShaderConstantLayout(long)
//   @ 0x0098B5E0 on the static shader manager @ 0x013BC470 (current layout index +0x8), restored at exit. Same callees
//   otherwise (Enable/DisableVertexShaders 0x00987FE0/0x009880E0, Enable/DisablePixelShaders 0x00988110/0x00988190,
//   ApplyVertexShader 0x00988020, ApplyPixelShader 0x00988140, inlined PrepareForRender(0), DrawAsIndexedTris 0x00A0AEA0
//   with the quad indices @ 0x0139A550, RestoreCaptureBlock 0x00A05840), tracked states +0x28b8/+0x2868 (0) and +0x2890
//   (0x01396FB8) through the value-first free SetState(value, state, core). The three saves are inline RAII frame objects
//   (capture block, layout, shader enables) destroyed in reverse order, as retail restores them.
// Layout: engine/CEngineScreenEffectRadialBlurRenderer.h (BlurTexture256_1 +0x0c, BlurTexture256_2 +0x10,
//   ScreenCopyVertexShader +0x20, TextureOnlyPixelShader +0x40, ShowDebug +0x1c4, BlurToBackBufferVerts +0x1cc).
#include "fable_compiler.h"
// NF45 (round 7): the render core is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h; CRenderManagerCore / CRenderStateManager at
//   offset 0): the three states are the shared inline setters AlphaBlendEnable (+0x28B8) / ZBufferEnable (+0x2868) false and
//   PolyCullMode (+0x2890) [POLY_CULL_NONE] through D3DPolyCullModeTable 0x01396FB0, and the quad is
//   ?DrawAsIndexedTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KPBXJJPAXJ@Z 0x00A0AEA0 (repo opt-in DRAWASINDEXEDTRIS; const
//   CTexture* texture, the index table passed as the PDB's void*). The capture open / RestoreCaptureBlock pair on 0x013CB508 is a
//   CRenderStateScopedCaptureBlock (engine/CRenderStateCaptureBlock.h, CRenderStateCaptureBlock::StateManager). Was a local
//   CRenderManagerCoreView / CTrackedRenderState / SetState model, g_pRenderManagerCore / g_pRenderStateManager,
//   g_renderStateDefault_01396fb8 and a local struct CRenderStateCaptureBlock.
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_SCOPED_FORCEINLINE
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"

class C4DVector { public: float X, Y, Z, W; };
class CTexture;


extern const unsigned short g_QuadIndices_0139a550[6]; // 0x0139A550

struct CShaderEnableState {                             // the static shader manager object @ 0x013BC470
    unsigned char _pad000[0x8];
    long          CurrentVSConstantLayoutIndex;         // +0x008
    unsigned char _pad00c[0x390 - 0xc];
    bool          VertexShadersEnabled;                 // +0x390
    bool          PixelShadersEnabled;                  // +0x391
    unsigned char _pad392[2];
    long          _pad394;
    void SetVertexShaderConstantLayout(long layout);    // 0x0098B5E0
    void EnableVertexShaders();                         // 0x00987FE0
    void DisableVertexShaders();                        // 0x009880E0
    void EnablePixelShaders();                          // 0x00988110
    void DisablePixelShaders();                         // 0x00988190
    FABLE_FORCEINLINE void SetVertexShadersEnabled(bool enable)
    {
        if (enable != VertexShadersEnabled) {
            if (enable)
                EnableVertexShaders();
            else
                DisableVertexShaders();
        }
    }
    FABLE_FORCEINLINE void SetPixelShadersEnabled(bool enable)
    {
        if (enable != PixelShadersEnabled) {
            if (enable)
                EnablePixelShaders();
            else
                DisablePixelShaders();
        }
    }
};
extern CShaderEnableState g_ShaderRenderManager;        // 0x013BC470

template <class T> class CArray { public:
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
};
class CShaderConstantRange { public:
    void* _vptr;                                        // +0
    long Offset;                                        // +4
    unsigned long Count;                                // +8
};
class CVertexShaderConstantLayout { public:
    void* _vptr;                                        // +0x00
    CShaderConstantRange Reserved;                      // +0x04
    unsigned char _pad10[0x130];                        // +0x10 the other ranges
    CArray<C4DVector> ReservedPresets;                  // +0x140
};
class CVertexShader;
class CPixelShader;

class CShaderRenderManager {
public:
    void ApplyVertexShader(CVertexShader* shader, bool force);                  // 0x00988020
    void ApplyPixelShader(CPixelShader* shader);                                // 0x00988140

    FABLE_FORCEINLINE void PrepareForRender(unsigned long flags)
    {
        if (UpdateFlags & 4)
            SetVSConstants(CurrentVSConstantLayout->Reserved.Offset, CurrentVSConstantLayout->ReservedPresets.GetData(),
                           CurrentVSConstantLayout->ReservedPresets.Size());
        if (UpdateFlags & 1)
            UpdatePixelShader();
        if (UpdateFlags & 2)
            UpdatePixelShaderConstants();
        if ((flags & 0x400) && (UpdateFlags & 0x20))
            UpdateBoneTransform();
        if (flags & 0x40) {
            if (UpdateFlags & 0x1000)
                UpdateWorldSpaceTextureTransform();
        } else if (flags & 0x80) {
            if (UpdateFlags & 0x800)
                UpdateObjectSpaceTextureTransform();
        }
        if (flags & 0x200) {
            if (UpdateFlags & 0x2000)
                UpdateWorldSpaceFogTransform();
        } else if (flags & 0x100) {
            if (UpdateFlags & 0x4000)
                UpdateObjectSpaceFogTransform();
        }
        if (UpdateFlags & 4)
            UpdateAmbient();
        if (UpdateFlags & 0x20000)
            UpdateFogColour();
        if (UpdateFlags & 0x400)
            UpdateLightGlobals();
        if (flags & 8) {
            if (UpdateFlags & 0x100)
                UpdateObjectSpaceLighting(flags);
        } else if (flags & 0x10) {
            if (UpdateFlags & 0x200)
                UpdateWorldSpaceLighting();
        }
        if ((flags & 0x20) && (UpdateFlags & 0x8000))
            UpdateLocalisedSphereMapping();
        if (!(flags & 1) && !WorldMatrixIsIdentity)
            ResetWorldTransform();
        if (UpdateFlags & 0x80)
            UpdateCombinedProjectionTransform();
        if ((flags & 4) && (UpdateFlags & 8))
            UpdateCameraPos();
    }

    void* _vptr;                                        // +0x000
    CVertexShaderConstantLayout* CurrentVSConstantLayout;   // +0x004
    unsigned char _pad008[0x1e0];                       // +0x008
    bool WorldMatrixIsIdentity;                         // +0x1e8
    unsigned char _pad1e9[0x1ef];                       // +0x1e9
    unsigned long UpdateFlags;                          // +0x3d8

private:
    void SetVSConstants(long reg, const void* data, unsigned long count);   // 0x0098B930
    void UpdatePixelShader();                           // 0x00988A20
    void UpdatePixelShaderConstants();                  // 0x00989BF0
    void UpdateBoneTransform();                         // 0x0098A8E0
    void UpdateWorldSpaceTextureTransform();            // 0x009894B0
    void UpdateObjectSpaceTextureTransform();           // 0x00989230
    void UpdateWorldSpaceFogTransform();                // 0x00989640
    void UpdateObjectSpaceFogTransform();               // 0x00989540
    void UpdateAmbient();                               // 0x00989760
    void UpdateFogColour();                             // 0x009897C0
    void UpdateLightGlobals();                          // 0x0098A760
    void UpdateObjectSpaceLighting(unsigned long flags);   // 0x00989CB0
    void UpdateWorldSpaceLighting();                    // 0x0098A540
    void UpdateLocalisedSphereMapping();                // 0x00988E80
    void ResetWorldTransform();                         // 0x00988290
    void UpdateCombinedProjectionTransform();           // 0x00988A50
    void UpdateCameraPos();                             // 0x00989700
};
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }    // 0x01436E14
struct CShaderEnableSaver {                             // saves and forces the shader enables, restores them on exit
    bool m_vertexShaders;
    bool m_pixelShaders;
    FABLE_FORCEINLINE CShaderEnableSaver(bool vertexShaders, bool pixelShaders)
    {
        m_vertexShaders = g_ShaderRenderManager.VertexShadersEnabled;
        g_ShaderRenderManager.SetVertexShadersEnabled(vertexShaders);
        m_pixelShaders = g_ShaderRenderManager.PixelShadersEnabled;
        g_ShaderRenderManager.SetPixelShadersEnabled(pixelShaders);
    }
    FABLE_FORCEINLINE ~CShaderEnableSaver()
    {
        g_ShaderRenderManager.SetVertexShadersEnabled(m_vertexShaders);
        g_ShaderRenderManager.SetPixelShadersEnabled(m_pixelShaders);
    }
};
struct CShaderLayoutSaver {                             // selects a vertex constant layout, restores the previous one on exit
    long m_layout;
    FABLE_FORCEINLINE CShaderLayoutSaver(long layout)
    {
        m_layout = g_ShaderRenderManager.CurrentVSConstantLayoutIndex;
        g_ShaderRenderManager.SetVertexShaderConstantLayout(layout);
    }
    FABLE_FORCEINLINE ~CShaderLayoutSaver() { g_ShaderRenderManager.SetVertexShaderConstantLayout(m_layout); }
};

struct CEngineScreenEffectRadialBlurRenderer_Methods : CEngineScreenEffectRadialBlurRenderer {
    void ShowBlurTexture();
};

void CEngineScreenEffectRadialBlurRenderer_Methods::ShowBlurTexture()
{
    CRenderStateScopedCaptureBlock capture;
    CShaderLayoutSaver layout(2);
    CShaderEnableSaver shaders(true, true);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader((CVertexShader*)ScreenCopyVertexShader, false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader((CPixelShader*)TextureOnlyPixelShader);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
    NEngineInternal::GPRenderManager->SetZBufferEnable(false);
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    const CTexture* texture;
    switch (ShowDebug) {
    case 1:
        texture = BlurTexture256_1;
        break;
    default:
        texture = BlurTexture256_2;
        break;
    }
    NEngineInternal::GPRenderManager->DrawAsIndexedTris(&texture, 1, BlurToBackBufferVerts, 0x18, 4, const_cast<unsigned short*>(g_QuadIndices_0139a550), 2);
}