// CEngineClothRenderer::Render2DClothPrimitive -- draw one 2D (flat, alpha-masked) cloth primitive (Fable.exe TLC, VC7.1) @ 0x00BB30A0, 3538 bytes, public thiscall, `ret 20h`
// Grade: RELOCATION_MATCH (3538v3538) -- release-mode assert (fable_assert.h) present; see Inferred below.
// Record-keyed residue closed by agentC8 (the bookmark test/or operand order of the SetAlphaTestEnable(false) expansion 0x00BB33B7 and the
//   load order of the SetFogEnable(false) expansion 0x00BB3D28 are c2 record-id ties: the first moves with the function's whole record
//   count, the second with the records created before the fog expansion; probe tools tools/om.py / meter.sh / combo.py):
//   shared helper texts (same text in the sibling 0x00BB3E80, re-graded RELOCATION_MATCH): FlushRender releases stream 0 with
//   PD3DDevice->SetStreamSource(0, 0, 0, 0) (the inline SelectVertexBuffer(0, 0) gives the same bytes but ~34 more records; this is what
//   moves 0x00BB33B7), SetTextureFactor's pixel path C4DVector(colour) (FableWin ??0C4DVector@@QAE@ABVCRGBFloatColour@@@Z), opt-ins
//   FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR, FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX (FableWin 28i/79b SAME),
//   FABLE_RETAIL_C3DCLOTHPRIMITIVE_GETPARTICLEPROGRAM_GET (FableWin 10i/22b) and FABLE_RETAIL_C3DMESHINFO_PEEKTEXTUREBANK_MEMBER (FableWin
//   10i/25b); GetCurrent2DAlphaMaskInfo with a named viewport-manager pointer (fwsize 16i/46b = FableWin SAME; the one-expression form is 15i/40b).
//   Row text: the tint built as a CRGBColour copy with the sub-primitive alpha and a named CRGBFloatColour, the mask position factors
//   through a const C4DVector&, named mesh and self-illumination values, and one release assert (program pointer, as in the sibling).
//   These six names / the assert are FITTED as a set (the fog load order needs the record count in an 8-wide window; this set sits at its
//   low edge and tolerates 6 more records); each one is a plain spelling of a value the function already has.
// Inferred: one FABLE_RETAIL_ASSERT (cloth_primitive.GetParticleProgram(), before the dereference; the same statement the sibling has). Dead
//   arm, no bytes; site and count FITTED with the names above.
// What it does: inside a scoped render-state capture block, set the alpha states from the sub-primitive render flags; enable vertex +
//   pixel shaders; set the 2D alpha-mask texture mode on stage 1 (point filtering unless the current viewport's mask filters); pass the
//   mask position factors to vertex constant 0; apply the cloth vertex shader for 0 lights and the 2D diffuse pixel shader; set the texture
//   factor from the tint and the sub-primitive alpha; fetch the diffuse texture (mipmap 1) and the 2D alpha-mask texture (flush and bail
//   while either preloads); set self illumination, the global lighting states, fog off; draw the polygons (RenderPolygons); flush.
// Identity: Ego_r.pdb / FableWin.pdb 0x02E73570 (mangled name = the Leaf line); callers CEngineClothSimulation2DAnimatedMesh::Render
//   0x00B81640 and the 2D static sibling. The last bool is never read.
// Callees (out of line): CRenderStateManager::RestoreCaptureBlock 0x00A05840; CShaderRenderManager::EnableVertexShaders 0x00987FE0 /
//   DisableVertexShaders 0x009880E0 / EnablePixelShaders 0x00988110 / DisablePixelShaders 0x00988190 / ApplyVertexShader 0x00988020 /
//   ApplyPixelShader 0x00988140 / SetUserVertexShaderConstant(K, const C3DVector&, float) 0x00989A60; __ftol2 0x00BFEA70;
//   CEngineLightingManager::UpdateGlobalLightingStates 0x00B46C80; CEnginePrimitiveManagerMeshBase::GetDiffuseTexture 0x00BA9360 /
//   Get2DAlphaMaskTexture 0x00BA9A70; CEngineClothRenderer::RenderPolygons 0x00BB2540; virtual PeekRenderInfoMeshBase (slot 29) /
//   GetMorphInfo (slot 32); D3D device SetStreamSource / SetIndices / SetTexture (FlushRender).
// Expanded inlines (FableWin.pdb names, the ones listed right after this row in FableWin are new here): StartCaptureBlock,
//   SetupAlphaBlendingStatesFromRenderFlags, CScopedShadersEnable, GetCurrent2DAlphaMaskInfo / GetCurrentAlphaMask,
//   Set2DAlphaMaskTextureMode / DisableTextureFiltering, ApplyTriVertexShader, GetPixelShader2DDiffuse, SetTextureFactor + GetRGBColour,
//   SetSelfIlluminationAmbient, FlushRender.
// Retail: 0x00BB30A0, 3538 bytes (0x00BB30A0..0x00BB3E71, last instruction ret 20h at 0x00BB3E6F)
// Leaf: ?Render2DClothPrimitive@CEngineClothRenderer@@QAEXABVCParticleSimulation@@ABVC3DClothPrimitive@@ABVC3DPrimitive2@@ABVCEngineSubPrimitiveMeshBase@@AAVCEngineInternalPrimitiveMeshBase@@ABVCMatrix3x4@@W4ERenderPassType@@_N@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTH2D
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_CLOTH2D
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE2DVIEWPORTMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_2DALPHAMASK
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CENGINECLOTHRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINECLOTHRENDERER_RENDER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_C3DPRIMITIVE2_RENDER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CENGINEMESHEFFECT
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_GETAPPROPRIATEMIPMAP
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_STATICBLOCKS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTHSHADERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHRENDER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTHRENDER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS
#define FABLE_RETAIL_CENGINESUBPRIMITIVEMESHBASE_ACCESSORS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_RESETOCCLUSIONINFO
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SUMS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_NAMED_SQUARED
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
#define FABLE_RETAIL_CGRAPHICINFO_GETWIDTH
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SPOTLIGHT
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD_DIRECT
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_CLOTH
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_ALLOCATE
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONINFO_ADDOCCLUSIONTEST
#define FABLE_RETAIL_CENGINESHADOWRENDERER_SHADOWFUNC
#define FABLE_RETAIL_C3DMESHMATERIAL_FLAGS
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_C3DMESHINFO_PEEKTEXTUREBANK_MEMBER
#define FABLE_RETAIL_C3DCLOTHPRIMITIVE_GETPARTICLEPROGRAM_GET
#define FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX
#define FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_GETRGBCOLOUR_NAMED_RESULT
#include "engine/CMatrix3x4.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineCamera.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineOcclusionManager.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEnginePrimitiveManagerMeshBase.h"
#include "engine/CEngineSubPrimitiveRepeatedStaticMesh.h"
#include "engine/CEngineInternalPrimitiveMeshBase.h"
#include "engine/CEngineInternalPrimitiveMorphedStaticMesh.h"
#include "engine/CEnginePrimitiveOcclusionInfo.h"
#include "engine/CEngineRenderInfoMeshBase.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CGraphicInfo.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DMeshMaterial.h"
#include "engine/C3DPrimitive2.h"
#include "engine/CParticleSimulation.h"
#include "engine/CParticleProgram.h"
#include "engine/CCountedPointer.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/C4DVector.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CEngineMeshEffect.h"
#include "engine/CArray.h"
#include "fable_assert.h"
#include "engine/C3DClothPrimitive.h"
#include "engine/CEngineClothRenderer.h"

#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEnginePrimitive2DViewportManager.h"
#include "engine/C2DAlphaMaskInfo.h"
#include "engine/GFDebugPDynamicCast.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }      // 0x01436E14
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }        // 0x01436E9C
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                                                // 0x01436EA0
namespace NEngineInternal { extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; }   // 0x01436E78
namespace NEngineInternal { extern CEngineOcclusionManager* GPOcclusionManager; }                                  // 0x01436E5C
namespace NEngineInternal { extern CEngineShadowRenderer* GPShadowRenderer; }                                // 0x01436E60
namespace NEngineInternal { extern CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo; }                          // 0x01436E84


FABLE_FORCEINLINE bool CShaderRenderManager::GetPixelShadersEnabled() const { return PixelShadersEnabled; }
FABLE_FORCEINLINE void CRenderStateManager::SetTextureFactor(unsigned long factor) { SetRenderState(factor, DeviceState.TextureFactor); }
FABLE_FORCEINLINE void CRenderManagerCore::FlushRender()
{
    PD3DDevice->SetStreamSource(0, 0, 0, 0);
    PD3DDevice->SetIndices(0);
    for (long stage = 0; stage < TextureStagesInUse; ++stage) {
        if (CurrentlySelectedTextures[stage]) {
            CurrentlySelectedTextures[stage] = 0;
            PD3DDevice->SetTexture(stage, 0);
        }
    }
    TextureStagesInUse = 0;
}
FABLE_FORCEINLINE void __fastcall CEnginePrimitiveRenderUtil::SetTextureFactor(const CRGBFloatColour& colour)
{
    if (NEngineInternal::GPShaderRenderManager->GetPixelShadersEnabled())
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(2, C4DVector(colour));
    else
        NEngineInternal::GPRenderManager->SetTextureFactor(colour.GetRGBColour().IntValue);
}
inline void __fastcall CEnginePrimitiveRenderUtil::ApplyVertexShader(const CVertexShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(shader, false);
}
inline void __fastcall CEnginePrimitiveRenderUtil::ApplyPixelShader(const CPixelShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(shader);
}
inline CEngineCamera& __fastcall CEnginePrimitiveRenderUtil::GetCurrentCamera()
{
    return *NEngineInternal::GPCamera;
}
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseOnly() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseOnly; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseColourTextureDiffuseAlpha() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseColourTextureDiffuseAlpha; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFill() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderOutlineSmoothFill; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFillAlpha() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderOutlineSmoothFillAlpha; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuse() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuse; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFogShadow() { return *NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseFogShadow; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFogShadowOutputFactor() { return *NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseFogShadowOutputFactor; }
inline EEngineOcclusionLayer __fastcall CEnginePrimitiveRenderUtil::GetCurrentOcclusionLayer()
{
    return NEngineInternal::GPPrimitiveRendererSharedData->GetCurrentOcclusionLayer();
}
inline bool __fastcall CEnginePrimitiveRenderUtil::GetOcclusionEnable()
{
    return NEngineInternal::GPPrimitiveRendererSharedData->GetOcclusionEnable();
}
inline float __fastcall CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier()
{
    return NEngineInternal::GPPrimitiveRendererSharedData->MipmapSizeMultiplier;
}
FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::SetupAlphaBlendingStatesFromRenderFlags(unsigned long render_flags, ERenderPassType pass, unsigned char alpha,
    unsigned char alpha_ref)
{
    if (pass == 1) {
        NEngineInternal::GPRenderManager->SetZBufferWriteEnable(true);
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
        NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
        NEngineInternal::GPRenderManager->SetAlphaRef(alpha_ref);
        NEngineInternal::GPRenderManager->SetColourWriteEnable(false);
    } else {
        if (render_flags & 2) {
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
            NEngineInternal::GPRenderManager->SetAlphaRef((alpha * alpha_ref) >> 8);
        } else {
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
            NEngineInternal::GPRenderManager->SetAlphaRef(0);
        }
        if (render_flags & 1) {
            NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
            NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
            NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
            NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
        } else {
            NEngineInternal::GPRenderManager->SetZBufferWriteEnable(true);
            NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
        }
    }
}

FABLE_FORCEINLINE void CEngineClothRenderer::ApplyTriVertexShader(unsigned long light_count, bool force, unsigned long render_flags)
{
    if (render_flags & 0x200) {
        if (render_flags & 0x10) {
            const CVertexShader& shader = TriVertexShadersSpotShadow[light_count < 5 ? light_count : 5];
            NEngineInternal::GPShaderRenderManager->ApplyVertexShader(shader, force);
        } else {
            const CVertexShader& shader = TriVertexShadersShadow[light_count < 5 ? light_count : 5];
            NEngineInternal::GPShaderRenderManager->ApplyVertexShader(shader, force);
        }
    } else {
        const CVertexShader& shader = TriVertexShaders[light_count < 5 ? light_count : 5];
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(shader, force);
    }
}

inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShader2DDiffuse(EEnginePrimitiveBlendMode blend_mode)
{
    if (blend_mode == ENGINE_PRIMITIVE_BLEND_ADDITIVE || blend_mode == ENGINE_PRIMITIVE_BLEND_ADDSMOOTH)
        return NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuseAdditiveAlpha;
    return NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuse;
}
inline const C2DAlphaMaskInfo* CEnginePrimitive2DViewportManager::GetCurrentAlphaMask() { return &AlphaMaskInfo; }
FABLE_FORCEINLINE const C2DAlphaMaskInfo* CEnginePrimitiveRenderUtil::GetCurrent2DAlphaMaskInfo()
{
    CEnginePrimitive2DViewportManager* manager = GFDebugPDynamicCast(NEngineInternal::GPPrimitiveTypeInfo->GetPrimitiveManager(ENGINE_PRIMITIVE_2D_VIEWPORT),
                                                                      (CEnginePrimitive2DViewportManager*)0);
    return manager->GetCurrentAlphaMask();
}
FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::DisableTextureFiltering(unsigned long stage)
{
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(stage, TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(stage, TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureMipMapMode(stage, TEXTURE_MIPMAP_DISABLE);
}
FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::Set2DAlphaMaskTextureMode(unsigned long stage, bool filter, bool clamp)
{
    if (!filter)
        DisableTextureFiltering(stage);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(stage, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(stage, TEXTURE_UV_CLAMP);
}

void CEngineClothRenderer::Render2DClothPrimitive(const CParticleSimulation& simulation, const C3DClothPrimitive& cloth_primitive,
    const C3DPrimitive2& mesh_primitive, const CEngineSubPrimitiveMeshBase& sub_primitive, CEngineInternalPrimitiveMeshBase& primitive,
    const CMatrix3x4& transform, ERenderPassType pass, bool block_on_load)
{
    FABLE_RETAIL_ASSERT(cloth_primitive.GetParticleProgram());
    const CParticleProgram& program = *cloth_primitive.GetParticleProgram();
    CRenderStateScopedCaptureBlock capture_block;
    SetupAlphaBlendingStatesFromRenderFlags(sub_primitive.PeekRenderFlags(), pass, sub_primitive.PeekAlpha(), sub_primitive.PeekParent()->AlphaRef);
    CScopedShadersEnable shaders(true, true);
    const C2DAlphaMaskInfo* alpha_mask = GetCurrent2DAlphaMaskInfo();
    Set2DAlphaMaskTextureMode(1, alpha_mask->FilterMask, true);
    const C4DVector& factors = alpha_mask->AlphaMaskPositionFactors;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, C3DVector(factors.X, factors.Y, factors.Z), factors.W);
    ApplyTriVertexShader(0, false, sub_primitive.PeekRenderFlags());
    ApplyPixelShader(GetPixelShader2DDiffuse(ENGINE_PRIMITIVE_BLEND_ALPHA));
    CRGBColour tint(primitive.TintColour);
    tint.A = sub_primitive.PeekAlpha();
    CRGBFloatColour tint_colour(tint);
    CEnginePrimitiveRenderUtil::SetTextureFactor(tint_colour);
    const CEngineRenderInfoMeshBase* render_info = primitive.PeekRenderInfoMeshBase();
    const CEngineLocalLightArray& lights = *render_info->Lights;
    const C3DMesh2* mesh = render_info->Mesh;
    const C3DMeshMaterial* material = mesh->GetMaterial(cloth_primitive.GetMaterialIndex());
    const CGraphicDataBank* graphic_bank = primitive.MeshInfo->PeekTextureBank();
    CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >* composite_surfaces = 0;
    CEngineInternalPrimitiveMorphInfo* morph_info = primitive.GetMorphInfo();
    if (morph_info)
        composite_surfaces = &morph_info->CompositeSurfaces;
    {
    const CTexture* textures[2];
    {
        bool preloading = false;
    textures[0] = CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(material, composite_surfaces, 1, preloading, graphic_bank, false);
    textures[1] = CEnginePrimitiveManagerMeshBase::Get2DAlphaMaskTexture(alpha_mask->AlphaMaskBankIndex, 0, preloading, alpha_mask->AlphaMaskBank,
                                                                         false);
        if (preloading) {
            NEngineInternal::GPRenderManager->FlushRender();
            return;
        }
    }
    long self_illumination = material->GetSelfIllumination();
    NEngineInternal::GPLightingManager->SetSelfIlluminationAmbient(self_illumination);
    NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
    bool two_sided = material->IsTwoSided();
    NEngineInternal::GPRenderManager->SetFogEnable(false);
    RenderPolygons(simulation, program, transform, lights, textures, 1, true, two_sided, sub_primitive.PeekRenderFlags(), true, true, 9, pass,
                   primitive);
    NEngineInternal::GPRenderManager->FlushRender();
    }
}
