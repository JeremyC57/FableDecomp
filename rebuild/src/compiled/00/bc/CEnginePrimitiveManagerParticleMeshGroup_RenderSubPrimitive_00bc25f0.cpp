// CEnginePrimitiveManagerParticleMeshGroup::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass) -- draw a particle mesh group: fetch the group's mesh (or start its texture load and give up), set the pass states (displacement pass 6 / mask pass 7: displacement shaders and the camera view matrix as constants 1..3; else point lights, blend op / mode and the group pixel shader), then for each mesh primitive with a diffuse map bind its texture, buffers and self illumination and draw the group's particle meshes in batches of up to ten (three transposed object-matrix rows and the colour with doubled alpha per mesh)
// Leaf: ?RenderSubPrimitive@CEnginePrimitiveManagerParticleMeshGroup@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC25F0, 3480 bytes (`sub esp,68h` .. `add esp,68h ; ret 8` at 0x00BC3385), thiscall, no EH frame.
//   Vtable slot 5 of CEnginePrimitiveManagerParticleMeshGroup (RTTI vtable 0x012A63B8; the only reference to 0x00BC25F0).
// Identity: Ego_r.pdb + FableWin.pdb ?RenderSubPrimitive@CEnginePrimitiveManagerParticleMeshGroup@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
//   (FableWin 0x02EB7530, followed by its first-use COMDATs GetReadZBuffer, ApplyMeshPointLightVertexShader, ApplyDisplacementVertexShader,
//   C3DPrimitive2::GetRepeatingMeshBatchSize, C3DMeshMaterial::HasValidDiffuseMap: the helpers this body uses).
// Callees: C3DMeshInfo::GetMesh 0x00A25E70, C3DMeshInfo::BeginLoadingTextures 0x00A23C50 (empty PC body; Ego_r donor order),
//   CEngineScreenEffectDisplacementRenderer::SetupShadowHWAsDepthBuffer 0x00B86A00 (empty PC body; Ego_r donor order),
//   CShaderRenderManager::ApplyVertexShader 0x00988020 / ApplyPixelShader 0x00988140 / SetUserVertexShaderConstant(KMMMM) 0x00989B00 /
//   SetWorldTransform 0x009881F0 / SetUserVertexShaderConstants 0x009896D0 / ResetWorldTransform 0x00988290 and the PrepareForRender(0x11)
//   updaters (SetVSConstants 0x0098B930, UpdatePixelShader 0x00988A20, UpdatePixelShaderConstants 0x00989BF0, UpdateAmbient 0x00989760,
//   UpdateFogColour 0x009897C0, UpdateLightGlobals 0x0098A760, UpdateWorldSpaceLighting 0x0098A540, UpdateCombinedProjectionTransform
//   0x00988A50), CEnginePrimitiveManagerParticleGroup::SetUpPointLights 0x00BBC0B0 / SetupPixelShader 0x00BBCC70,
//   CEnginePrimitiveManagerMeshBase::GetDiffuseTexture 0x00BA9360, CEngineLightingManager::UpdateGlobalLightingStates 0x00B46C80,
//   CRenderManagerCore::DrawCurrentBuffersAsIndexedTris 0x00A0AC60, IDirect3DDevice9 SetTexture / SetStreamSource / SetIndices (slots 65,
//   100, 104), the expanded ~CCountedPointer<const C3DMesh2> (deleter call, operator delete 0x00BFE9BC).
// Globals: NEngineInternal::GPRenderManager 0x01436E18, GPShaderRenderManager 0x01436E14, g_pEnginePrimitiveRenderer 0x01436E80,
//   g_pEnginePrimitiveTypeInfo 0x01436E84 (GetPrimitiveManager(24) = the particle-group manager), g_pDisplacementRenderer_01436e38,
//   g_pEngineLightingManager 0x01436E9C, g_pEngineCamera 0x01436EA0 (established landed spellings); five render switches 0x0139B7A0..
//   0x0139B7B0 (dwords, no PDB data name found: descriptive); D3D tables CRenderStateManager::D3DBlendOpModeTable 0x01396ED0,
//   D3DAlphaBlendModeTable 0x01396F68, D3DAlphaFuncModeTable 0x01396DD8; 1/255 0x01231724 (CRGBFloatColour(const CRGBColour&)).
// Layout: the typed overlay opt-ins (hdr/README.md): manager members at their retail offsets (VertexShadersMeshPointLights +0x20 ..
//   ConstantBlock +0x5C), mesh group MeshInfo +0xAC / WorldTransform +0xB0 / BlendOp +0xE0 / BlendMode +0xE4, particle mesh
//   ObjectTransform +0x48 / Colour +0x78, C3DPrimitive2 VertexBuffer +0x30 / IndexBuffer +0x34 (scoped_ptr), C3DMesh2 primitives +0x90/+0x94.
// Shape (each FITTED unless said): pass 6 sets its blend states through the same helpers as the other passes with constant arguments
//   (SetupBlendOpState(ADD) + SetupAlphaBlendingStatesFromBlendMode(ALPHA)): the code folds to the three direct SetRenderState expansions,
//   but the extra IL is what gives retail's bookmark test / or operand order in every later SetRenderState expansion (written as direct
//   SetBlendOp / SetSource / SetDest calls the order flips after block 7 instead of block 1: grades 11..48); the texture lookup's `failed`
//   flag lives in its own block (frame 0x68 with the 1/255 conversion temporaries in the dead `sub` home, 0x6C at function scope);
//   `repeats < 10 ? repeats : 10` (an if-clamp of one variable allocates the reps load straight into ebp); the colour block's reference is
//   taken before the CRGBFloatColour conversion (edi for the colour bytes, ecx for the block); the batch flush passes `batch_size * 4`.
// Inline-budget: ApplyMeshPointLightVertexShader is FABLE_FORCEINLINE in the header opt-in (c2 trace, cost rule, default T 74: c1 46 at
//   f 0.0625, score 144, refused as plain inline: 866 v 960 equal instructions); SetupBlendOpState (constant site expanded with c1 -13; the
//   variable site c1 154, f 0.0312, score 836) and SetupAlphaBlendingStatesFromBlendMode (constant site c1 168, f 0.125, score 336; the
//   variable site c1 693, over c2's hard 450 cap) are FABLE_FORCEINLINE as in the landed-pending sibling 0x00BB8EF0 (FableWin-only debug
//   copies, no Ego_r.pdb or Fable.exe copy). Every other helper is plain inline.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEMESHGROUP
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEMESH_CLASS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEMESHGROUP
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEGROUP_GETREADZBUFFER
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_PARENTBASE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CENGINECAMERA_NOSCALEVIEWTRANSFORM
#define FABLE_RETAIL_CENGINECAMERA_PEEKNONHOMOGENOUSVIEWTRANSFORM
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_SHADOWHW
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_GETBLOCKONLOAD
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CSHADERRENDERMANAGER_SELECTVERTEXBUFFER
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_C3DMESHINFO_BEGINLOADINGTEXTURES
#define FABLE_RETAIL_C3DMESH2_PRIMITIVES
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_VERTEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_PARTICLEMESH
#define FABLE_RETAIL_C3DMESHMATERIAL_PARTICLEMESH
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_LIST
#include "fable_compiler.h"
#include "engine/CEnginePrimitiveManagerParticleMeshGroup.h"
#include "engine/CEngineInternalPrimitiveParticleMeshGroup.h"
#include "engine/CEngineInternalPrimitiveParticleMesh.h"
#include "engine/CEngineSubPrimitiveBase.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveManagerParticleGroup.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineScreenEffectDisplacementRenderer.h"
#include "engine/CEngineCamera.h"
#include "engine/CCountedPointer.h"
#include "engine/CBankFile.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMesh2.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }      // 0x01436E14
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;                           // 0x01436E84
extern CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer;                           // 0x01436E80
extern CEngineLightingManager* g_pEngineLightingManager;                               // 0x01436E9C
extern CEngineScreenEffectDisplacementRenderer* g_pDisplacementRenderer_01436e38;      // 0x01436E38
extern CEngineCamera* g_pEngineCamera;                                                 // 0x01436EA0
extern long g_particleMeshRenderEnabled;                                      // 0x0139B7B0 (descriptive)
extern long g_particleMeshBlendOpEnabled;                                     // 0x0139B7AC (descriptive)
extern long g_particleMeshBlendModeEnabled;                                   // 0x0139B7A8 (descriptive)
extern long g_particleMeshBatchDrawEnabled;                                   // 0x0139B7A4 (descriptive)
extern long g_particleMeshTailDrawEnabled;                                    // 0x0139B7A0 (descriptive)

#include "engine/GFDebugPDynamicCast.h"

inline bool __fastcall CEnginePrimitiveManager::GetBlockOnLoad()
{
    return g_pEnginePrimitiveRenderer->GetBlockOnLoad();
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
FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::SetupBlendOpState(EEnginePrimitiveBlendOp op)
{
    switch (op) {
    case ENGINE_PRIMITIVE_BLEND_OP_SUB:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_SUBTRACT);
        break;
    case ENGINE_PRIMITIVE_BLEND_OP_REVSUB:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_REVSUBTRACT);
        break;
    default:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_ADD);
        break;
    }
}
FABLE_FORCEINLINE bool CEnginePrimitiveRenderUtil::SetupAlphaBlendingStatesFromBlendMode(EEnginePrimitiveBlendMode mode)
{
    switch (mode) {
    case ENGINE_PRIMITIVE_BLEND_ADDITIVE:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
        return true;
    case ENGINE_PRIMITIVE_BLEND_ADDSMOOTH:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_COLOR);
        return true;
    default:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
        return false;
    }
}

void CEnginePrimitiveManagerParticleMeshGroup::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass)
{
    if (!g_particleMeshRenderEnabled)
        return;
    CEngineInternalPrimitiveParticleMeshGroup* group = (CEngineInternalPrimitiveParticleMeshGroup*)sub.GetParentBase();
    CCountedPointer<const C3DMesh2> mesh;
    if (!group->MeshInfo->GetMesh(mesh, 0, GetBlockOnLoad() ? 0x11 : 0)) {
        group->MeshInfo->BeginLoadingTextures((EBankLoadPriority)1);
        return;
    }
    CEnginePrimitiveManagerParticleGroup* group_manager =
        GFDebugPDynamicCast(g_pEnginePrimitiveTypeInfo->GetPrimitiveManager(24), (CEnginePrimitiveManagerParticleGroup*)0);
    if (pass == RENDER_PASS_DISPLACEMENT_BUFFER) {
        if (group_manager->GetReadZBuffer())
            g_pDisplacementRenderer_01436e38->SetupShadowHWAsDepthBuffer(&group->WorldTransform);
        SetupBlendOpState(ENGINE_PRIMITIVE_BLEND_OP_ADD);
        SetupAlphaBlendingStatesFromBlendMode(ENGINE_PRIMITIVE_BLEND_ALPHA);
        ApplyDisplacementVertexShader();
        ApplyPixelShader(PixelShaderDisplacement);
        CMatrix3x4 view = GetCurrentCamera().PeekNonHomogenousViewTransform();
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 0.0f, 0.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, view.E11, view.E21, view.E31, view.E41);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, view.E12, view.E22, view.E32, view.E42);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, view.E13, view.E23, view.E33, view.E43);
    } else if (pass == RENDER_PASS_DISPLACEMENT_STENCIL) {
        NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
        NEngineInternal::GPRenderManager->SetAlphaRef(16);
        NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER);
        ApplyDisplacementVertexShader();
        ApplyPixelShader(PixelShaderDisplacement);
        CMatrix3x4 view = GetCurrentCamera().PeekNonHomogenousViewTransform();
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 0.0f, 0.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, view.E11, view.E21, view.E31, view.E41);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, view.E12, view.E22, view.E32, view.E42);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, view.E13, view.E23, view.E33, view.E43);
    } else {
        ApplyMeshPointLightVertexShader(group_manager->SetUpPointLights());
        if (g_particleMeshBlendOpEnabled)
            SetupBlendOpState(group->BlendOp);
        bool additive = false;
        if (g_particleMeshBlendModeEnabled)
            additive = SetupAlphaBlendingStatesFromBlendMode(group->BlendMode);
        if (additive)
            group_manager->SetupPixelShader(0, PixelShaderAdditiveAlphaStipple, PixelShaderAdditiveAlpha);
        else
            group_manager->SetupPixelShader(0, PixelShaderStipple, PixelShader);
    }
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(4, 4.0f, 0.0f, 0.0f, 0.0f);
    const CGraphicDataBank* texture_bank = group->MeshInfo->PeekTextureBank();
    for (unsigned long i = 0; i < mesh->GetPrimitiveCount(); ++i) {
        C3DPrimitive2* primitive = mesh->GetPrimitive(i);
        unsigned long repeats = primitive->GetRepeatingMeshBatchSize();
        unsigned long batch_size = repeats < 10 ? repeats : 10;
        const C3DMeshMaterial* material = mesh->GetMaterial(primitive->GetMaterialIndex());
        if (!material->HasValidDiffuseMap())
            continue;
        const CTexture* texture;
        {
            bool failed = false;
            texture = GetDiffuseTexture(material, 0, 0, failed, texture_bank, false);
            if (failed)
                continue;
        }
        NEngineInternal::GPRenderManager->AttachTextureToStage(0, texture);
        g_pEngineLightingManager->SetSelfIlluminationAmbient(material->GetSelfIllumination());
        g_pEngineLightingManager->UpdateGlobalLightingStates();
        NEngineInternal::GPShaderRenderManager->SelectVertexBuffer(*primitive->GetVertexBuffer());
        NEngineInternal::GPRenderManager->SelectIndexBuffer(primitive->GetIndexBuffer(), 0);
        unsigned long batch_triangles = primitive->GetTriangleCount() * batch_size;
        unsigned long batch_vertices = primitive->GetVertexCount() * batch_size;
        unsigned long count = 0;
        for (std::list<CEnginePrimitiveHandle>::const_iterator it = group->Primitives.begin(); it != group->Primitives.end(); ++it) {
            const CEngineInternalPrimitiveParticleMesh& particle = (const CEngineInternalPrimitiveParticleMesh&)it->GetData();
            const CMatrix3x4& transform = particle.ObjectTransform;
            CEngineParticleConstantBlock& row0 = ConstantBlock[count];
            row0.X = transform.E11;
            row0.Y = transform.E21;
            row0.Z = transform.E31;
            row0.W = transform.E41;
            ++count;
            CEngineParticleConstantBlock& row1 = ConstantBlock[count];
            row1.X = transform.E12;
            row1.Y = transform.E22;
            row1.Z = transform.E32;
            row1.W = transform.E42;
            ++count;
            CEngineParticleConstantBlock& row2 = ConstantBlock[count];
            row2.X = transform.E13;
            row2.Y = transform.E23;
            row2.Z = transform.E33;
            row2.W = transform.E43;
            ++count;
            CEngineParticleConstantBlock& col = ConstantBlock[count];
            CRGBFloatColour colour(particle.Colour);
            col.X = colour.R;
            col.Y = colour.G;
            col.Z = colour.B;
            col.W = colour.A * 2.0f;
            ++count;
            if (count == batch_size * 4) {
                NEngineInternal::GPShaderRenderManager->SetWorldTransform(group->WorldTransform);
                NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstants(5, batch_size * 4, &ConstantBlock[0].X);
                NEngineInternal::GPShaderRenderManager->PrepareForRender(0x11);
                if (g_particleMeshBatchDrawEnabled)
                    NEngineInternal::GPRenderManager->DrawCurrentBuffersAsIndexedTris(0, batch_vertices, 0, batch_triangles);
                count = 0;
            }
        }
        if (count) {
            NEngineInternal::GPShaderRenderManager->SetWorldTransform(group->WorldTransform);
            unsigned long meshes = count / 4;
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstants(5, meshes * 4, &ConstantBlock[0].X);
            NEngineInternal::GPShaderRenderManager->PrepareForRender(0x11);
            if (g_particleMeshTailDrawEnabled)
                NEngineInternal::GPRenderManager->DrawCurrentBuffersAsIndexedTris(0, primitive->GetVertexCount() * meshes, 0,
                                                                                 primitive->GetTriangleCount() * meshes);
        }
        g_pEngineLightingManager->RestoreDefaultPrimitiveParameters();
    }
    NEngineInternal::GPShaderRenderManager->ResetWorldTransform();
}