// CEngineClothRenderer::RenderClothPrimitive -- draw one cloth primitive (Fable.exe TLC, VC7.1) @ 0x00BB3E80, 4540 bytes, public thiscall, `ret 20h`,
// Grade: RELOCATION_MATCH (4540v4540) -- release-mode asserts (fable_assert.h) present; see Inferred below.
// ebp frame (sub esp,48h), no EH frame, no /GS, no pragma. Matches with the repo DEFAULT CRenderStateManager
//   SetRenderState / AddStateToUpdateList text and the eleven release-mode asserts below (fable_assert.h, FABLE_RETAIL_ASSERT_DEAD_ARM).
// agentC8 (2026-11-13) re-fit after the shared helper texts changed for the same-TU sibling 0x00BB30A0 (one text per helper; both rows
//   graded with them): FlushRender releases stream 0 with PD3DDevice->SetStreamSource(0, 0, 0, 0) (was the inline SelectVertexBuffer(0, 0);
//   same bytes), SetTextureFactor's pixel path builds C4DVector(colour) (FableWin ??0C4DVector@@QAE@ABVCRGBFloatColour@@@Z), opt-ins
//   FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR (existing), FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX (existing; fwsize = FableWin
//   28i/79b), FABLE_RETAIL_C3DCLOTHPRIMITIVE_GETPARTICLEPROGRAM_GET and FABLE_RETAIL_C3DMESHINFO_PEEKTEXTUREBANK_MEMBER (new, FableWin
//   10i/22b and 10i/25b). With them this row needs two more dead arms (program pointer before its dereference, the outline mesh effect in the
//   first outline arm): count eleven, FITTED (each arm moves the or-operand order by one 32-record step here; window: exactly these).
// Inferred: eleven FABLE_RETAIL_ASSERT statements (nine by agentC6, two by agentC8) (owner rule "Inferred statements", 2026-11-12; they emit no bytes). Retail bytes that
//   need them: the 16 inline SetRenderState expansions 0x00BB46E0..0x00BB4E80 -- retail tests `test CurrentBookmarkMask,mask` in the
//   first 8 and `test mask,CurrentBookmarkMask` (or-operand order swapped) in the last 8 (first swapped block 0x00BB49F2 area; ours
//   without the dead arms: 11 / 5, i.e. 0x00BB49F2, 0x00BB4A69, 0x00BB4AD9 come out the other way), and CurrentBookmarkMask loaded
//   before the state mask in 15 of 16 (0x00BB4C15 / 0x00BB4C5C). Both orders are c2 record-id ties: the dead arm of each release
//   ASSERT is turned into IL that c2 deletes after numbering, so the count of such arms (not their position) moves the operand order;
//   arms whose condition has no inline call leave the load order alone. Measured window with this macro: 9..10 asserts (8 -> 9/7
//   split, 11 -> 7/9). Evidence for asserts here: FableWin.pdb's debug body of this function is 566 /Od instructions v 440 for this
//   text at /Od /EHsc (+126 = about TWO debug asserts at ~63 instructions each); the count (nine, now eleven) is FITTED to the window (the
//   conditions are the pointers / ranges the function dereferences, chosen as plausible, not recovered). Ruled out: the
//   DESIRED / BOOKMARK / DIRTY setter opt-ins (no count lines L and O up), 0..130 dead named locals except 124..128 (not a
//   statement a programmer writes), class-typed temporaries in the dead arm (change the live register allocation), dead arms in
//   the inline SetRenderState / AddStateToUpdateList (change the frame and inlining: 4514 / 4704 bytes), TU flags.
// What it does: inside a scoped render-state capture block, allocate an occlusion handle and add a pixel-count test when the current
//   occlusion layer has occlusion info and occlusion is enabled; enable vertex + pixel shaders, specular and fog; set the texture factor
//   from the primitive tint and the sub-primitive alpha; set the theme channel and global lighting states; pick the diffuse mipmap from
//   the camera distance and fetch the diffuse texture (flush and bail while it is still preloading); per pass set the outline / stencil
//   states and shaders, or (default) the stencil mask, the light-count vertex shader, the alpha states, self illumination, the shadow
//   pixel shader and shadow functions; draw the polygons (RenderPolygons) inside the occlusion test; flush and restore the defaults.
// Identity: Ego_r.pdb 0x00A69DA0 / FableWin.pdb 0x017C3B8E (mangled name = the Leaf line); callers CEngineClothSimulationStaticMesh::Render
//   0x00B812F0 (call 0x00B81426) and CEngineClothSimulationAnimatedMesh::Render 0x00B814B0 (0x00B81612). The last bool is never read.
// Callees (out of line): CRenderStateManager::RestoreCaptureBlock 0x00A05840; CShaderRenderManager::EnableVertexShaders 0x00987FE0 /
//   DisableVertexShaders 0x009880E0 / EnablePixelShaders 0x00988110 / DisablePixelShaders 0x00988190 / ApplyVertexShader 0x00988020 /
//   ApplyPixelShader 0x00988140; std::vector<unsigned long>::_M_insert_overflow 0x00464ACE; memmove 0x00BFEAE6 (dead arm); __ftol2
//   0x00BFEA70; CEngineLightingManager::UpdateGlobalLightingStates 0x00B46C80 / SetupShaderSpotLight 0x00B47390;
//   CEnginePrimitiveManagerMeshBase::GetDiffuseTexture 0x00BA9360; CEnginePrimitiveRenderUtil::SetupOutlineSmoothStencilStates 0x00BEFD90 /
//   SetupOutlineSmoothAlphaStencilMaskStates 0x00BF01E0 / SetupOutlineSmoothFillStates 0x00BF2240 / SetupOutlineStencilStates 0x00BF0740 /
//   SetupOutlineFillStates 0x00BF0B20 / SetupStencilMask 0x00BF1030; CEngineShadowRenderer::SetupShadowFuncRenderStates 0x00B4FC40;
//   CEngineOcclusionManager::StartOcclusionTest 0x00B8D090 / EndOcclusionTest 0x00B8D0A0; CEngineClothRenderer::RenderPolygons 0x00BB2540;
//   virtual PeekRenderInfoMeshBase (slot 29) / GetMorphInfo (slot 32); D3D device SetStreamSource / SetIndices / SetTexture (FlushRender).
// Expanded inlines (no retail copy; FableWin.pdb names): StartCaptureBlock, CScopedShadersEnable, AllocateOccluderHandle, AddOcclusionTest /
//   ClearHandles, SetTextureFactor (RenderUtil) + GetRGBColour, SetCurrentEnvironmentThemeChannel, GetAppropriateMipmap, ApplyTriVertexShader,
//   SetupAlphaBlendingStatesFromRenderFlags, SetSelfIlluminationAmbient, SetEnableFadedShadowFactor, FlushRender,
//   RestoreDefaultPrimitiveParameters.
// Retail-asm: 0x00BB43EA..0x00BB4411 `fld [ebp-20h] ; fsub [ebp-18h] ; fistp dword ptr [ebp+0Ch] ; fild [ebp+0Ch] ; fadd [ebp+10h] ;
//   fstp [ebp-24h] ; mov eax,[ebp-20h] ; cmp eax,[ebp-24h] ; jne ; inc dword ptr [ebp+0Ch]` = the shared engine/GFFloatToLongFloor.h helper
//   (mipmap size floor inside the expanded GetAppropriateMipmap). Per-site attempts table: not built yet (row not matched).
// Scopes: the texture lives in a block opened after the mipmap (shares retail's [ebp-24h] word with the floor temporary) and preloading in
//   an inner block that ends before the pass dispatch (lands in the dead parameter word +0Fh, shared with the alpha-ref byte).
// Retail: 0x00BB3E80, 4540 bytes (0x00BB3E80..0x00BB503B, last instruction ret 20h)
// Leaf: ?RenderClothPrimitive@CEngineClothRenderer@@QAEXABVCParticleSimulation@@ABVC3DClothPrimitive@@ABVC3DPrimitive2@@ABVCEngineSubPrimitiveMeshBase@@AAVCEngineInternalPrimitiveMeshBase@@ABVCMatrix3x4@@W4ERenderPassType@@_N@Z
#include "fable_compiler.h"
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
#define FABLE_RETAIL_C3DVECTOR_NAMED_RESULT_SUB
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
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFLimit.h"
#include "fable_assert.h"
#include "engine/C3DClothPrimitive.h"
#include "engine/CEngineClothRenderer.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }      // 0x01436E14
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }        // 0x01436E9C
extern CEngineCamera* g_pEngineCamera;                                                // 0x01436EA0
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer;                                // 0x01436E78
extern CEngineOcclusionManager* g_pOcclusionManager;                                  // 0x01436E5C
extern CEngineShadowRenderer* g_pEngineShadowRenderer;                                // 0x01436E60
extern unsigned char g_highestSetBitTable[256];                                       // 0x0143B990
extern const float DELTA_NOTIONAL_ZERO;                                               // 0x0129BA3C


inline unsigned long FABLE_FASTCALL GFGetTopBitInByte(unsigned char value)
{
    return g_highestSetBitTable[value];
}
FABLE_FORCEINLINE const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
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
    return *g_pEngineCamera;
}
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseOnly() { return g_pPrimitiveRenderer->PixelShaderDiffuseOnly; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseColourTextureDiffuseAlpha() { return g_pPrimitiveRenderer->PixelShaderDiffuseColourTextureDiffuseAlpha; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFill() { return g_pPrimitiveRenderer->PixelShaderOutlineSmoothFill; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFillAlpha() { return g_pPrimitiveRenderer->PixelShaderOutlineSmoothFillAlpha; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuse() { return g_pPrimitiveRenderer->PixelShaderDiffuse; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFogShadow() { return *g_pPrimitiveRenderer->PixelShaderDiffuseFogShadow; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFogShadowOutputFactor() { return *g_pPrimitiveRenderer->PixelShaderDiffuseFogShadowOutputFactor; }
inline EEngineOcclusionLayer __fastcall CEnginePrimitiveRenderUtil::GetCurrentOcclusionLayer()
{
    return g_pPrimitiveRenderer->GetCurrentOcclusionLayer();
}
inline bool __fastcall CEnginePrimitiveRenderUtil::GetOcclusionEnable()
{
    return g_pPrimitiveRenderer->GetOcclusionEnable();
}
inline float __fastcall CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier()
{
    return g_pPrimitiveRenderer->MipmapSizeMultiplier;
}
FABLE_FORCEINLINE void CEnginePrimitiveOcclusionInfo::AddOcclusionTest(unsigned long handle)
{
    if (g_pOcclusionManager->CurrentFrame != TestFrame)
        ClearHandles();
    OcclusionTestHandles.push_back(handle);
    TestFrame = g_pOcclusionManager->CurrentFrame;
}
FABLE_FORCEINLINE unsigned long FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap(const C3DVector& camera_position,
    const CMatrix3x4& transform, const C3DPrimitive2& mesh_primitive, float scale, const C3DMeshMaterial* material,
    const CGraphicDataBank* graphic_bank)
{
    C3DBoundingSphere sphere = mesh_primitive.GetBoundingSphere();
    transform.TransformPoint(sphere.Center);
    unsigned long mipmap = 0;
    float distance = ((camera_position - sphere.Center).GetMagnitude() - sphere.GetRadius() * scale) * mesh_primitive.GetAverageTextureStretch();
    if (distance > DELTA_NOTIONAL_ZERO) {
        long graphic_index = material->GetDiffuseMapBankIndex();
        if (graphic_index > 0) {
            const CGraphicInfo* graphic = graphic_bank->PeekGraphic(graphic_index);
            float ratio = (float)(long)graphic->GetWidth() / (CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier() / distance * scale);
            long size = GFFloatToLongFloor(ratio);
            GFLimit(size, 1, 255);
            mipmap = GFGetTopBitInByte((unsigned char)size);
        }
    }
    return mipmap;
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

void CEngineClothRenderer::RenderClothPrimitive(const CParticleSimulation& simulation, const C3DClothPrimitive& cloth_primitive,
    const C3DPrimitive2& mesh_primitive, const CEngineSubPrimitiveMeshBase& sub_primitive, CEngineInternalPrimitiveMeshBase& primitive,
    const CMatrix3x4& transform, ERenderPassType pass, bool block_on_load)
{
    FABLE_RETAIL_ASSERT(pass < RENDER_PASS_COUNT);
    FABLE_RETAIL_ASSERT(cloth_primitive.GetParticleProgram());
    const CParticleProgram& program = *cloth_primitive.GetParticleProgram();
    CRenderStateScopedCaptureBlock capture_block;
    bool occlusion_test = false;
    unsigned long occlusion_handle = 0;
    EEngineOcclusionLayer layer = CEnginePrimitiveRenderUtil::GetCurrentOcclusionLayer();
    if (layer != OCCLUSION_LAYER_NONE && !primitive.OcclusionInfo[layer].IsNull() && CEnginePrimitiveRenderUtil::GetOcclusionEnable()) {
        FABLE_RETAIL_ASSERT(layer < OCCLUSION_LAYER_MAX);
        CEnginePrimitiveOcclusionInfo& occlusion_info = primitive.GetOcclusionInfo(layer);
        occlusion_handle = g_pOcclusionManager->AllocateOccluderHandle();
        occlusion_info.AddOcclusionTest(occlusion_handle);
        occlusion_test = true;
    }
    CScopedShadersEnable shaders(true, true);
    NEngineInternal::GPRenderManager->SetSpecularEnable(true);
    NEngineInternal::GPRenderManager->SetFogEnable(true);
    CEnginePrimitiveRenderUtil::SetTextureFactor(CRGBFloatColour(CRGBColour(primitive.TintColour.R, primitive.TintColour.G, primitive.TintColour.B,
                                                                              sub_primitive.PeekAlpha())));
    const CEngineRenderInfoMeshBase* render_info = primitive.PeekRenderInfoMeshBase();
    FABLE_RETAIL_ASSERT(render_info);
    NEngineInternal::GPLightingManager->SetCurrentEnvironmentThemeChannel(primitive.ThemeChannelDesc);
    NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
    FABLE_RETAIL_ASSERT(render_info->Lights);
    const CEngineLocalLightArray& lights = *render_info->Lights;
    FABLE_RETAIL_ASSERT(render_info->Mesh);
    const C3DMeshMaterial* material = render_info->Mesh->GetMaterial(cloth_primitive.GetMaterialIndex());
    FABLE_RETAIL_ASSERT(material);
    FABLE_RETAIL_ASSERT(primitive.MeshInfo);
    const CGraphicDataBank* graphic_bank = primitive.MeshInfo->PeekTextureBank();
    FABLE_RETAIL_ASSERT(graphic_bank);
    CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >* composite_surfaces = 0;
    CEngineInternalPrimitiveMorphInfo* morph_info = primitive.GetMorphInfo();
    if (morph_info)
        composite_surfaces = &morph_info->CompositeSurfaces;
    unsigned long mipmap_level = CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap(GetCurrentCamera().PeekCameraPos(), transform, mesh_primitive,
                                                                                    primitive.Scale, material, graphic_bank);
    {
    const CTexture* texture;
    {
        bool preloading = false;
        texture = CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(material, composite_surfaces, mipmap_level, preloading, graphic_bank, false);
        if (preloading) {
            NEngineInternal::GPRenderManager->FlushRender();
            NEngineInternal::GPLightingManager->RestoreDefaultPrimitiveParameters();
            return;
        }
    }
    if (pass == RENDER_PASS_OUTLINE_SMOOTH_STENCIL) {
        FABLE_RETAIL_ASSERT(primitive.MeshEffect.Get());
        SetupOutlineSmoothStencilStates(*static_cast<const CEngineMeshEffectOutline*>(primitive.MeshEffect.Get()), primitive.Scale);
        ApplyVertexShader(TriVertexShaderOutline);
        ApplyPixelShader(GetPixelShaderDiffuseOnly());
    } else if (pass == RENDER_PASS_OUTLINE_SMOOTH_FILL) {
        const CEngineMeshEffectOutline& effect = *static_cast<const CEngineMeshEffectOutline*>(primitive.MeshEffect.Get());
        ApplyVertexShader(TriVertexShaderOutlineSmoothFill);
        if (material->GetDiffuseMapBankIndex() != 0 && material->IsTransparent()) {
            ApplyPixelShader(GetPixelShaderOutlineSmoothFillAlpha());
            SetupOutlineSmoothFillStates(effect, primitive.Scale, true, 1);
        } else {
            ApplyPixelShader(GetPixelShaderOutlineSmoothFill());
            SetupOutlineSmoothFillStates(effect, primitive.Scale, false, 1);
        }
    } else if (pass == RENDER_PASS_OUTLINE_SMOOTH_ALPHA_STENCIL_MASK) {
        const CEngineMeshEffectOutline& effect = *static_cast<const CEngineMeshEffectOutline*>(primitive.MeshEffect.Get());
        ApplyVertexShader(TriVertexShaderOutline);
        SetupOutlineSmoothAlphaStencilMaskStates(effect);
        ApplyPixelShader(GetPixelShaderDiffuseColourTextureDiffuseAlpha());
    } else if (pass == RENDER_PASS_OUTLINE_STENCIL) {
        SetupOutlineStencilStates(*static_cast<const CEngineMeshEffectOutline*>(primitive.MeshEffect.Get()), primitive.Scale);
        ApplyVertexShader(TriVertexShaderOutline);
        ApplyPixelShader(GetPixelShaderDiffuseOnly());
    } else if (pass == RENDER_PASS_OUTLINE_FILL) {
        SetupOutlineFillStates(*static_cast<const CEngineMeshEffectOutline*>(primitive.MeshEffect.Get()), primitive.Scale);
        ApplyVertexShader(TriVertexShaderOutline);
        ApplyPixelShader(GetPixelShaderDiffuseOnly());
    } else {
        if (sub_primitive.PeekRenderFlags() & 0x80)
            SetupStencilMask(primitive.MeshEffect.Get(), sub_primitive.PeekRenderFlags(), pass);
        ApplyTriVertexShader(lights.GetCount(), false, sub_primitive.PeekRenderFlags());
        SetupAlphaBlendingStatesFromRenderFlags(sub_primitive.PeekRenderFlags(), pass, sub_primitive.PeekAlpha(), sub_primitive.PeekParentBase()->AlphaRef);
        NEngineInternal::GPLightingManager->SetSelfIlluminationAmbient(material->GetSelfIllumination());
        NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
        bool tinted = (primitive.TintColour.IntValue & 0xffffff) != 0xffffff;
        if (sub_primitive.PeekRenderFlags() & 0x200) {
            if (sub_primitive.PeekRenderFlags() & 0x10) {
                if (tinted)
                    ApplyPixelShader(GetPixelShaderDiffuseFogShadowOutputFactor());
                else
                    ApplyPixelShader(GetPixelShaderDiffuseFogShadow());
                CEngineShadowRenderer::CColorBufferDepthPackParameters params = { 4, 5, 0 };
                g_pEngineShadowRenderer->SetupShadowFuncRenderStates(1, transform, params);
                NEngineInternal::GPLightingManager->SetupShaderSpotLight(transform);
            } else {
                if (tinted)
                    ApplyPixelShader(GetPixelShaderDiffuseFogShadowOutputFactor());
                else
                    ApplyPixelShader(GetPixelShaderDiffuseFogShadow());
                CEngineShadowRenderer::CColorBufferDepthPackParameters params = { 4, 5, 0 };
                g_pEngineShadowRenderer->SetupShadowFuncRenderStates(1, transform, params);
            }
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
        } else {
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            ApplyPixelShader(GetPixelShaderDiffuse());
        }
    }
    FABLE_RETAIL_ASSERT(texture);
    bool two_sided = material->IsTwoSided();
    if (occlusion_test)
        g_pOcclusionManager->StartOcclusionTest();
    RenderPolygons(simulation, program, transform, lights, &texture, 1, false, two_sided, sub_primitive.PeekRenderFlags(), true, true, 0x109, pass,
                   primitive);
    if (occlusion_test)
        g_pOcclusionManager->EndOcclusionTest(occlusion_handle);
    NEngineInternal::GPRenderManager->FlushRender();
    NEngineInternal::GPLightingManager->RestoreDefaultPrimitiveParameters();
    }
}
