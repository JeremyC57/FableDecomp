// CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitive(CEngineSubPrimitiveStaticMesh& sub_primitive, ERenderPassType pass) -- draw one static mesh primitive inside a render-state capture block: fog / specular on, pick the diffuse mipmap from the camera distance, set the pass's z-write / alpha-test / alpha-blend states, the stencil mask, the shaders and the static blocks' textures, then draw its blocks with an occlusion test around them and reset the device and lighting state
// Grade: DIFFER(4056v4056) -- agentC17: residue A gone (real same-TU bodies of SetupFirstTextures / IndexBuffer / DegenerateTriangles / SetupStaticBlocksBlankTextures / SetupStaticBlocks, all RELOCATION_MATCH at their own addresses in this TU; SetupStaticBlocks needs the GetDiffuseTexture / GetBumpTexture bodies, carried here as the landed NoMorph texts, which DIFFER at their own addresses 382v613 / 373v597: NOT landable as is); residue B (cull bookmark test/or) gone with the repo-default operator- + SHADOWGUARD_DIRECT + PDB global names; remaining: TransformPoint x87 product order 0x00BA5434..0x00BA5440 (2 instructions swapped) = about 1 more 'ruler unit' of IL needed (one dead `const ERenderPassType r = pass;` anywhere matches RELOCATION_MATCH; GetCenter() in GetAppropriateMipmap supplies 2 of the 3 needed). See NOTES.md.
//   9632 bytes, not landed): retail compiled this row with that callee's body visible, so c2 knew &shader_flags / &render_degenerate_triangles
//   are not captured, read them lazily and cached shader_flags across later calls (0x00BA5FEB `mov ebx,[ebp-1Ch]`, 0x00BA60F2 `mov al,[ebp-4]`).
//   With any non-capturing body of it in the TU (PROBE probe/u1_standin_PROBE.cpp, never landable) this exact text grades DIFFER(4056v4056)
//   with ONE residue: the cull SetRenderState bookmark loads swap edx/esi (0x00BA5EB3 `mov edx,[ecx+2814h]` / 0x00BA5EB9 `mov esi,[eax+8]`).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BA5260, 4056 bytes, public thiscall, `ret 8`, ebp frame (sub esp,50h), no EH frame.
// Identity: Ego_r.pdb / FableWin.pdb ?RenderMeshPrimitive@CEnginePrimitiveManagerStaticMeshesBase@@QAEXAAVCEngineSubPrimitiveStaticMesh@@W4ERenderPassType@@@Z;
//   callers RenderSubPrimitive 0x00BA6FA0 (landed) and DrawImmediately 0x00BA6DC0 (landed) plus 0x00BA7807.
// Callees: CRenderStateManager::RestoreCaptureBlock 0x00A05840 (the scoped capture block, StateManager 0x013CB508), CEnginePrimitiveRenderUtil::
//   SetupStencilMask 0x00BF1030, SetupRenderModeShadersAndConstants 0x00BA2350, SetupStaticBlocks 0x00BA50A0, SetupFirstTextures 0x00B9D670,
//   CEnginePrimitiveManagerOcclusionBase::PrepareOcclusionTest 0x00B905A0, CEngineOcclusionManager::StartOcclusionTest 0x00B8D090 /
//   EndOcclusionTest 0x00B8D0A0 (Ego_r names; landed as BeginQuery / EndQuery), CRenderManagerCore::SetWorldTransform 0x00A0B140 /
//   CShaderRenderManager::SetWorldTransform 0x009881F0, CRenderStateManager::RealiseRenderState 0x00A058C0, the PrepareForRender Update*
//   helpers 0x00988A20..0x0098A8E0 and SetVSConstants 0x0098B930, RenderMeshPrimitiveBlocksIndexBuffer 0x00B9E5B0 /
//   RenderMeshPrimitiveBlocksDegenerateTriangles 0x00B9E750 (through the expanded inline RenderMeshPrimitiveBlocks).
// Expanded inlines (FableWin.pdb names, debug sizes reproduced by plain /Od, fwsize SAME): SetupAlphaBlendingStatesFromRenderFlags 71i/268b,
//   GetMagnitude 26i/66b (named squared sum, X Z Y), TransformPoint 61i, GetBoundingSphere 13i, C3DTextureStages() 14i, SetWorldTransform 21i,
//   PrepareForRender 158i, RestoreDefaultPrimitiveParameters 18i, the Set* state setters; GetAppropriateMipmap 75i v FableWin 81i (FITTED text).
// Inline-budget: FABLE_FORCEINLINE on GetAppropriateMipmap, CGraphicDataBank::PeekGraphic, SetupAlphaBlendingStatesFromRenderFlags, FlushRender and
//   SelectVertexBuffer (retail expands all of them in this 4 KB caller; plain inline leaves SetupAlphaBlendingStatesFromRenderFlags and
//   PeekGraphic called, grades 6 / 23). No retail out-of-line copy of any of them.
// Retail-asm: 0x00BA5519..0x00BA5533 `fld [ebp-2Ch] ; fsub [ebp-14h] ; fistp dword ptr [ebp-20h] ; fild [ebp-20h] ; fadd [ebp-1Ch] ; fstp [ebp-30h] ;
//   mov eax,[ebp-2Ch] ; cmp eax,[ebp-30h] ; jne ; inc dword ptr [ebp-20h]` = the shared engine/GFFloatToLongFloor.h helper (the mipmap size
//   floor inside the expanded GetAppropriateMipmap). Per-site attempts table (ASM_RULES 30 + 10): NOT built yet (row parked, see NOTES.md).
// Scopes: the address-taken locals sit in nested blocks (texture stages / shader flags after the mipmap; the render-degenerate flag after the
//   alpha states) and the occlusion handle at function scope: c2's frame packer treats an address-taken local as live over its whole
//   scope, and only this nesting reproduces retail's slot sharing (stages with the subtraction temporary, render-degenerate flag with the
//   alpha-ref byte parameter, shader flags with the mipmap temporaries, the occlusion handle alone). FITTED (packer traces, pk.sh).
// Leaf: ?RenderMeshPrimitive@CEnginePrimitiveManagerStaticMeshesBase@@QAEXAAVCEngineSubPrimitiveStaticMesh@@W4ERenderPassType@@@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS_BLOCKDRAW
#define FABLE_RETAIL_CSHADERRENDERMANAGER_INLINE_BLOCKDRAW
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_BLANKTEXTURES
#define FABLE_RETAIL_C3DPRIMITIVE2_RECORDEDSTATE
#define FABLE_RETAIL_C3DTEXTURESTAGES_EQUALS
#define FABLE_RETAIL_CGRAPHICINFO_TEXTURETYPE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_STATICBLOCKS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_STATICBLOCKS
#define FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
#define FABLE_RETAIL_C3DPRIMITIVE2_RENDER
#define FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_VERTEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_GETVERTEXBUFFER
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CENGINEMESHEFFECT
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_RENDERMESHPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_GETAPPROPRIATEMIPMAP
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_PREPAREOCCLUSIONTEST
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHPRIMITIVE
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS
#define FABLE_RETAIL_CENGINESUBPRIMITIVEMESHBASE_ACCESSORS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DTEXTURESTAGES_CTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_NAMED_SQUARED
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH_PEEKMESH
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH_PEEKMESH_RENDERINFO
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
#define FABLE_RETAIL_CGRAPHICINFO_GETWIDTH
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_CLOTH
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_OCCLUSIONTEST
#define FABLE_RETAIL_C3DPRIMITIVE2_ISDP3BUMP
#define FABLE_RETAIL_C3DPRIMITIVE2_ISDP3BUMP_MASK
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH_PEEKRENDERINFO
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_SETUPRENDERMODE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_REFLECTIONTEXTURE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_2DALPHAMASK
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTHRENDER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTH2D
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETUPBLANKTEXTURES
#define FABLE_RETAIL_CENGINESUBPRIMITIVESTATICMESH_PARENTBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH_PEEKLIGHTS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SPOTLIGHT
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD_DIRECT
#define FABLE_RETAIL_CENGINECAMERA_PEEKWINDOW
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_CLOTH2D
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SETUPRENDERMODE
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR
#define FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX
#define FABLE_RETAIL_C3DMESHINFO_PEEKTEXTUREBANK_MEMBER
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_SHADOWFUNC
#define FABLE_RETAIL_CENGINESHADOWRENDERER_SHADOWFUNC_CTOR
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_DEPTHBUFFERTRANSFORM
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETSTIPLEFADETEXTURE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_ZERORGBATEXTURE
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE2DVIEWPORTMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVE2DVIEWPORTMANAGER_ALPHAMASK
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineCamera.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineOcclusionManager.h"
#include "engine/CEnginePrimitiveManagerStaticMeshesBase.h"
#include "engine/CEngineSubPrimitiveStaticMesh.h"
#include "engine/CEngineInternalPrimitiveMorphedStaticMesh.h"
#include "engine/CGraphicInfo.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DPrimitive2.h"
#include "engine/C3DTextureStages.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFLimit.h"
#include "engine/CEngineMeshEffect.h"
#include "engine/CEngineInternalPrimitiveStaticMesh.h"
#include "engine/C3DMeshMaterial.h"
#include "engine/CTexture.h"
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/C4DVector.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineScreenEffectOutlineGlow.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEnginePrimitive2DViewportManager.h"
#include "engine/C2DAlphaMaskInfo.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/GFDebugPDynamicCast.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }      // 0x01436E14
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                                                // 0x01436EA0
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer;                                // 0x01436E78
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }                              // 0x01436E9C
namespace NEngineInternal { extern CEngineOcclusionManager* GPOcclusionManager; }                                  // 0x01436E5C
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
inline bool CShaderRenderManager::IsFixedFunctionVertexMode() const { return FixedFunctionVertexMode; }
FABLE_FORCEINLINE void CShaderRenderManager::SelectVertexBuffer(CVertexBufferWin32& vertexBuffer)
{
    D3DDevice->SetStreamSource(0, vertexBuffer.PVertexBuffer, 0, vertexBuffer.Stride);
}
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
inline float __fastcall CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier()
{
    return g_pPrimitiveRenderer->MipmapSizeMultiplier;
}
inline void __fastcall CEnginePrimitiveRenderUtil::SetWorldTransform(const CMatrix3x4& transform)
{
    if (NEngineInternal::GPShaderRenderManager->IsFixedFunctionVertexMode())
        NEngineInternal::GPRenderManager->SetWorldTransform(transform);
    else
        NEngineInternal::GPShaderRenderManager->SetWorldTransform(transform);
}
inline CEngineCamera& __fastcall CEnginePrimitiveRenderUtil::GetCurrentCamera()
{
    return *NEngineInternal::GPCamera;
}
FABLE_FORCEINLINE unsigned long FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap(const C3DVector& camera_position,
    const CMatrix3x4& transform, const C3DPrimitive2& mesh_primitive, float scale, const C3DMeshMaterial* material,
    const CGraphicDataBank* graphic_bank)
{
    C3DBoundingSphere sphere = mesh_primitive.GetBoundingSphere();
    transform.TransformPoint(sphere.Center);
    unsigned long mipmap = 0;
    float distance = ((camera_position - sphere.GetCenter()).GetMagnitude() - sphere.GetRadius() * scale) * mesh_primitive.GetAverageTextureStretch();
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
inline void FABLE_FASTCALL CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocks(C3DPrimitive2& mesh_primitive,
    const C3DTextureStages& texture_stages, bool use_push_buffer, bool render_degenerate_triangles)
{
    RenderMeshPrimitiveBlocksIndexBuffer(mesh_primitive, texture_stages);
    if (render_degenerate_triangles)
        RenderMeshPrimitiveBlocksDegenerateTriangles(mesh_primitive, texture_stages);
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
namespace NEngineInternal { extern CEngineScreenEffectOutlineGlow* GPScreenEffectOutlineGlow; } // 0x01436E3C
extern CEngineShadowRenderer* g_pEngineShadowRenderer;                                        // 0x01436E60
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }                                            // 0x01436E98
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;                                  // 0x01436E84

FABLE_FORCEINLINE bool CShaderRenderManager::GetPixelShadersEnabled() const { return PixelShadersEnabled; }
FABLE_FORCEINLINE void CRenderStateManager::SetTextureFactor(unsigned long factor) { SetRenderState(factor, DeviceState.TextureFactor); }
FABLE_FORCEINLINE void __fastcall CEnginePrimitiveRenderUtil::SetTextureFactor(const CRGBFloatColour& colour)
{
    if (NEngineInternal::GPShaderRenderManager->GetPixelShadersEnabled())
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(2, C4DVector(colour));
    else
        NEngineInternal::GPRenderManager->SetTextureFactor(colour.GetRGBColour().IntValue);
}
FABLE_FORCEINLINE void __fastcall CEnginePrimitiveRenderUtil::ApplyPixelShader(const CPixelShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(shader);
}
FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::SetupBlankTextures(C3DTextureStages& texture_stages, bool dp3_bump)
{
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, 0);
    texture_stages.UseBlankTextures = true;
    texture_stages.DiffuseTextureStage = 0;
    if (dp3_bump)
        texture_stages.BumpTextureStage = 1;
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
FABLE_FORCEINLINE const C2DAlphaMaskInfo* CEnginePrimitiveRenderUtil::GetCurrent2DAlphaMaskInfo()
{
    CEnginePrimitive2DViewportManager* manager = GFDebugPDynamicCast(g_pEnginePrimitiveTypeInfo->GetPrimitiveManager(ENGINE_PRIMITIVE_2D_VIEWPORT),
                                                                      (CEnginePrimitive2DViewportManager*)0);
    return manager->GetCurrentAlphaMask();
}
FABLE_FORCEINLINE void CEnginePrimitiveManagerStaticMeshesBase::ApplyVertexShader(const CVertexShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(shader, true);
}
#define FABLE_STATIC_VS_HELPER(name, expr) \
FABLE_FORCEINLINE void CEnginePrimitiveManagerStaticMeshesBase::name(unsigned long light_count) \
{ unsigned long index = light_count; if (index > 5) index = 5; ApplyVertexShader(expr[PointLightShaderIndex[index]]); }
FABLE_STATIC_VS_HELPER(ApplyStaticVertexShader, VertexShadersStatic)
FABLE_STATIC_VS_HELPER(ApplyStaticVertexShaderStippleAlpha, VertexShadersStaticStippleAlpha)
FABLE_STATIC_VS_HELPER(ApplyStaticVertexShaderEnv, VertexShadersStaticEnv)
FABLE_STATIC_VS_HELPER(ApplyStaticVertexShaderShadow, VertexShadersStaticShadow)
FABLE_STATIC_VS_HELPER(ApplyStaticVertexShaderEnvShadow, VertexShadersStaticEnvShadow)
FABLE_STATIC_VS_HELPER(ApplyStaticVertexShaderDP3BumpShadow, VertexShadersStaticDP3BumpShadow)
FABLE_STATIC_VS_HELPER(ApplyStaticVertexShaderSpotLight, VertexShadersStaticSpotlight)
FABLE_STATIC_VS_HELPER(ApplyStaticVertexShaderSpotShadow, VertexShadersStaticSpotShadow)
FABLE_STATIC_VS_HELPER(ApplyStaticVertexShaderEnvSpotShadow, VertexShadersStaticEnvSpotShadow)
FABLE_FORCEINLINE void CEnginePrimitiveManagerStaticMeshesBase::Apply2DStaticVertexShader() { ApplyVertexShader(VertexShaders2DStatic); }
FABLE_FORCEINLINE void CEnginePrimitiveManagerStaticMeshesBase::Apply2DStaticVertexShaderEnv() { ApplyVertexShader(VertexShaders2DStaticEnv); }
FABLE_FORCEINLINE void CEnginePrimitiveManagerStaticMeshesBase::Apply2DStaticVertexShaderDP3Bump() { ApplyVertexShader(VertexShaders2DStaticDP3Bump); }

void FABLE_FASTCALL CEnginePrimitiveManagerStaticMeshesBase::SetupFirstTextures(C3DPrimitive2& primitive, C3DTextureStages& textureStages)
{
    N3DPrimitive::CStaticBlock* block = primitive.StaticBlocks;
    if (textureStages.DiffuseTextureStage != -1)
        NEngineInternal::GPRenderManager->AttachTextureToStage(textureStages.DiffuseTextureStage, block->DiffuseTexture);
    if (textureStages.BumpTextureStage != -1)
        NEngineInternal::GPRenderManager->AttachTextureToStage(textureStages.BumpTextureStage, block->BumpTexture);
}
void FABLE_FASTCALL CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksIndexBuffer(C3DPrimitive2& mesh_primitive, const C3DTextureStages& texture_stages)
{
    unsigned long no_blocks = mesh_primitive.GetStaticBlockCount();
    for (unsigned long c_block = 0; c_block < no_blocks; ++c_block) {
        N3DPrimitive::CStaticBlock& block_info = *mesh_primitive.GetStaticBlock(c_block);
        if (!block_info.GetDegenerateTriangles()) {
            if (block_info.GetChangeFlags() & 1)
                NEngineInternal::GPRenderManager->AttachTextureToStage(texture_stages.DiffuseTextureStage, block_info.DiffuseTexture);
            if ((block_info.GetChangeFlags() & 2) && texture_stages.BumpTextureStage != -1)
                NEngineInternal::GPRenderManager->AttachTextureToStage(texture_stages.BumpTextureStage, block_info.BumpTexture);
            NEngineInternal::GPShaderRenderManager->DrawBuffersIndexed(0, mesh_primitive.GetVertexCount(), *mesh_primitive.IndexBuffer,
                block_info.GetStartIndex(), block_info.GetPrimitiveCount(), block_info.GetPrimitiveType());
        }
    }
}

void FABLE_FASTCALL CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksDegenerateTriangles(C3DPrimitive2& primitive,
                                                                                                      const C3DTextureStages& textureStages)
{
    if (textureStages.DiffuseTextureStage != -1)
        NEngineInternal::GPRenderManager->AttachTextureToStage(textureStages.DiffuseTextureStage, 0);
    if (textureStages.BumpTextureStage != -1)
        NEngineInternal::GPRenderManager->AttachTextureToStage(textureStages.BumpTextureStage, 0);
    const unsigned long blockCount = primitive.StaticBlockCount;
    for (unsigned long i = 0; i < blockCount; ++i) {
        N3DPrimitive::CStaticBlock& block = primitive.StaticBlocks[i];
        if (block.GetDegenerateTriangles()) {
            const unsigned long primitiveCount = block.GetPrimitiveCount();
            const unsigned long startIndex = block.GetStartIndex();
            const unsigned long vertexCount = primitive.VertexCount;
            CIndexBuffer& indexBuffer = *primitive.IndexBuffer;
            NEngineInternal::GPShaderRenderManager->DrawBuffersIndexed(0, vertexCount, indexBuffer, startIndex, primitiveCount,
                                                                       block.GetPrimitiveType());
        }
    }
}

bool CEnginePrimitiveManagerStaticMeshesBase::SetupRenderModeShadersAndConstants(ERenderPassType pass, const C3DMeshMaterial* material,
                                                                                 CEngineSubPrimitiveStaticMesh& sub_primitive,
                                                                                 C3DTextureStages& texture_stages, long mipmap_level,
                                                                                 unsigned long* shader_flags, bool* render_degenerate_triangles)
{
    CEngineInternalPrimitiveStaticMesh* primitive = sub_primitive.GetParent();
    const CGraphicDataBank* graphic_bank = primitive->MeshInfo->PeekTextureBank();
    bool block_on_load = (primitive->EnginePrimitiveFlags >> 7) & 1;
    CRGBColour tint_colour = primitive->TintColour;
    bool tinted = (tint_colour.IntValue & 0xffffff) != 0xffffff;
    C3DPrimitive2* mesh_primitive = sub_primitive.MeshPrimitive;
    unsigned long flags = 0x109;
    if (pass == RENDER_PASS_COMBINED) {
        long render_mode = sub_primitive.GetRenderMode();
        unsigned char alpha = sub_primitive.PeekAlpha();
        if (sub_primitive.PeekRenderFlags() & 8) {
            render_mode = 5;
            if (!material->Transparent) {
                long alpha_value = alpha;
                long solid_alpha = StippleAlphaSolidFactor * alpha_value;
                if (solid_alpha >= 255)
                    alpha = 255;
                else
                    alpha = (unsigned char)solid_alpha;
            }
        }
        tint_colour.A = alpha;
        CEnginePrimitiveRenderUtil::SetTextureFactor(CRGBFloatColour(tint_colour));
        NEngineInternal::GPLightingManager->SetCurrentEnvironmentThemeChannel(primitive->ThemeChannelDesc);
        const CEngineLocalLightArray& lights = sub_primitive.GetParent()->PeekLights();
        switch (render_mode) {
        case 2:
            texture_stages.DiffuseTextureStage = 0;
            ApplyStaticVertexShader(lights.GetCount());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShaderDiffuse);
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            break;
        case 3:
            texture_stages.DiffuseTextureStage = 0;
            ApplyStaticVertexShaderSpotLight(lights.GetCount());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShaderDiffuse);
            NEngineInternal::GPLightingManager->SetupShaderSpotLight(primitive->ObjectTransform);
            break;
        case 4:
            texture_stages.DiffuseTextureStage = 0;
            ApplyStaticVertexShader(lights.GetCount());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShaderDiffuseFog);
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            break;
        case 5: {
            texture_stages.DiffuseTextureStage = 0;
            ApplyStaticVertexShaderStippleAlpha(lights.GetCount());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(PixelShaderDiffuseFogStippleAlpha);
            const C2DBoxF& window = NEngineInternal::GPCamera->PeekWindow();
            float left = window.TLX;
            float top = window.TLY;
            float right = window.BRX;
            float bottom = window.BRY;
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, (right - left) * 0.015625f, (bottom - top) * -0.015625f,
                                                                                (right + left) * 0.015625f + 0.015625,
                                                                                (bottom + top) * 0.015625f + 0.015625);
            NEngineInternal::GPRenderManager->SetTextureFilterModeMin(1, TEXTURE_FILTER_POINT);
            NEngineInternal::GPRenderManager->SetTextureFilterModeMag(1, TEXTURE_FILTER_POINT);
            NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_WRAP);
            NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_WRAP);
            NEngineInternal::GPRenderManager->AttachTextureToStage(1, &NEngineInternal::GPResourceManager->GetStipleFadeTexture());
            NEngineInternal::GPRenderManager->SetTextureTransformMode(1, TEXTURE_TRANSFORM_PROJECTED);
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            break;
        }
        case 6: {
            texture_stages.DiffuseTextureStage = 0;
            NEngineInternal::GPRenderManager->AttachTextureToStage(1, NEngineInternal::GPResourceManager->GetZeroRGBATexture());
            bool used_blank;
            NEngineInternal::GPRenderManager->AttachTextureToStage(2, GetReflectionTexture(material, mipmap_level, used_blank, graphic_bank, block_on_load));
            flags = 0x129;
            ApplyStaticVertexShaderEnv(lights.GetCount());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShaderDiffuseEnvFog);
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            break;
        }
        case 7: {
            texture_stages.DiffuseTextureStage = 0;
            bool used_blank;
            NEngineInternal::GPRenderManager->AttachTextureToStage(1, GetReflectionTexture(material, mipmap_level, used_blank, graphic_bank, block_on_load));
            const CRGBFloatColour& reflection = NEngineInternal::GPLightingManager->ThemeChannels[NEngineInternal::GPLightingManager->CurrentThemeChannel.ChannelIndex0].ReflectionColour;
            NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(0, C4DVector(reflection));
            flags = 0x129;
            ApplyStaticVertexShaderEnv(lights.GetCount());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShaderDiffuseEnvFog);
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            break;
        }
        case 8:
            texture_stages.DiffuseTextureStage = 0;
            texture_stages.BumpTextureStage = 1;
            ApplyStaticVertexShader(lights.GetCount());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShaderDiffuseFog);
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            break;
        case 9:
            texture_stages.DiffuseTextureStage = 0;
            ApplyStaticVertexShaderShadow(lights.GetCount());
            if (tinted)
                CEnginePrimitiveRenderUtil::ApplyPixelShader(*g_pPrimitiveRenderer->PixelShaderDiffuseFogShadowOutputFactor);
            else
                CEnginePrimitiveRenderUtil::ApplyPixelShader(*g_pPrimitiveRenderer->PixelShaderDiffuseFogShadow);
            g_pEngineShadowRenderer->SetupShadowFuncRenderStates(1, primitive->ObjectTransform, CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            flags = 0x10b;
            break;
        case 10: {
            texture_stages.DiffuseTextureStage = 0;
            bool used_blank;
            NEngineInternal::GPRenderManager->AttachTextureToStage(1, GetReflectionTexture(material, mipmap_level, used_blank, graphic_bank, block_on_load));
            const CRGBFloatColour& reflection = NEngineInternal::GPLightingManager->ThemeChannels[NEngineInternal::GPLightingManager->CurrentThemeChannel.ChannelIndex0].ReflectionColour;
            NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(0, C4DVector(reflection));
            flags = 0x12b;
            ApplyStaticVertexShaderEnvShadow(lights.GetCount());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(*g_pPrimitiveRenderer->PixelShaderDiffuseEnvFogShadow);
            g_pEngineShadowRenderer->SetupShadowFuncRenderStates(2, primitive->ObjectTransform, CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            break;
        }
        case 11:
            texture_stages.DiffuseTextureStage = 0;
            texture_stages.BumpTextureStage = 1;
            NEngineInternal::GPRenderManager->AttachTextureToStage(2, NEngineInternal::GPResourceManager->GetZeroRGBATexture());
            ApplyStaticVertexShaderDP3BumpShadow(lights.GetCount());
            if (tinted)
                CEnginePrimitiveRenderUtil::ApplyPixelShader(*g_pPrimitiveRenderer->PixelShaderDiffuseDP3BumpFogShadowOutputFactor);
            else
                CEnginePrimitiveRenderUtil::ApplyPixelShader(*g_pPrimitiveRenderer->PixelShaderDiffuseDP3BumpFogShadow);
            flags = 0x10b;
            g_pEngineShadowRenderer->SetupShadowFuncRenderStates(3, primitive->ObjectTransform, CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            break;
        case 12:
            texture_stages.DiffuseTextureStage = 0;
            ApplyStaticVertexShaderSpotShadow(lights.GetCount());
            if (tinted)
                CEnginePrimitiveRenderUtil::ApplyPixelShader(*g_pPrimitiveRenderer->PixelShaderDiffuseFogShadowOutputFactor);
            else
                CEnginePrimitiveRenderUtil::ApplyPixelShader(*g_pPrimitiveRenderer->PixelShaderDiffuseFogShadow);
            g_pEngineShadowRenderer->SetupShadowFuncRenderStates(1, primitive->ObjectTransform, CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            NEngineInternal::GPLightingManager->SetupShaderSpotLight(primitive->ObjectTransform);
            flags = 0x10b;
            break;
        case 13: {
            texture_stages.DiffuseTextureStage = 0;
            bool used_blank;
            NEngineInternal::GPRenderManager->AttachTextureToStage(1, GetReflectionTexture(material, mipmap_level, used_blank, graphic_bank, block_on_load));
            const CRGBFloatColour& reflection = NEngineInternal::GPLightingManager->ThemeChannels[NEngineInternal::GPLightingManager->CurrentThemeChannel.ChannelIndex0].ReflectionColour;
            NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(0, C4DVector(reflection));
            flags = 0x12b;
            ApplyStaticVertexShaderEnvSpotShadow(lights.GetCount());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(*g_pPrimitiveRenderer->PixelShaderDiffuseEnvFogShadow);
            g_pEngineShadowRenderer->SetupShadowFuncRenderStates(2, primitive->ObjectTransform, CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            NEngineInternal::GPLightingManager->SetupShaderSpotLight(primitive->ObjectTransform);
            break;
        }
        case 1:
            ApplyStaticVertexShader(0);
            CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShaderDiffuseOnly);
            break;
        case 16: {
            texture_stages.DiffuseTextureStage = 0;
            const C2DAlphaMaskInfo* mask = GetCurrent2DAlphaMaskInfo();
            bool used_blank;
            NEngineInternal::GPRenderManager->AttachTextureToStage(1, Get2DAlphaMaskTexture(mask->AlphaMaskBankIndex, 0, used_blank, mask->AlphaMaskBank, block_on_load));
            Set2DAlphaMaskTextureMode(1, mask->FilterMask, true);
            Apply2DStaticVertexShader();
            CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShader2DDiffuse);
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, C3DVector(mask->AlphaMaskPositionFactors.X, mask->AlphaMaskPositionFactors.Y,
                                                                                             mask->AlphaMaskPositionFactors.Z), mask->AlphaMaskPositionFactors.W);
            NEngineInternal::GPRenderManager->SetFogEnable(false);
            break;
        }
        case 17: {
            texture_stages.DiffuseTextureStage = 0;
            const C2DAlphaMaskInfo* mask = GetCurrent2DAlphaMaskInfo();
            bool used_blank;
            NEngineInternal::GPRenderManager->AttachTextureToStage(1, Get2DAlphaMaskTexture(mask->AlphaMaskBankIndex, 0, used_blank, mask->AlphaMaskBank, block_on_load));
            NEngineInternal::GPRenderManager->AttachTextureToStage(2, GetReflectionTexture(material, mipmap_level, used_blank, graphic_bank, block_on_load));
            Set2DAlphaMaskTextureMode(1, mask->FilterMask, true);
            const CRGBFloatColour& reflection = NEngineInternal::GPLightingManager->ThemeChannels[NEngineInternal::GPLightingManager->CurrentThemeChannel.ChannelIndex0].ReflectionColour;
            NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(0, C4DVector(reflection));
            flags = 0x129;
            Apply2DStaticVertexShaderEnv();
            CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShader2DDiffuseEnv);
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, C3DVector(mask->AlphaMaskPositionFactors.X, mask->AlphaMaskPositionFactors.Y,
                                                                                             mask->AlphaMaskPositionFactors.Z), mask->AlphaMaskPositionFactors.W);
            NEngineInternal::GPRenderManager->SetFogEnable(false);
            break;
        }
        case 18: {
            texture_stages.DiffuseTextureStage = 0;
            texture_stages.BumpTextureStage = 1;
            const C2DAlphaMaskInfo* mask = GetCurrent2DAlphaMaskInfo();
            bool used_blank;
            const CTexture* mask_texture = Get2DAlphaMaskTexture(mask->AlphaMaskBankIndex, 0, used_blank, mask->AlphaMaskBank, block_on_load);
            NEngineInternal::GPRenderManager->AttachTextureToStage(2, mask_texture);
            NEngineInternal::GPRenderManager->AttachTextureToStage(3, mask_texture);
            Set2DAlphaMaskTextureMode(2, mask->FilterMask, true);
            Apply2DStaticVertexShaderDP3Bump();
            CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShader2DDiffuseDP3Bump);
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, C3DVector(mask->AlphaMaskPositionFactors.X, mask->AlphaMaskPositionFactors.Y,
                                                                                             mask->AlphaMaskPositionFactors.Z), mask->AlphaMaskPositionFactors.W);
            NEngineInternal::GPRenderManager->SetFogEnable(false);
            break;
        }
        case 15: {
            const C2DAlphaMaskInfo* mask = GetCurrent2DAlphaMaskInfo();
            bool used_blank;
            NEngineInternal::GPRenderManager->AttachTextureToStage(0, Get2DAlphaMaskTexture(mask->AlphaMaskBankIndex, 0, used_blank, mask->AlphaMaskBank, block_on_load));
            Set2DAlphaMaskTextureMode(0, mask->FilterMask, true);
            Apply2DStaticVertexShader();
            CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShader2DDiffuseOnly);
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, C3DVector(mask->AlphaMaskPositionFactors.X, mask->AlphaMaskPositionFactors.Y,
                                                                                             mask->AlphaMaskPositionFactors.Z), mask->AlphaMaskPositionFactors.W);
            NEngineInternal::GPRenderManager->SetFogEnable(false);
            break;
        }
        default:
            __assume(0);
        }
        NEngineInternal::GPLightingManager->SetupShaderPointLights(lights);
        NEngineInternal::GPLightingManager->SetSelfIlluminationAmbient(material->SelfIllumination);
        NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
    } else if (pass == RENDER_PASS_Z_WRITE) {
        CEnginePrimitiveRenderUtil::SetTextureFactor(CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f));
        if (material->Transparent) {
            texture_stages.DiffuseTextureStage = 0;
            if (mesh_primitive->IsDP3Bump())
                texture_stages.BumpTextureStage = 1;
        } else {
            SetupBlankTextures(texture_stages, mesh_primitive->IsDP3Bump());
        }
        ApplyStaticVertexShader(0);
        CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShaderDiffuseFog);
    } else if (pass == RENDER_PASS_OUTLINE_SMOOTH_STENCIL) {
        SetupOutlineSmoothStencilStates(*static_cast<const CEngineMeshEffectOutline*>(primitive->MeshEffect.Get()), primitive->Scale);
        SetupBlankTextures(texture_stages, mesh_primitive->IsDP3Bump());
        ApplyVertexShader(mesh_primitive->IsDP3Bump() ? VertexShaderDP3BumpOutline : VertexShaderOutline);
        CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShaderDiffuseOnly);
        *render_degenerate_triangles = true;
    } else if (pass == RENDER_PASS_OUTLINE_SMOOTH_FILL) {
        const CEngineMeshEffectOutline* effect = static_cast<const CEngineMeshEffectOutline*>(primitive->MeshEffect.Get());
        ApplyVertexShader(mesh_primitive->IsDP3Bump() ? VertexShaderDP3BumpOutlineSmoothFill : VertexShaderOutlineSmoothFill);
        *render_degenerate_triangles = true;
        NEngineInternal::GPScreenEffectOutlineGlow->SetupDepthBufferTransform(primitive->ObjectTransform);
        bool dp3_bump = mesh_primitive->IsDP3Bump();
        long stage_count = dp3_bump ? 2 : 1;
        if (material->DiffuseMapBankIndex != 0 && material->Transparent) {
            texture_stages.DiffuseTextureStage = 0;
            if (mesh_primitive->IsDP3Bump()) {
                texture_stages.BumpTextureStage = 1;
                CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShaderOutlineSmoothFillDP3Alpha);
            } else
                CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShaderOutlineSmoothFillAlpha);
            SetupOutlineSmoothFillStates(*effect, primitive->Scale, true, stage_count);
        } else {
            SetupBlankTextures(texture_stages, dp3_bump);
            if (mesh_primitive->IsDP3Bump())
                CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShaderOutlineSmoothFillDP3);
            else
                CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShaderOutlineSmoothFill);
            SetupOutlineSmoothFillStates(*effect, primitive->Scale, false, stage_count);
        }
    } else if (pass == RENDER_PASS_OUTLINE_SMOOTH_ALPHA_STENCIL_MASK) {
        const CEngineMeshEffectOutline* effect = static_cast<const CEngineMeshEffectOutline*>(primitive->MeshEffect.Get());
        ApplyVertexShader(mesh_primitive->IsDP3Bump() ? VertexShaderDP3BumpOutline : VertexShaderOutline);
        *render_degenerate_triangles = true;
        if (material->DiffuseMapBankIndex != 0 && material->Transparent) {
            texture_stages.DiffuseTextureStage = 0;
            if (mesh_primitive->IsDP3Bump())
                texture_stages.BumpTextureStage = 1;
            CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShaderDiffuseColourTextureDiffuseAlpha);
            SetupOutlineSmoothAlphaStencilMaskStates(*effect);
        } else {
            SetupBlankTextures(texture_stages, mesh_primitive->IsDP3Bump());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShaderDiffuseOnly);
            SetupOutlineSmoothAlphaStencilMaskStates(*effect);
        }
    } else {
        const CEngineMeshEffectOutline* effect = static_cast<const CEngineMeshEffectOutline*>(primitive->MeshEffect.Get());
        if (pass == RENDER_PASS_OUTLINE_STENCIL)
            SetupOutlineStencilStates(*effect, primitive->Scale);
        else
            SetupOutlineFillStates(*effect, primitive->Scale);
        SetupBlankTextures(texture_stages, mesh_primitive->IsDP3Bump());
        ApplyVertexShader(mesh_primitive->IsDP3Bump() ? VertexShaderDP3BumpOutline : VertexShaderOutline);
        CEnginePrimitiveRenderUtil::ApplyPixelShader(g_pPrimitiveRenderer->PixelShaderDiffuseOnly);
        *render_degenerate_triangles = true;
    }
    if (texture_stages.DiffuseTextureStage >= 0) {
        NEngineInternal::GPRenderManager->SetTextureUVModeU(texture_stages.DiffuseTextureStage, TEXTURE_UV_WRAP);
        NEngineInternal::GPRenderManager->SetTextureUVModeV(texture_stages.DiffuseTextureStage, TEXTURE_UV_WRAP);
    }
    if (texture_stages.BumpTextureStage >= 0) {
        NEngineInternal::GPRenderManager->SetTextureUVModeU(texture_stages.BumpTextureStage, TEXTURE_UV_WRAP);
        NEngineInternal::GPRenderManager->SetTextureUVModeV(texture_stages.BumpTextureStage, TEXTURE_UV_WRAP);
    }
    *shader_flags = flags;
    return true;
}


FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic,unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}
bool CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksBlankTextures(C3DPrimitive2& mesh_prim, CEngineInternalPrimitiveStaticMesh& prim,
    C3DTextureStages& stages, const C3DMeshMaterial* material, bool* already_set_up)
{
    const CGraphicDataBank* graphic_bank=prim.MeshInfo->PeekTextureBank();
    long frame=graphic_bank->GetCurrentFrame();
    if(frame==mesh_prim.GetRecordedFrame() && stages==mesh_prim.PeekRecordedTextureStages()) {
        *already_set_up=true;
        return true;
    }
    *already_set_up=false;
    N3DPrimitive::CStaticBlock& first_block=*mesh_prim.GetStaticBlock(0);
    if(stages.DiffuseTextureStage!=-1) {
        CGraphicDataBank::ETextureType texture_type=CGraphicDataBank::TEXTURE_TYPE_OPAQUE;
        unsigned long graphic_index=material->GetDiffuseMapBankIndex();
        if(graphic_index!=0) texture_type=graphic_bank->PeekGraphicInfoFrame(graphic_index,0)->GetTextureType();
        first_block.DiffuseTexture=graphic_bank->GetBlankTexture(texture_type);
    }
    if(stages.BumpTextureStage!=-1) {
        CGraphicDataBank::ETextureType texture_type=CGraphicDataBank::TEXTURE_TYPE_BUMP;
        unsigned long graphic_index=material->GetBumpMapBankIndex();
        if(graphic_index!=0) texture_type=graphic_bank->PeekGraphicInfoFrame(graphic_index,0)->GetTextureType();
        first_block.BumpTexture=graphic_bank->GetBlankTexture(texture_type);
    }
    unsigned long no_blocks=mesh_prim.GetStaticBlockCount();
    for(unsigned long c_block=1;c_block<no_blocks;++c_block) {
        N3DPrimitive::CStaticBlock& block_info=*mesh_prim.GetStaticBlock(c_block);
        block_info.DiffuseTexture=first_block.DiffuseTexture;
        block_info.BumpTexture=first_block.BumpTexture;
    }
    mesh_prim.SetRecordedFrame(frame);
    mesh_prim.SetRecordedTextureStages(stages);
    return true;
}
inline void CManagedTexture::TouchTexture()
{
    if(!Data.IsNull() && Data->ResourceList && Data->LastUsedFrame!=Data->ResourceList->GetCurrentFrame()) {
        Data->SetAsMRU();
        Data->LastUsedFrame=Data->ResourceList->GetCurrentFrame();
    }
}
inline void CGraphicInfoFrame::TouchFrame(unsigned long mipmap_level) const
{
    const unsigned long frame=GraphicInfo->GetBank()->GetCurrentFrame();
    if(Data->TouchedFrame!=frame || mipmap_level<=Data->TouchedMipmapLevel) {
        Data->TouchedFrame=frame;
        Data->ManagedTexture.TouchTexture();
        if(mipmap_level<=Data->CurrentMipmapLevel && Data->ResourceList && Data->LastUsedFrame!=Data->ResourceList->GetCurrentFrame()) {
            Data->SetAsMRU();
            Data->LastUsedFrame=Data->ResourceList->GetCurrentFrame();
        }
        if(Data->TouchedMipmapLevel>mipmap_level)
            Data->TouchedMipmapLevel=(signed char)mipmap_level;
    }
}
FABLE_FORCEINLINE bool CGraphicInfoFrame::GetTexture(const CTexture** texture,unsigned long mipmap_level,unsigned long flags) const
{
    if(!Data.IsNull() && Data->Frame.FrameData) {
        if(!(flags&8)) TouchFrame(mipmap_level);
        if(mipmap_level<Data->CurrentMipmapLevel) {
            Data->BeginLoading(mipmap_level,flags);
            if((flags&16) && Data->FileData.Data) goto load_frame;
        }
        *texture=Data->Frame.FrameData->GetTexture();
        return true;
    }
load_frame:
    if(Data.IsNull()) {
        if(flags&4) return false;
        Data.Reset(new CFrameData(this));
    }
    Data->BeginLoading(mipmap_level,flags);
    if((flags&1) && InternalGetFrameForcedLoading(flags)) {
        *texture=Data->Frame.FrameData->GetTexture();
        return true;
    }
    return false;
}

const CTexture* FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(const C3DMeshMaterial* material,
    CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >* composite_surfaces,
    unsigned long mipmap_level,bool& failed,const CGraphicDataBank* graphic_bank,const bool block_on_load)
{
    long graphic_index=material->GetDiffuseMapBankIndex();
    if(graphic_index==0) return graphic_bank->GetBlankTexture(CGraphicDataBank::TEXTURE_TYPE_OPAQUE);
    if(composite_surfaces) {
        const CTexture* texture=GetComposedTexture(graphic_index,*composite_surfaces,mipmap_level,false,graphic_bank);
        if(texture) return texture;
    }
    const CGraphicInfoFrame* frame_info=graphic_bank->PeekGraphicInfoFrame(graphic_index,0);
    unsigned long get_flags=0;
    if(g_pPrimitiveRenderer->GetBlockOnLoad() || block_on_load) get_flags=17;
    if(frame_info) {
        const CTexture* texture;
        if(frame_info->GetTexture(&texture,mipmap_level,get_flags)) return texture;
    }
    failed=true;
    return 0;
}
const CTexture* FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetBumpTexture(const C3DMeshMaterial* material,
    CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >* composite_surfaces,
    unsigned long mipmap_level,bool& failed,const CGraphicDataBank* graphic_bank,const bool block_on_load)
{
    long graphic_index=material->GetBumpMapBankIndex();
    if(graphic_index==0) return graphic_bank->GetBlankTexture(CGraphicDataBank::TEXTURE_TYPE_BUMP);
    if(composite_surfaces) {
        const CTexture* texture=GetComposedTexture(graphic_index,*composite_surfaces,mipmap_level,true,graphic_bank);
        if(texture) return texture;
    }
    const CGraphicInfoFrame* frame_info=graphic_bank->PeekGraphicInfoFrame(graphic_index,0);
    unsigned long get_flags=0;
    if(g_pPrimitiveRenderer->GetBlockOnLoad() || block_on_load) get_flags=17;
    const CTexture* texture;
    if(frame_info->GetTexture(&texture,mipmap_level,get_flags)) return texture;
    return frame_info->GetBlankTexture();
}

bool CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocks(C3DPrimitive2& mesh_prim, CEngineInternalPrimitiveStaticMesh& prim,
    C3DTextureStages& stages, const C3DMeshMaterial* material,
    CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >* composite_surfaces,
    long mipmap_level, bool* already_set_up)
{
    if(stages.UseBlankTextures) {
        SetupStaticBlocksBlankTextures(mesh_prim,prim,stages,material,already_set_up);
        return true;
    }
    bool failed=false;
    const CGraphicDataBank* graphic_bank=prim.MeshInfo->PeekTextureBank();
    const C3DMesh2& mesh=prim.PeekMesh();
    const bool block_on_load=(prim.EnginePrimitiveFlags>>7)&1;
    if(composite_surfaces==0 && graphic_bank->GetCurrentFrame()==mesh_prim.GetRecordedFrame() && stages==mesh_prim.PeekRecordedTextureStages()) {
        *already_set_up=true;
        return true;
    }
    *already_set_up=false;
    N3DPrimitive::CStaticBlock& first_block=*mesh_prim.GetStaticBlock(0);
    if(stages.DiffuseTextureStage!=-1)
        first_block.DiffuseTexture=CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(material,composite_surfaces,mipmap_level,failed,graphic_bank,block_on_load);
    if(stages.BumpTextureStage!=-1)
        first_block.BumpTexture=CEnginePrimitiveManagerMeshBase::GetBumpTexture(material,composite_surfaces,mipmap_level,failed,graphic_bank,block_on_load);
    unsigned long no_blocks=mesh_prim.GetStaticBlockCount();
    for(unsigned long c_block=1;c_block<no_blocks;++c_block) {
        N3DPrimitive::CStaticBlock& block_info=*mesh_prim.GetStaticBlock(c_block);
        const C3DMeshMaterial* current_material=mesh.GetMaterial(block_info.GetMaterialIndex());
        if(block_info.GetChangeFlags()&1) {
            if(stages.DiffuseTextureStage!=-1)
                block_info.DiffuseTexture=CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(current_material,composite_surfaces,mipmap_level,failed,graphic_bank,block_on_load);
            else failed=true;
        }
        if((block_info.GetChangeFlags()&2)&&stages.BumpTextureStage!=-1)
            block_info.BumpTexture=CEnginePrimitiveManagerMeshBase::GetBumpTexture(current_material,composite_surfaces,mipmap_level,failed,graphic_bank,block_on_load);
    }
    return !failed;
}

void CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitive(CEngineSubPrimitiveStaticMesh& sub_primitive, ERenderPassType pass)
{
    CRenderStateScopedCaptureBlock capture_block;
    unsigned long occlusion_handle;
    NEngineInternal::GPRenderManager->SetSpecularEnable(true);
    NEngineInternal::GPRenderManager->SetFogEnable(true);
    CEngineInternalPrimitiveStaticMesh* primitive = sub_primitive.GetParent();
    C3DPrimitive2* mesh_primitive = sub_primitive.MeshPrimitive;
    const C3DMesh2& mesh = primitive->PeekMesh();
    const C3DMeshMaterial* material = mesh.GetMaterial(mesh_primitive->GetMaterialIndex());
    CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >* composite_surfaces = 0;
    if (primitive->GetType() == 10)
        composite_surfaces = &static_cast<CEngineInternalPrimitiveMorphedStaticMesh*>(primitive)->Morph.CompositeSurfaces;
    const CGraphicDataBank* graphic_bank = primitive->MeshInfo->PeekTextureBank();
    if (material->GetDiffuseMapBankIndex() == 0)
        return;
    unsigned long mipmap_level = CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap(GetCurrentCamera().PeekCameraPos(), sub_primitive.PeekObjectTransform(),
                                                                              *mesh_primitive, sub_primitive.GetScale(), material, graphic_bank);
    {
        C3DTextureStages texture_stages;
        unsigned long shader_flags = 0;
        unsigned long render_flags = sub_primitive.PeekRenderFlags();
        SetupAlphaBlendingStatesFromRenderFlags(render_flags, pass, sub_primitive.PeekAlpha(), sub_primitive.GetParent()->AlphaRef);
        if (render_flags & 0x80)
            SetupStencilMask(primitive->MeshEffect.Get(), render_flags, pass);
        bool render_degenerate_triangles = false;
        if (!SetupRenderModeShadersAndConstants(pass, material, sub_primitive, texture_stages, mipmap_level, &shader_flags,
                                                &render_degenerate_triangles)) {
            NEngineInternal::GPLightingManager->RestoreDefaultPrimitiveParameters();
            NEngineInternal::GPRenderManager->FlushRender();
            return;
        }
        bool used_blank_textures;
        if (!SetupStaticBlocks(*mesh_primitive, *primitive, texture_stages, material, composite_surfaces, mipmap_level, &used_blank_textures)) {
            NEngineInternal::GPLightingManager->RestoreDefaultPrimitiveParameters();
            NEngineInternal::GPRenderManager->FlushRender();
            return;
        }
        SetupFirstTextures(*mesh_primitive, texture_stages);
        NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_CCW);
        PrepareOcclusionTest(*primitive, pass, occlusion_handle);
        if (occlusion_handle != (unsigned long)-1)
            NEngineInternal::GPOcclusionManager->StartOcclusionTest();
        NEngineInternal::GPShaderRenderManager->SelectVertexBuffer(*mesh_primitive->GetVertexBuffer());
        CEnginePrimitiveRenderUtil::SetWorldTransform(sub_primitive.PeekObjectTransform());
        NEngineInternal::GPRenderManager->RealiseRenderState();
        NEngineInternal::GPShaderRenderManager->PrepareForRender(shader_flags);
        RenderMeshPrimitiveBlocks(*mesh_primitive, texture_stages, false, render_degenerate_triangles);
        if (occlusion_handle != (unsigned long)-1)
            NEngineInternal::GPOcclusionManager->EndOcclusionTest(occlusion_handle);
        NEngineInternal::GPRenderManager->FlushRender();
        NEngineInternal::GPLightingManager->RestoreDefaultPrimitiveParameters();
    }
}
