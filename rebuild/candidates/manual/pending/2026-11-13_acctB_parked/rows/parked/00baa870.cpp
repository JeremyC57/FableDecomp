// CEnginePrimitiveManagerRepeatedZSprites::RenderFromSortList(CEngineInternalPrimitiveRepeatedZSprite& sprite) -- draw one sorted z-sprite batch:
// Grade: DIFFER(4575v4600) -- frame 0x124 matches (slot order: other_side w15 v retail <=14, shaders/mesh and side_tex/query/top_tex ties); x87: E41 keep + rep movsd order, k fst/fstp, top centre operand order, const 3/4 z consume, push 0xb pop order; SetRenderState sites 2-8 load/test orders; bbox copy order
//   make sure the sprite's mesh (and its best LOD) is loading, regenerate the billboard texture when the mesh changes, then under scoped
//   shader-enable / VS-constant-layout(2) / render-state capture build the billboard texture projection (view rotation about the bounding
//   sphere, scaled 0.5/r, -0.5/r and offset 0.5), the camera-up control points weighted by VerticalControlPointWeightingFactor /
//   ControlPointWeightingPower, the u clamp range, and upload VS constants 0..0xC + the object table (0x10), set alpha test / fog / stage-1
//   sampler states, and draw count*4 vertices / count*2 primitives inside the occlusion query.
// Leaf: ?RenderFromSortList@CEnginePrimitiveManagerRepeatedZSprites@@AAEXAAVCEngineInternalPrimitiveRepeatedZSprite@@@Z
// Flags: none
// Retail: Fable.exe 0x00BAA870, 4600 bytes, thiscall `ret 4`, frame 0x124, no EH.
// Callees: C3DMeshInfo::IsLoaded 0x00A23E40, BeginLoading 0x00A256F0, GetMesh 0x00A25E70, ~CCountedPointer<const C3DMesh2> 0x00A36EC0,
//   CEngineLightingManager::UpdateGlobalLightingStates 0x00B46C80, CEngineBillboardGenerator::RenderBillboard 0x00B8B780,
//   CShaderRenderManager::EnableVertexShaders 0x00987FE0 / DisableVertexShaders 0x009880E0 / EnablePixelShaders 0x00988110 /
//   DisablePixelShaders 0x00988190 / SetVertexShaderConstantLayout 0x0098B5E0 / ApplyVertexShader 0x00988020 / ApplyPixelShader 0x00988140 /
//   SetUserVertexShaderConstant 0x00989A60 (KABVC3DVector M) and 0x00989B00 (KMMMM) / SetUserVertexShaderConstants 0x009896D0 and the
//   PrepareForRender(0x200) updaters, CEngineInternalPrimitiveRepeatedZSprite::GetTransitionFade 0x00BAA460, CPreTransposedBoneMatrix::Set
//   0x00987F70, _CIpow 0x00BFEB40, CEnginePrimitiveManagerOcclusionBase::PrepareOcclusionTest 0x00B905A0,
//   CEngineOcclusionManager::StartOcclusionTest 0x00B8D090 / EndOcclusionTest 0x00B8D0A0, CRenderStateManager::RealiseRenderState 0x00A058C0 /
//   RestoreCaptureBlock 0x00A05840, IDirect3DDevice9 SetStreamSource / SetTexture / SetIndices / DrawIndexedPrimitive.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_ZSPRITE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_PREPAREOCCLUSIONTEST
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDZSPRITES_TYPED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEREPEATEDZSPRITE_TYPED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_FADEDSHADOW_ENABLE_FIRST
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_ZSPRITE_ACCESSORS
#define FABLE_RETAIL_CENGINELOCALDETAILGENERATOR_ZSPRITE
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
#define FABLE_RETAIL_C3DMESHINFO_LODS
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#define FABLE_RETAIL_CPRETRANSPOSEDBONEMATRIX_SET
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_POSTSCALE_XYZ
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINEFADEDISTANCE_INLINE_CTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_CSCOPEDSHADERSENABLE_FORCEINLINE_DTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS
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
#include "engine/CEngineRenderEnums.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }    // 0x01436E9C
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }    // 0x01436E98
extern CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer;                       // 0x01436E80
extern CEngineCamera* g_pEngineCamera;                                             // 0x01436EA0
extern CEngineLocalDetailGenerator* g_pLocalDetailGenerator;                       // 0x01436E88
extern CEngineOcclusionManager* g_pOcclusionManager;                               // 0x01436E5C
extern CEngineBillboardGenerator* g_pEngineBillboardGenerator;                     // 0x01436E4C
static const double TWO_PI = 6.283185307179586;

FABLE_FORCEINLINE void CShaderRenderManager::SelectVertexBuffer(CVertexBufferWin32& vertexBuffer)
{
    D3DDevice->SetStreamSource(0, vertexBuffer.PVertexBuffer, 0, vertexBuffer.Stride);
}
FABLE_FORCEINLINE void CShaderRenderManager::DrawBuffersIndexed(unsigned long minVertexIndex, unsigned long numVertices, CIndexBuffer& indexBuffer,
                                                            unsigned long startIndex, unsigned long primitiveCount, EPrimitiveType primitiveType)
{
    D3DDevice->SetIndices(indexBuffer.PD3DIndexBuffer);
    if (primitiveType == PT_NULL)
        primitiveType = *(EPrimitiveType*)indexBuffer.PrimitiveType;
    RenderManager->RealiseRenderState();
    D3DDevice->DrawIndexedPrimitive(primitiveType, 0, minVertexIndex, numVertices, startIndex, primitiveCount);
}
FABLE_FORCEINLINE void CShaderRenderManager::DrawBuffersIndexed(const CTexture** textures, unsigned long textureCount, CVertexBufferWin32& vertexBuffer,
                                                            unsigned long minVertexIndex, unsigned long numVertices, CIndexBuffer& indexBuffer,
                                                            unsigned long startIndex, unsigned long primitiveCount, EPrimitiveType primitiveType)
{
    SelectVertexBuffer(vertexBuffer);
    for (unsigned long i = 0; i < textureCount; ++i)
        RenderManager->AttachTextureToStage(i, textures[i]);
    DrawBuffersIndexed(minVertexIndex, numVertices, indexBuffer, startIndex, primitiveCount, primitiveType);
}

// Same-TU callee (landed 0x00BAA460, defined earlier in the retail source file and called out of line there): its real body, kept
// out of line, so c2 knows the hidden result's address does not escape.
template <class T> inline T Clamp(T value, T lo, T hi)
{
    return value < lo ? lo : (value > hi ? hi : value);
}
__declspec(noinline) CEngineFadeDistance CEngineInternalPrimitiveRepeatedZSprite::GetTransitionFade() const
{
    CEngineFadeDistance fade(FadeInStart, FadeInEnd);
    float transitionEnd = Clamp(fade.MaxDrawDistance, g_pEnginePrimitiveRenderer->GetMinimumZSpriteAsMeshDistance(),
                                g_pEnginePrimitiveRenderer->GetMaximumZSpriteAsMeshDistance());
    fade.FadeBeginDistance -= fade.MaxDrawDistance - transitionEnd;
    fade.MaxDrawDistance = transitionEnd;
    return fade;
}

void CEnginePrimitiveManagerRepeatedZSprites::RenderFromSortList(CEngineInternalPrimitiveRepeatedZSprite& sprite)
{
    if (!sprite.Mesh->IsLoaded()) {
        sprite.Mesh->BeginLoading(0, false, false);
        return;
    }
    if (sprite.Mesh->GetBestCurrentDetailLevel() != 0)
        sprite.Mesh->BeginLoading(0, false, false);
    if (CurrentRenderedMesh != sprite.Mesh) {
        CCountedPointer<const C3DMesh2> mesh;
        if (!sprite.Mesh->GetMesh(mesh, 0, g_pEnginePrimitiveRenderer->GetBlockOnLoad() ? 0x11 : 0))
            return;
        if (g_pLocalDetailGenerator->PeekZSpriteGenerationEnabled()) {
            bool faded = true;
            if (NEngineInternal::GPLightingManager->ShadowMode != 1)
                faded = false;
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(faded);
            NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
            g_pEngineBillboardGenerator->RenderBillboard(mesh.Get(), &BillboardTexture, sprite.MinDistance, sprite.MaxScale, sprite.AlphaRef);
        }
        CurrentRenderedMesh = sprite.Mesh;
    }
    CScopedShadersEnable shaders(true, true);
    CScopedVertexShaderConstantLayout layout(2);
    CRenderStateScopedCaptureBlock capture;
    bool faded = true;
    if (NEngineInternal::GPLightingManager->ShadowMode != 1)
        faded = false;
    NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(faded);
    NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VertexShader, false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PixelShader);

    const C3DMeshInfo* mesh_info = sprite.Mesh;
    CEngineCamera& camera = *g_pEngineCamera;
    C3DBoundingSphere sphere = mesh_info->GetBoundingSphere();
    CMatrix3x4 texture_transform = camera.PeekNonHomogenousViewTransform();
    C3DVector offset = -sphere.Center;
    texture_transform.E41 = texture_transform.E11 * offset.X + offset.Y * texture_transform.E21 + offset.Z * texture_transform.E31;
    texture_transform.E42 = texture_transform.E12 * offset.X + texture_transform.E22 * offset.Y + texture_transform.E32 * offset.Z;
    texture_transform.E43 = texture_transform.E13 * offset.X + texture_transform.E23 * offset.Y + texture_transform.E33 * offset.Z;
    float one_on_radius = 1.0f / sphere.Radius;
    texture_transform.PostScale(one_on_radius * 0.5f, one_on_radius * -0.5f, 0.0f);
    texture_transform.PostTranslate(0.5f, 0.5f, 0.0f);
    CMatrix3x4 inverse = camera.PeekInverseNonHomogenousViewTransform();
    C3DVector up(inverse.E31 * 0.0f + inverse.E21 + inverse.E11 * 0.0f, inverse.E32 * 0.0f + inverse.E22 + inverse.E12 * 0.0f, 0.0f);
    up.Normalise();
    C3DVector top(0.0f, 0.0f, sprite.Mesh->GetBoundingBox().GetCenter().Z + (sprite.Mesh->GetBoundingBox().PeekMaxCoord() - sprite.Mesh->GetBoundingBox().PeekMinCoord()).Z * 0.5f);
    C3DVector side = up * sphere.Radius;
    C3DVector other_side = -up * sphere.Radius;
    C3DVector bottom(0.0f, 0.0f, 0.0f);
    C3DVector bottom_tex;
    bottom_tex.X = texture_transform.E11 * bottom.X + texture_transform.E21 * bottom.Y + (texture_transform.E31 * bottom.Z + texture_transform.E41);
    bottom_tex.Y = texture_transform.E12 * bottom.X + texture_transform.E22 * bottom.Y + (texture_transform.E32 * bottom.Z + texture_transform.E42);
    bottom_tex.Z = texture_transform.E13 * bottom.X + texture_transform.E23 * bottom.Y + texture_transform.E33 * bottom.Z + texture_transform.E43;
    C3DVector top_tex;
    top_tex.X = texture_transform.E11 * top.X + texture_transform.E21 * top.Y + (texture_transform.E31 * top.Z + texture_transform.E41);
    top_tex.Y = texture_transform.E12 * top.X + texture_transform.E22 * top.Y + (texture_transform.E32 * top.Z + texture_transform.E42);
    top_tex.Z = texture_transform.E13 * top.X + texture_transform.E23 * top.Y + texture_transform.E33 * top.Z + texture_transform.E43;
    C3DVector side_tex = side;
    texture_transform.TransformPoint(side_tex);
    C3DVector other_side_tex = other_side;
    texture_transform.TransformPoint(other_side_tex);
    float distance = fabsf(bottom_tex.Y - top_tex.Y);
    float top_weight = (float)pow(distance * VerticalControlPointWeightingFactor, ControlPointWeightingPower);
    distance = fabsf(side_tex.Y - other_side_tex.Y);
    float side_weight = (float)pow(distance, ControlPointWeightingPower);
    float one_on_total = 1.0f / (top_weight + side_weight);
    float side_factor = one_on_total * side_weight;
    float top_factor = one_on_total * top_weight;
    C3DVector control0 = top * top_factor + side * side_factor;
    C3DVector control1 = bottom * top_factor + other_side * side_factor;
    C3DVector control_tex = top_tex * top_factor + side_tex * side_factor;
    C3DVector control1_tex = bottom_tex * top_factor + other_side_tex * side_factor;
    C3DBoundingBox box = mesh_info->GetBoundingBox();
    C3DVector half_extent = (box.PeekMaxCoord() - box.PeekMinCoord()) * 0.5f;
    float size = (float)sqrt(half_extent.X * half_extent.X + half_extent.Z * half_extent.Z + half_extent.Y * half_extent.Y) * one_on_radius * 0.5f;
    float u_min = 0.0f > bottom_tex.X - size ? 0.0f : bottom_tex.X - size;
    float u_max = 1.0f < size + bottom_tex.X ? 1.0f : size + bottom_tex.X;
    CEngineFadeDistance fade = sprite.GetTransitionFade();
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, camera.PeekCameraPos(),
        (float)(cos(camera.PeekHFOV() * 0.0795774683f * TWO_PI) * (fade.FadeBeginDistance - sphere.Radius * 0.01f)));
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, g_pEngineCamera->PeekNonHomogenousViewToViewScaleX(),
        g_pEngineCamera->PeekNonHomogenousViewToViewScaleY(), g_pEngineCamera->PeekInvZToZBufferFactor(), g_pEngineCamera->PeekInvZToZBufferOffset());
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, top_factor, side_factor, 1.0f / 256.0f, NonStippleAlphaFactor * 0.5f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, control0 / sphere.Radius, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(4, control1 / sphere.Radius, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(5, sphere.Center / sphere.Radius, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(6, control_tex, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(7, u_min, u_max - u_min, 1.0f / (control1_tex.Y - control_tex.Y), 0.0f);
    CPreTransposedBoneMatrix bone;
    bone.Set(g_pEngineCamera->PeekNonHomogenousViewTransform());
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstants(8, 3, &bone.Data[0][0]);
    const C2DBoxF& window = g_pEngineCamera->PeekCameraState().Window;
    float window_left = window.TLX;
    float window_top = window.TLY;
    float window_right = window.BRX;
    float window_bottom = window.BRY;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0xb, (window_right - window_left) * 0.015625f, (window_bottom - window_top) * -0.015625f,
                                                                        (window_right + window_left) * 0.015625f + 0.015625, (window_bottom + window_top) * 0.015625f + 0.015625);
    float draw_distance_factor = g_pEnginePrimitiveRenderer->GetZSpriteDrawDistanceFactor();
    float mesh_fade_begin = draw_distance_factor * sprite.FadeDistance.FadeBeginDistance;
    float mesh_fade_scale = 1.0f / (draw_distance_factor * sprite.FadeDistance.MaxDrawDistance - mesh_fade_begin);
    float fade_scale = 1.0f / (fade.MaxDrawDistance - fade.FadeBeginDistance);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0xc, fade_scale, -(fade_scale * fade.FadeBeginDistance),
        mesh_fade_scale * -1.0f, mesh_fade_scale * mesh_fade_begin + 1.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstants(0x10, sprite.ObjectCount, (float*)sprite.ObjectsEntries);
    const CTexture* textures[2] = { BillboardTexture.Texture, &NEngineInternal::GPResourceManager->GetStipleFadeTexture() };
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(EnableZSpriteAlpha);
    NEngineInternal::GPRenderManager->SetAlphaRef(sprite.AlphaRef);
    NEngineInternal::GPRenderManager->SetAlphaTestFunc((ALPHA_TEST_FUNC)4);
    NEngineInternal::GPRenderManager->SetSpecularEnable(true);
    NEngineInternal::GPRenderManager->SetFogEnable(true);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1, (TEXTURE_UV_MODE)0);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1, (TEXTURE_UV_MODE)0);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(1, (TEXTURE_FILTER_MODE)0);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(1, (TEXTURE_FILTER_MODE)0);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x200);
    unsigned long query;
    PrepareOcclusionTest(sprite, RENDER_PASS_COMBINED, query);
    if (query != 0xffffffff)
        g_pOcclusionManager->StartOcclusionTest();
    NEngineInternal::GPShaderRenderManager->DrawBuffersIndexed(textures, 2, VertexBuffer, 0, sprite.ObjectCount * 4, IndexBuffer, 0,
                                                               sprite.ObjectCount * 2, PT_NULL);
    if (query != 0xffffffff)
        g_pOcclusionManager->EndOcclusionTest(query);
}
