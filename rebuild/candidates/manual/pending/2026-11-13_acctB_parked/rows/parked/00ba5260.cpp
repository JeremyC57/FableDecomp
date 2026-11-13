// CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitive(CEngineSubPrimitiveStaticMesh& sub_primitive, ERenderPassType pass) -- draw one static mesh primitive inside a render-state capture block: fog / specular on, pick the diffuse mipmap from the camera distance, set the pass's z-write / alpha-test / alpha-blend states, the stencil mask, the shaders and the static blocks' textures, then draw its blocks with an occlusion test around them and reset the device and lighting state
// Grade: DIFFER(4060v4056) standalone -- BLOCKED on SetupRenderModeShadersAndConstants 0x00BA2350 (same retail TU, defined earlier,
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
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
#define FABLE_RETAIL_C3DPRIMITIVE2_RENDER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CENGINEMESHEFFECT
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_RENDERMESHPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_GETAPPROPRIATEMIPMAP
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_PREPAREOCCLUSIONTEST
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHRENDER
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS
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
#define FABLE_RETAIL_C3DVECTOR_NAMED_RESULT_SUB
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_NAMED_SQUARED
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH_PEEKMESH
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
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MIPMAP
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_OCCLUSIONTEST
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

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }      // 0x01436E14
extern CEngineCamera* g_pEngineCamera;                                                // 0x01436EA0
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer;                                // 0x01436E78
extern CEngineLightingManager* g_pEngineLightingManager;                              // 0x01436E9C
extern CEngineOcclusionManager* g_pOcclusionManager;                                  // 0x01436E5C
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
    SelectVertexBuffer(0, 0);
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
    unsigned long mipmap_level = CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap(g_pEngineCamera->PeekCameraPos(), sub_primitive.PeekObjectTransform(),
                                                                              *mesh_primitive, sub_primitive.GetScale(), material, graphic_bank);
    {
        C3DTextureStages texture_stages;
        unsigned long render_flags = sub_primitive.PeekRenderFlags();
        unsigned long shader_flags = 0;
        SetupAlphaBlendingStatesFromRenderFlags(render_flags, pass, sub_primitive.PeekAlpha(), sub_primitive.GetParent()->AlphaRef);
        if (render_flags & 0x80)
            SetupStencilMask(primitive->MeshEffect.Get(), render_flags, pass);
        bool render_degenerate_triangles = false;
        if (!SetupRenderModeShadersAndConstants(pass, material, sub_primitive, texture_stages, mipmap_level, &shader_flags,
                                                &render_degenerate_triangles)) {
            g_pEngineLightingManager->RestoreDefaultPrimitiveParameters();
            NEngineInternal::GPRenderManager->FlushRender();
            return;
        }
        bool used_blank_textures;
        if (!SetupStaticBlocks(*mesh_primitive, *primitive, texture_stages, material, composite_surfaces, mipmap_level, &used_blank_textures)) {
            g_pEngineLightingManager->RestoreDefaultPrimitiveParameters();
            NEngineInternal::GPRenderManager->FlushRender();
            return;
        }
        SetupFirstTextures(*mesh_primitive, texture_stages);
        NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_CCW);
        PrepareOcclusionTest(*primitive, pass, occlusion_handle);
        if (occlusion_handle != (unsigned long)-1)
            g_pOcclusionManager->StartOcclusionTest();
        NEngineInternal::GPShaderRenderManager->SelectVertexBuffer(*mesh_primitive->GetVertexBuffer());
        CEnginePrimitiveRenderUtil::SetWorldTransform(sub_primitive.PeekObjectTransform());
        NEngineInternal::GPRenderManager->RealiseRenderState();
        NEngineInternal::GPShaderRenderManager->PrepareForRender(shader_flags);
        RenderMeshPrimitiveBlocks(*mesh_primitive, texture_stages, false, render_degenerate_triangles);
        if (occlusion_handle != (unsigned long)-1)
            g_pOcclusionManager->EndOcclusionTest(occlusion_handle);
        NEngineInternal::GPRenderManager->FlushRender();
        g_pEngineLightingManager->RestoreDefaultPrimitiveParameters();
    }
}
