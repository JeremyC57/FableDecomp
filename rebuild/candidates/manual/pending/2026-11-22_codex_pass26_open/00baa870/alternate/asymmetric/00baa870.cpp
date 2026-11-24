// CEnginePrimitiveManagerRepeatedZSprites::RenderFromSortList()
// Flags: (none)
// Leaf: ?RenderFromSortList@CEnginePrimitiveManagerRepeatedZSprites@@AAEXAAVCEngineInternalPrimitiveRepeatedZSprite@@@Z
#define FABLE_RETAIL_ORIGINAL_PRIMITIVE_TO_D3D_PASS26
#define FABLE_RETAIL_C3DVECTOR_INITIALISE_SUB
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C2DBOXF_VIEWPORT_WIDTH_TEMPORARY_CONTROL_PASS26
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_PASS22_CORE_ORIGINAL_BUFFER_ACCESSORS
#define FABLE_RETAIL_CINDEXBUFFER_PRIMITIVE_TYPE_PASS26
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_ZSPRITE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_PREPAREOCCLUSIONTEST
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDZSPRITES_TYPED_PASS26
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEREPEATEDZSPRITE_TYPED_PASS26
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD_DIRECT

#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_ZSPRITE_ACCESSORS
#define FABLE_RETAIL_CENGINELOCALDETAILGENERATOR_ZSPRITE_PASS26
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_OCCLUSIONTEST
#define FABLE_RETAIL_CENGINEBILLBOARDGENERATOR
#define FABLE_RETAIL_CENGINEBILLBOARDGENERATOR_RENDERBILLBOARD
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETSTIPLEFADETEXTURE
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGBOX_CENTER_MIN_FIRST
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_C3DMESHINFO_ISLOADED
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGSPHERE
#define FABLE_RETAIL_C3DMESHINFO_DETAILLEVEL
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#define FABLE_RETAIL_CPRETRANSPOSEDBONEMATRIX_SET
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_PARTICLE_POINT
#define FABLE_RETAIL_CMATRIX3X4_ZSPRITE_SOURCE_GEOMETRY_PASS26
#define FABLE_RETAIL_CMATRIX3X4_DATA_UNION
#define FABLE_RETAIL_CMATRIX3X4_PRETRANSLATE
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY_XYZ
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS
#define FABLE_RETAIL_CMATRIX3X4_POSTSCALE_XYZ
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_DIV_ZSPRITE_OWNER_GROUP_PASS26
#define FABLE_RETAIL_CENGINEFADEDISTANCE_INLINE_CTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION_SQUARE_PAREN_PASS26
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#define FABLE_RETAIL_CSCOPEDSHADERSENABLE_FORCEINLINE
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_CENGINECAMERA_PEEKWINDOW
#define FABLE_RETAIL_C3DBOUNDINGBOX_SETMINMAX
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_GETBLOCKONLOAD
#define FABLE_RETAIL_CPRETRANSPOSEDBONEMATRIX_ZSPRITE_CONSTRUCTOR
#include "fable_compiler.h"
#include <math.h>
#pragma intrinsic(sqrt)
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEnginePrimitiveRendererSharedData.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEnginePrimitiveManagerRepeatedZSprites.h"
#include "engine/CEngineInternalPrimitiveRepeatedZSprite.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineLocalDetailGenerator.h"
#include "engine/CEngineOcclusionManager.h"
#include "engine/CEngineBillboardGenerator.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DMeshInfo.h"
#include "engine/CPreTransposedBoneMatrix.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DVector.h"
#include "engine/CTexture.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/GFCos.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }    // 0x01436E9C
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }    // 0x01436E98
namespace NEngineInternal { extern CEnginePrimitiveRenderer* GPPrimitiveRenderer; }                       // 0x01436E80
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                                             // 0x01436EA0
extern CEngineLocalDetailGenerator* g_pLocalDetailGenerator;                       // 0x01436E88
namespace NEngineInternal { extern CEngineOcclusionManager* GPOcclusionManager; }                               // 0x01436E5C
extern CEngineBillboardGenerator* g_pEngineBillboardGenerator;                     // 0x01436E4C
static const double TWO_PI = 6.2831854820251465;

inline IDirect3DDevice9* CShaderRenderManager::GetD3DDevice()
{
    return D3DDevice;
}

inline bool FABLE_FASTCALL CEnginePrimitiveManager::GetBlockOnLoad()
{
    return NEngineInternal::GPPrimitiveRenderer->GetBlockOnLoad();
}

// Inline-budget: the original three draw helpers expand through slots 0x190, 0x1A0 and 0x148.
FABLE_FORCEINLINE void CShaderRenderManager::SelectVertexBuffer(CVertexBufferWin32& vertex_buffer)
{
    GetD3DDevice()->SetStreamSource(0, vertex_buffer.GetD3DVertexBuffer(), 0, vertex_buffer.GetStride());
}
FABLE_FORCEINLINE void CShaderRenderManager::DrawBuffersIndexed(unsigned long min_vertex_index, unsigned long no_vertices, CIndexBuffer& index_buffer,
                                                            unsigned long first_index, unsigned long no_primitives, EPrimitiveType prim_type)
{
    D3DDevice->SetIndices(index_buffer.GetD3DIndexBuffer());
    if (prim_type == PT_NULL)
        prim_type = index_buffer.GetPrimitiveType();
    RenderManager->RealiseRenderState();
    D3DDevice->DrawIndexedPrimitive(GFPrimitiveTypeToD3DPT(prim_type), 0, min_vertex_index, no_vertices, first_index, no_primitives);
}
FABLE_FORCEINLINE void CShaderRenderManager::DrawBuffersIndexed(const CTexture* textures[], unsigned long no_textures, CVertexBufferWin32& vertex_buffer,
                                                            unsigned long min_vertex_index, unsigned long no_vertices, CIndexBuffer& index_buffer,
                                                            unsigned long first_index, unsigned long no_primitives, EPrimitiveType prim_type)
{
    SelectVertexBuffer(vertex_buffer);
    for (long c_stage = 0; static_cast<unsigned long>(c_stage) < no_textures; ++c_stage)
        RenderManager->AttachTextureToStage(c_stage, textures[c_stage]);
    DrawBuffersIndexed(min_vertex_index, no_vertices, index_buffer, first_index, no_primitives, prim_type);
}

// Same-TU callee (landed 0x00BAA460, defined earlier in the retail source file and called out of line there): its real body, kept
// out of line, so c2 knows the hidden result's address does not escape.
#ifdef _MSC_VER
__declspec(noinline)
#endif
CEngineFadeDistance CEngineInternalPrimitiveRepeatedZSprite::GetTransitionFade() const
{
    CEngineFadeDistance fade(FadeInStart, FadeInEnd);
    float min_mesh_dist = NEngineInternal::GPPrimitiveRenderer->GetMinimumZSpriteAsMeshDistance();
    float max_mesh_dist = NEngineInternal::GPPrimitiveRenderer->GetMaximumZSpriteAsMeshDistance();
    float clamped_dist = fade.MaxDrawDistance < min_mesh_dist ? min_mesh_dist :
        (fade.MaxDrawDistance > max_mesh_dist ? max_mesh_dist : fade.MaxDrawDistance);
    fade.FadeBeginDistance -= fade.MaxDrawDistance - clamped_dist;
    fade.MaxDrawDistance = clamped_dist;
    return fade;
}

void CEnginePrimitiveManagerRepeatedZSprites::RenderFromSortList(CEngineInternalPrimitiveRepeatedZSprite& primitive)
{
    if (!primitive.Mesh->IsLoaded()) {
        primitive.Mesh->BeginLoading(0, false, false);
        return;
    }
    if (primitive.Mesh->GetBestCurrentDetailLevel() != 0)
        primitive.Mesh->BeginLoading(0, false, false);
    if (CurrentRenderedMesh != primitive.Mesh) {
        CCountedPointer<const C3DMesh2> mesh;
        if (!primitive.Mesh->GetMesh(mesh, 0, GetBlockOnLoad() ? 0x11 : 0))
            return;
        if (g_pLocalDetailGenerator->PeekZSpriteGenerationEnabled()) {
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
            g_pEngineBillboardGenerator->RenderBillboard(mesh.Get(), &BillboardTexture, primitive.MinDistance, primitive.MaxScale, primitive.AlphaRef);
        }
        CurrentRenderedMesh = primitive.Mesh;
    }
    CScopedShadersEnable enable_shaders(true, true);
    CScopedVertexShaderConstantLayout layout(2);
    CRenderStateScopedCaptureBlock block;
    NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
    NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VertexShader, false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PixelShader);

    C3DBoundingSphere mesh_sphere = primitive.Mesh->GetBoundingSphere();
    CMatrix3x4 object_to_texture_matrix = NEngineInternal::GPCamera->PeekNonHomogenousViewTransform();
    object_to_texture_matrix.SetTranslateOnly(0.0f, 0.0f, 0.0f);
    object_to_texture_matrix.PreTranslate(-mesh_sphere.Center);
    object_to_texture_matrix.PostScale((1.0f / mesh_sphere.Radius) * 0.5f,
                                       (1.0f / mesh_sphere.Radius) * -0.5f, 0.0f);
    object_to_texture_matrix.PostTranslate(0.5f, 0.5f, 0.0f);

    CMatrix3x4 inv_view_mat = NEngineInternal::GPCamera->PeekInverseNonHomogenousViewTransform();
    inv_view_mat.SetTranslateOnly(0.0f, 0.0f, 0.0f);
    C3DVector back_vec_os(0.0f, 1.0f, 0.0f);
    inv_view_mat.TransformPoint(back_vec_os);
    back_vec_os.Z = 0.0f;
    back_vec_os.Normalise();

    C3DVector base_cp_object_space(0.0f, 0.0f, 0.0f);
    C3DVector top_cp_object_space(0.0f, 0.0f, primitive.Mesh->GetBoundingBox().GetCenter().Z +
                                             primitive.Mesh->GetBoundingBox().GetDiagonal().Z);
    C3DVector back_cp_object_space = back_vec_os * mesh_sphere.Radius;
    C3DVector front_cp_object_space = -back_vec_os * mesh_sphere.Radius;
    C3DVector sphere_center_object_space = mesh_sphere.GetCenter();

    C3DVector base_cp_texture_space;
    C3DVector top_cp_texture_space;
    C3DVector back_cp_texture_space;
    C3DVector front_cp_texture_space;
    object_to_texture_matrix.TransformPoint(base_cp_object_space, base_cp_texture_space);
    object_to_texture_matrix.TransformPoint(top_cp_object_space, top_cp_texture_space);
    object_to_texture_matrix.TransformPoint(back_cp_object_space, back_cp_texture_space);
    object_to_texture_matrix.TransformPoint(front_cp_object_space, front_cp_texture_space);

    float cp_pair_weight_v = fabs(base_cp_texture_space.Y - top_cp_texture_space.Y) * VerticalControlPointWeightingFactor;
    float cp_pair_weight_h = fabs(back_cp_texture_space.Y - front_cp_texture_space.Y);
    cp_pair_weight_v = pow(cp_pair_weight_v, ControlPointWeightingPower);
    cp_pair_weight_h = pow(cp_pair_weight_h, ControlPointWeightingPower);
    float total = cp_pair_weight_v + cp_pair_weight_h;
    cp_pair_weight_h /= total;
    cp_pair_weight_v /= total;
    C3DVector control_point1_object_space = top_cp_object_space * cp_pair_weight_v + back_cp_object_space * cp_pair_weight_h;
    C3DVector control_point2_object_space = base_cp_object_space * cp_pair_weight_v + front_cp_object_space * cp_pair_weight_h;
    C3DVector control_point1_texture_space = top_cp_texture_space * cp_pair_weight_v + back_cp_texture_space * cp_pair_weight_h;
    C3DVector control_point2_texture_space = base_cp_texture_space * cp_pair_weight_v + front_cp_texture_space * cp_pair_weight_h;

    float u_radius = primitive.Mesh->GetBoundingBox().GetDiagonal().GetMagnitude() * (1.0f / mesh_sphere.Radius) * 0.5f;
    float u_left = 0.0f > base_cp_texture_space.X - u_radius ? 0.0f : base_cp_texture_space.X - u_radius;
    float u_right = 1.0f < base_cp_texture_space.X + u_radius ? 1.0f : base_cp_texture_space.X + u_radius;

    CEngineFadeDistance near_fade = primitive.GetTransitionFade();
    float world_space_radius = mesh_sphere.Radius * 0.01f;
    float half_fov = NEngineInternal::GPCamera->PeekHFOV() * (float)(0.5 / TWO_PI);
    float clip_z = GFCos(half_fov) * (near_fade.FadeBeginDistance - world_space_radius);

    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, NEngineInternal::GPCamera->PeekCameraPos(), clip_z);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, NEngineInternal::GPCamera->PeekNonHomogenousViewToViewScaleX(),
        NEngineInternal::GPCamera->PeekNonHomogenousViewToViewScaleY(), NEngineInternal::GPCamera->PeekInvZToZBufferFactor(), NEngineInternal::GPCamera->PeekInvZToZBufferOffset());
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, cp_pair_weight_v, cp_pair_weight_h, 1.0f / 256.0f, NonStippleAlphaFactor * 0.5f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, control_point1_object_space / mesh_sphere.Radius, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(4, control_point2_object_space / mesh_sphere.Radius, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(5, sphere_center_object_space / mesh_sphere.Radius, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(6, control_point1_texture_space, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(7, u_left, u_right - u_left, 1.0f / (control_point2_texture_space.Y - control_point1_texture_space.Y), 0.0f);
    CPreTransposedBoneMatrix view_matrix(NEngineInternal::GPCamera->PeekNonHomogenousViewTransform());
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstants(8, 3, &view_matrix.Data[0][0]);

    float sts = 32.0f;
    C2DBoxF window = NEngineInternal::GPCamera->PeekWindow();
    // Inferred: retail keeps float extents and the double viewport offset (0x00BAB33C, 0x00BAB355).
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(
        0xb, window.GetWidth() / 2.0f / sts, -window.GetHeight() / 2.0f / sts,
        (window.TLX + window.BRX) / 2.0f / sts + 0.5 / sts,
        (window.TLY + window.BRY) / 2.0f / sts + 0.5 / sts);

    float draw_dist_factor = NEngineInternal::GPPrimitiveRenderer->GetZSpriteDrawDistanceFactor();
    CEngineFadeDistance far_fade;
    far_fade.FadeBeginDistance = draw_dist_factor * primitive.FadeDistance.FadeBeginDistance;
    far_fade.MaxDrawDistance = draw_dist_factor * primitive.FadeDistance.MaxDrawDistance;

    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(
        0xc, 1.0f / (near_fade.MaxDrawDistance - near_fade.FadeBeginDistance),
        -(near_fade.FadeBeginDistance * (1.0f / (near_fade.MaxDrawDistance - near_fade.FadeBeginDistance))),
        (1.0f / (far_fade.MaxDrawDistance - far_fade.FadeBeginDistance)) * -1.0f,
        far_fade.FadeBeginDistance * (1.0f / (far_fade.MaxDrawDistance - far_fade.FadeBeginDistance)) + 1.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstants(0x10, primitive.ObjectCount, (float*)primitive.ObjectsEntries);
    const CTexture* texture[2] = { BillboardTexture.Texture, &NEngineInternal::GPResourceManager->GetStipleFadeTexture() };
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(EnableZSpriteAlpha);
    NEngineInternal::GPRenderManager->SetAlphaRef(primitive.AlphaRef);
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER);
    NEngineInternal::GPRenderManager->SetSpecularEnable(true);
    NEngineInternal::GPRenderManager->SetFogEnable(true);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(1, TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(1, TEXTURE_FILTER_POINT);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x200);
    unsigned long occlusion_handle;
    PrepareOcclusionTest(primitive, RENDER_PASS_COMBINED, occlusion_handle);
    if (occlusion_handle != 0xffffffff)
        NEngineInternal::GPOcclusionManager->StartOcclusionTest();
    NEngineInternal::GPShaderRenderManager->DrawBuffersIndexed(texture, 2, VertexBuffer, 0, primitive.ObjectCount * 4, IndexBuffer, 0,
                                                               primitive.ObjectCount * 2, PT_NULL);
    if (occlusion_handle != 0xffffffff)
        NEngineInternal::GPOcclusionManager->EndOcclusionTest(occlusion_handle);
}
