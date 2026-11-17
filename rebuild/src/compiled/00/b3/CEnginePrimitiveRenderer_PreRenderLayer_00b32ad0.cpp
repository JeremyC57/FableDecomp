// CEnginePrimitiveRenderer::PreRenderLayer(EEngineRenderLayer layer) -- before a render layer: remember the shader enables, then (except for the shadow-buffer layer) force vertex shaders on; the bounding-box layer turns pixel shaders off, resets the world transform and sets its seven fixed tracked render states, every other layer turns pixel shaders on and enables fog for the fogged layers; finally re-initialise the global lights and select the occlusion layer
// Leaf: ?PreRenderLayer@CEnginePrimitiveRenderer@@QAEXW4EEngineRenderLayer@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B32AD0, length 960 bytes (body 0x3b3 + int3 padding), `ret 4`.
// Identity: Ego_r.pdb 0x009D35F0 / FableWin.pdb 0x02DA92D0 ?PreRenderLayer@CEnginePrimitiveRenderer@@QAEXW4EEngineRenderLayer@@@Z.
// Port (NF68, round 7): the local CShaderRenderManager / CEngineLightingManager / CEnginePrimitiveRenderer models are replaced by the
//   shared headers: engine/CEnginePrimitiveRenderer.h over engine/CEnginePrimitiveRendererSharedData.h (Ego_r.pdb OldVertexShadersEnable
//   +0x261, OldPixelShadersEnable +0x260, OcclusionEnable +0xBD, CurrentOcclusionLayer +0x78; the old row called them saved enables,
//   UseFixedLights and LightIndexMode), engine/CShaderRenderManager.h (FableWin.pdb SetVertexShadersEnable / SetPixelShadersEnable(bool,
//   bool*) inlines and the inline getters) and engine/CRenderManager.h's CRenderStateManager setters, plus NF68 overlay opt-ins
//   (PreRenderLayer / InitialiseGlobalLights declarations, the inline shader getters, EnableFog's declaration).
// Callees: CShaderRenderManager::EnableVertexShaders 0x00987FE0 / DisableVertexShaders 0x009880E0 / EnablePixelShaders 0x00988110 /
//   DisablePixelShaders 0x00988190 / ResetWorldTransform 0x00988290, CEngineLightingManager::EnableFog 0x00B46890,
//   InitialiseGlobalLights 0x00B32300.
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER_PRERENDERLAYER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_RENDERSETTINGS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SILHOUETTES
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CSHADERRENDERMANAGER_GETSHADERSENABLED_INLINE
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_ENABLEFOG
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#include "engine/CEngineLightingManager.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEnginePrimitiveRenderer.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }    // 0x01436E9C

void CEnginePrimitiveRenderer::PreRenderLayer(EEngineRenderLayer layer)
{
    OldVertexShadersEnable = NEngineInternal::GPShaderRenderManager->GetVertexShadersEnabled();
    OldPixelShadersEnable = NEngineInternal::GPShaderRenderManager->GetPixelShadersEnabled();
    if (layer == RENDER_LAYER_SHADOW_BUFFER_RENDER)
        return;
    NEngineInternal::GPShaderRenderManager->SetVertexShadersEnable(true, 0);
    if (layer == RENDER_LAYER_BOUNDING_BOXES) {
        NEngineInternal::GPShaderRenderManager->SetPixelShadersEnable(false, 0);
        NEngineInternal::GPShaderRenderManager->ResetWorldTransform();
        NEngineInternal::GPRenderManager->SetTextureOperation(0, TEXTURE_OP_DISABLE);
        NEngineInternal::GPRenderManager->SetTextureAlphaOperation(0, TEXTURE_OP_DISABLE);
        NEngineInternal::GPRenderManager->SetColourWriteEnable(false);
        NEngineInternal::GPRenderManager->SetFogEnable(false);
        NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
        NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
    } else {
        NEngineInternal::GPShaderRenderManager->SetPixelShadersEnable(true, 0);
        if (layer == RENDER_LAYER_MESHES_OCCLUSION || layer == RENDER_LAYER_MESHES_NO_OCCLUSION || layer == RENDER_LAYER_NON_SORTED_PRIMITIVES ||
            layer == RENDER_LAYER_STENCIL_MESHES || layer == RENDER_LAYER_POSTWATER_MESHES || layer == RENDER_LAYER_LANDSCAPE_BASE ||
            layer == RENDER_LAYER_LANDSCAPE_POST_SOLID_SCENE)
            NEngineInternal::GPLightingManager->EnableFog();
    }
    InitialiseGlobalLights();
    CurrentOcclusionLayer = OcclusionEnable ? OCCLUSION_LAYER_PRIMARY_VIEW : OCCLUSION_LAYER_NONE;
}
