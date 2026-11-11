// CEngineShadowRenderer::SetupShadowFuncRenderStates(long stage, const CMatrix3x4& texture_transform, const CColorBufferDepthPackParameters& pack_parameters) -- set up the shadow-map lookup for a render pass: nothing when the pack's depth-to-colour lookup stage is negative; attach the shadow buffer texture to the stage; for the colour-buffer technique attach the depth packing lookup texture to the pack's stage, point-filter both stages, wrap U / clamp V on the lookup stage and set the low-byte carry pixel constant (1/204.8, -1/204.8, 0, 0) and the low-repeat user vertex constant (128 x4), otherwise linear-filter the stage with a projected texture transform; then the shadow texture transform = texture_transform (as 4x4) * WorldToShadowBufferMatrixTextureSpace, and with the lighting manager's global shadow mode 1 the fade transform (1 / (FadeStart - FadeEnd), 0, 0, -FadeEnd / (FadeStart - FadeEnd))
// Grade: DIFFER(1837v1837) -- x87 term order of ONE 4-term sum (PostMultiply column 0, retail 0x00B5023F..0x00B5025E); every other instruction, register, slot and relocation matches
// Leaf: ?SetupShadowFuncRenderStates@CEngineShadowRenderer@@QAEXJABVCMatrix3x4@@ABUCColorBufferDepthPackParameters@1@@Z
// Flags: /d2inlT314
// Inline-budget: /d2inlT314 -- c2 inline trace (tools/inl.sh, c2.dll 0x1078D1AA): Initialise(const CMatrix3x4&) scores 79 (c1 79, f 1.0, inside the
//   ctor) and PostMultiply 313 (c1 178, f 0.5) against the default 74, so a plain compile calls both; retail expands both (0x00B501A9..0x00B50301)
//   and keeps their unreferenced COMDATs (Initialise 0x00B50770, PostMultiply 0x00B507E0, ctor 0x00B50AE0). 314 is the smallest threshold that
//   expands PostMultiply and changes no other site (the other 8 sites are already expanded). FABLE_FORCEINLINE on the three CMatrix4x4 members
//   gives byte-identical code (measured in fragments); no loop / guard shape can bring 313 under 74.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B4FC40, 1837 bytes, thiscall, `ret 0Ch`, esp frame (sub esp,90h; esi / edi pushed after the early-out).
// Identity (proven): Ego_r.pdb 0x009EDF60 / FableWin.pdb 0x02D73690 ?SetupShadowFuncRenderStates@CEngineShadowRenderer@@QAEXJABVCMatrix3x4@@ABUCColorBufferDepthPackParameters@1@@Z
//   (the 3-argument overload; the 2-argument overload 0x00B50510 forwards here with an identity matrix); 11 callers pass `this` = [0x01436E60]
//   and a CColorBufferDepthPackParameters(4, 5, 0) built in place (0x00BA2D5F, 0x00BD3EE1). Members read: SceneData (+0x0C)
//   .WorldToShadowBufferMatrixTextureSpace +0x8C, ShadowFadeStart / End +0xFC / +0x100, ShadowBufferTexture +0x2E8, RenderingTechnique +0x334;
//   the depth packing lookup texture is read through the engine's shadow renderer global (+0x32C of [0x01436E60]).
// Callees: CShaderRenderManager::SetUserVertexShaderConstant(unsigned long, const C4DVector&) 0x00989AB0, SetShadowTextureTransform(const CMatrix4x4&)
//   0x009896B0, SetShadowFadeTransform(const C4DVector&) 0x00989910 (Ego_r / FableWin), IDirect3DDevice9::SetTexture (vtable +0x104).
//   Inline: CRenderManagerCore::AttachTextureToStage / AttachD3DTextureToStage, CRenderStateManager::SetTextureFilterModeMag / Min,
//   SetTextureUVModeU / V, SetTextureTransformMode (SetRenderState / AddStateToUpdateList), CShaderRenderManager::SetPixelShaderConstant,
//   CMatrix4x4(const CMatrix3x4&) -> Initialise, CMatrix4x4::PostMultiply, CEngineLightingManager::PeekGlobalShadowMode,
//   CEngineShadowRenderer::GetShadowBufferTexture / GetDepthPackingTexture / PeekRenderingTechnique (all FableWin.pdb names).
// Globals: NEngineInternal::GPRenderManager 0x01436E18, NEngineInternal::GPShaderRenderManager 0x01436E14 (established), g_pEngineShadowRenderer
//   0x01436E60, g_pEngineLightingManager 0x01436E9C (settled spellings; see NOTES.md), the CRenderStateManager D3D tables 0x01396E58 / 0x01396E5C
//   (filter), 0x01396DC8 / 0x01396DD0 (UV), 0x01396FE0 (transform [5]).
// Residue (measured; pass 1 8 grades, pass 2 7 grades): retail sums column 0 of the inlined PostMultiply as M2*m20 + m30*M3 + m10*M1 + m00*M0,
//   ours as M2*m20 + m00*M0 + m30*M3 + m10*M1 (columns 1..3 and everything else identical). The order is c2's canonical sum order and moves
//   only with the condition accessor (direct RenderingTechnique also changes the induction base and all four columns); 12 natural variants
//   (direct members, switch, if-wrap, named technique / fade vector / scene-matrix reference, default ctor + Initialise, matrix declared early,
//   GetD3DTexture nesting, direct ShadowMode) leave it unchanged. See NOTES.md.
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CMATRIX4X4_FROMMATRIX3X4
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_PEEKGLOBALSHADOWMODE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#include "engine/CMatrix4x4.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CRenderTarget.h"
#include "engine/CShadowClipVolume.h"
#include "engine/CShadowRendererSceneData.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/C4DVector.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
extern CEngineShadowRenderer* g_pEngineShadowRenderer;                             // 0x01436E60
extern CEngineLightingManager* g_pEngineLightingManager;                           // 0x01436E9C

void CEngineShadowRenderer::SetupShadowFuncRenderStates(long stage, const CMatrix3x4& texture_transform,
                                                        const CColorBufferDepthPackParameters& pack_parameters)
{
    if (pack_parameters.DepthToColourLookupTextureStage < 0)
        return;
    NEngineInternal::GPRenderManager->AttachTextureToStage(stage, &GetShadowBufferTexture());
    if (PeekRenderingTechnique() == SHADOW_RENDERING_TECHNIQUE_COLOUR_BUFFER) {
        NEngineInternal::GPRenderManager->AttachTextureToStage(pack_parameters.DepthToColourLookupTextureStage, &g_pEngineShadowRenderer->GetDepthPackingTexture());
        NEngineInternal::GPRenderManager->SetTextureFilterModeMag(stage, TEXTURE_FILTER_POINT);
        NEngineInternal::GPRenderManager->SetTextureFilterModeMin(stage, TEXTURE_FILTER_POINT);
        NEngineInternal::GPRenderManager->SetTextureFilterModeMag(pack_parameters.DepthToColourLookupTextureStage, TEXTURE_FILTER_POINT);
        NEngineInternal::GPRenderManager->SetTextureFilterModeMin(pack_parameters.DepthToColourLookupTextureStage, TEXTURE_FILTER_POINT);
        NEngineInternal::GPRenderManager->SetTextureUVModeU(pack_parameters.DepthToColourLookupTextureStage, TEXTURE_UV_WRAP);
        NEngineInternal::GPRenderManager->SetTextureUVModeV(pack_parameters.DepthToColourLookupTextureStage, TEXTURE_UV_CLAMP);
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(pack_parameters.LowByteCarryPixelShaderConstant,
                                                                       C4DVector(0.0048828125f, -0.0048828125f, 0.0f, 0.0f));
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(pack_parameters.LowRepeatFactorUserVertexShaderConstant,
                                                                            C4DVector(128.0f, 128.0f, 128.0f, 128.0f));
    } else {
        NEngineInternal::GPRenderManager->SetTextureFilterModeMag(stage, TEXTURE_FILTER_LINEAR);
        NEngineInternal::GPRenderManager->SetTextureFilterModeMin(stage, TEXTURE_FILTER_LINEAR);
        NEngineInternal::GPRenderManager->SetTextureTransformMode(stage, TEXTURE_TRANSFORM_PROJECTED);
    }
    CMatrix4x4 shadow_texture_transform(texture_transform);
    shadow_texture_transform.PostMultiply(SceneData.WorldToShadowBufferMatrixTextureSpace);
    NEngineInternal::GPShaderRenderManager->SetShadowTextureTransform(shadow_texture_transform);
    if (g_pEngineLightingManager->PeekGlobalShadowMode() == 1) {
        float fade_scale = 1.0f / (SceneData.ShadowFadeStart - SceneData.ShadowFadeEnd);
        NEngineInternal::GPShaderRenderManager->SetShadowFadeTransform(C4DVector(fade_scale, 0.0f, 0.0f, -(fade_scale * SceneData.ShadowFadeEnd)));
    }
}
