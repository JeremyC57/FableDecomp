// CEnginePrimitiveManagerRepeatedZSprites::RenderFromSortList(CEngineInternalPrimitiveRepeatedZSprite& sprite) -- draw one sorted z-sprite batch:
// Grade: DIFFER(4600v4600) -- (wfA4 pass 6) 5 true rh hunks (was 7): residue = side_tex Y-row term order 0x00BAAE27/3B/45 and the SetTextureUVModeU bookmark test/or 0x00BAB67E/82; see NOTES.md
//   make sure the sprite's mesh (and its best LOD) is loading, regenerate the billboard texture when the mesh changes, then under scoped
//   shader-enable / VS-constant-layout(2) / render-state capture build the billboard texture projection (view rotation about the bounding
//   sphere, scaled 0.5/r, -0.5/r and offset 0.5), the camera-up control points weighted by VerticalControlPointWeightingFactor /
//   ControlPointWeightingPower, the u clamp range, and upload VS constants 0..0xC + the object table (0x10), set alpha test / fog / stage-1
//   sampler states, and draw count*4 vertices / count*2 primitives inside the occlusion query.
// Leaf: ?RenderFromSortList@CEnginePrimitiveManagerRepeatedZSprites@@AAEXAAVCEngineInternalPrimitiveRepeatedZSprite@@@Z
// Flags: none
// Inferred: 4 dead release asserts at entry (FABLE_RETAIL_ASSERT(sprite.Mesh), fable_assert.h) -- FITTED COUNT (device d, ruler): they only
//   shift every c2 record id (period ~158 asserts); 4 (with 3 fidelity locals, device c) is the best of a=0..170 x k=0..8. FITTED: `((TLX + BRX) * 0.015625f) + 0.015625` parens (0xc ecx load) and `cos((PeekHFOV() * k) * TWO_PI)` grouping (region cut of const 5).
//   Top declared before bottom (natural). Kept while the structure is open.
// Fidelity: one unused `CIVCountedPointer<CTexture> unused_texture;` in the billboard-generation block (owner device (c), class-typed as the matched
//   0x00B751F0; FITTED, k=1): no byte (a null counted pointer: the inline release folds away), but its constructor / release IL re-keys
//   c2's second operand sort of the bookmark tests: fixes the SetTextureFilterModeMin test/or 0x00BAB7C5 (A key +86, B +96, measured).
// Retail: Fable.exe 0x00BAA870, 4600 bytes, thiscall `ret 4`, frame 0x124, no EH.
// Spellings (wfA4 pass 7, byte-neutral, PDB): FableWin ?DrawBuffersIndexed@CShaderRenderManager@@QAEXQAPBVCTexture@@... = an array parameter
//   `const CTexture* textures[]`; ?PeekCameraState@CEngineCamera@@QAEABUCEngineCameraDesc@@XZ = struct key (FABLE_RETAIL_CENGINECAMERADESC_STRUCT).
// Callees: C3DMeshInfo::IsLoaded 0x00A23E40, BeginLoading 0x00A256F0, GetMesh 0x00A25E70, ~CCountedPointer<const C3DMesh2> 0x00A36EC0,
//   CEngineLightingManager::UpdateGlobalLightingStates 0x00B46C80, CEngineBillboardGenerator::RenderBillboard 0x00B8B780,
//   CShaderRenderManager::EnableVertexShaders 0x00987FE0 / DisableVertexShaders 0x009880E0 / EnablePixelShaders 0x00988110 /
//   DisablePixelShaders 0x00988190 / SetVertexShaderConstantLayout 0x0098B5E0 / ApplyVertexShader 0x00988020 / ApplyPixelShader 0x00988140 /
//   SetUserVertexShaderConstant 0x00989A60 (KABVC3DVector M) and 0x00989B00 (KMMMM) / SetUserVertexShaderConstants 0x009896D0 and the
//   PrepareForRender(0x200) updaters, CEngineInternalPrimitiveRepeatedZSprite::GetTransitionFade 0x00BAA460, CPreTransposedBoneMatrix::Set
//   0x00987F70, _CIpow 0x00BFEB40, CEnginePrimitiveManagerOcclusionBase::PrepareOcclusionTest 0x00B905A0,
//   CEngineOcclusionManager::StartOcclusionTest 0x00B8D090 / EndOcclusionTest 0x00B8D0A0, CRenderStateManager::RealiseRenderState 0x00A058C0 /
//   RestoreCaptureBlock 0x00A05840, IDirect3DDevice9 SetStreamSource / SetTexture / SetIndices / DrawIndexedPrimitive.
#define FABLE_RETAIL_C3DVECTOR_INITIALISE_SUB
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C2DBOXF_EXTENT_NAMED
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
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD_DIRECT

#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
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
#define FABLE_RETAIL_C3DBOUNDINGBOX_GETDIMENTIONS
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
#define FABLE_RETAIL_CMATRIX3X4_PRETRANSLATE
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY_XYZ
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS
#define FABLE_RETAIL_CMATRIX3X4_POSTSCALE_XYZ
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINEFADEDISTANCE_INLINE_CTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_CSCOPEDSHADERSENABLE_FORCEINLINE
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARED_ZXY
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_NAMED_SQUARED
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
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

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }    // 0x01436E9C
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }    // 0x01436E98
namespace NEngineInternal { extern CEnginePrimitiveRenderer* GPPrimitiveRenderer; }                       // 0x01436E80
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                                             // 0x01436EA0
extern CEngineLocalDetailGenerator* g_pLocalDetailGenerator;                       // 0x01436E88
namespace NEngineInternal { extern CEngineOcclusionManager* GPOcclusionManager; }                               // 0x01436E5C
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
FABLE_FORCEINLINE void CShaderRenderManager::DrawBuffersIndexed(const CTexture* textures[], unsigned long textureCount, CVertexBufferWin32& vertexBuffer,
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
    float transitionEnd = Clamp(fade.MaxDrawDistance, NEngineInternal::GPPrimitiveRenderer->GetMinimumZSpriteAsMeshDistance(),
                                NEngineInternal::GPPrimitiveRenderer->GetMaximumZSpriteAsMeshDistance());
    fade.FadeBeginDistance -= fade.MaxDrawDistance - transitionEnd;
    fade.MaxDrawDistance = transitionEnd;
    return fade;
}

void CEnginePrimitiveManagerRepeatedZSprites::RenderFromSortList(CEngineInternalPrimitiveRepeatedZSprite& sprite)
{
    FABLE_RETAIL_ASSERT(sprite.Mesh);
    FABLE_RETAIL_ASSERT(sprite.Mesh);
    FABLE_RETAIL_ASSERT(sprite.Mesh);
    FABLE_RETAIL_ASSERT(sprite.Mesh);
    if (!sprite.Mesh->IsLoaded()) {
        sprite.Mesh->BeginLoading(0, false, false);
        return;
    }
    if (sprite.Mesh->GetBestCurrentDetailLevel() != 0)
        sprite.Mesh->BeginLoading(0, false, false);
    if (CurrentRenderedMesh != sprite.Mesh) {
        CCountedPointer<const C3DMesh2> mesh;
        if (!sprite.Mesh->GetMesh(mesh, 0, NEngineInternal::GPPrimitiveRenderer->GetBlockOnLoad() ? 0x11 : 0))
            return;
        if (g_pLocalDetailGenerator->PeekZSpriteGenerationEnabled()) {
            CIVCountedPointer<CTexture> unused_texture;
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
            g_pEngineBillboardGenerator->RenderBillboard(mesh.Get(), &BillboardTexture, sprite.MinDistance, sprite.MaxScale, sprite.AlphaRef);
        }
        CurrentRenderedMesh = sprite.Mesh;
    }
    CScopedShadersEnable shaders(true, true);
    CScopedVertexShaderConstantLayout layout(2);
    CRenderStateScopedCaptureBlock capture;
    NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
    NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VertexShader, false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PixelShader);

    const C3DMeshInfo* mesh_info = sprite.Mesh;
    CEngineCamera& camera = *NEngineInternal::GPCamera;
    C3DBoundingSphere sphere = mesh_info->GetBoundingSphere();
    CMatrix3x4 texture_transform = camera.PeekNonHomogenousViewTransform();
    texture_transform.SetTranslateOnly(0.0f, 0.0f, 0.0f);
    texture_transform.PreTranslate(-sphere.Center);
    float one_on_radius = 1.0f / sphere.Radius;
    texture_transform.PostScale(one_on_radius * 0.5f, one_on_radius * -0.5f, 0.0f);
    texture_transform.PostTranslate(0.5f, 0.5f, 0.0f);
    CMatrix3x4 inverse = camera.PeekInverseNonHomogenousViewTransform();
    C3DVector up(inverse.E31 * 0.0f + inverse.E21 + inverse.E11 * 0.0f, inverse.E32 * 0.0f + inverse.E22 + inverse.E12 * 0.0f, 0.0f);
    up.Normalise();
    C3DVector top(0.0f, 0.0f, sprite.Mesh->GetBoundingBox().GetCenter().Z + sprite.Mesh->GetBoundingBox().GetDimentions().Z * 0.5f);
    C3DVector bottom(0.0f, 0.0f, 0.0f);
    C3DVector side = up * sphere.Radius;
    C3DVector other_side = -up * sphere.Radius;
    C3DVector bottom_tex;
    texture_transform.TransformPoint(bottom, bottom_tex);
    C3DVector top_tex;
    texture_transform.TransformPoint(top, top_tex);
    C3DVector side_tex;
    texture_transform.TransformPoint(side, side_tex);
    C3DVector other_side_tex;
    texture_transform.TransformPoint(other_side, other_side_tex);
    // Fidelity: three unused named locals (device c) re-key the c2 record count (with the 4 dead asserts) -- FITTED count.
    float fidelity0 = sprite.MinDistance;
    float fidelity1 = sprite.MinDistance;
    float fidelity2 = sprite.MinDistance;
    float top_factor = (float)pow(fabsf(bottom_tex.Y - top_tex.Y) * VerticalControlPointWeightingFactor, ControlPointWeightingPower);
    float side_factor = (float)pow(fabsf(side_tex.Y - other_side_tex.Y), ControlPointWeightingPower);
    float one_on_total = 1.0f / (top_factor + side_factor);
    side_factor *= one_on_total;
    top_factor *= one_on_total;
    C3DVector control0 = top * top_factor + side * side_factor;
    C3DVector control1 = bottom * top_factor + other_side * side_factor;
    C3DVector control_tex = top_tex * top_factor + side_tex * side_factor;
    C3DVector control1_tex = bottom_tex * top_factor + other_side_tex * side_factor;
    C3DBoundingBox box = mesh_info->GetBoundingBox();
    C3DVector half_extent = box.GetDimentions() * 0.5f;
    float size = half_extent.GetMagnitude() * one_on_radius * 0.5f;
    float u_min = 0.0f > bottom_tex.X - size ? 0.0f : bottom_tex.X - size;
    float u_max = 1.0f < size + bottom_tex.X ? 1.0f : size + bottom_tex.X;
    CEngineFadeDistance fade = sprite.GetTransitionFade();
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, camera.PeekCameraPos(),
        (float)(cos((camera.PeekHFOV() * 0.0795774683f) * TWO_PI) * (fade.FadeBeginDistance - sphere.Radius * 0.01f)));
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, NEngineInternal::GPCamera->PeekNonHomogenousViewToViewScaleX(),
        NEngineInternal::GPCamera->PeekNonHomogenousViewToViewScaleY(), NEngineInternal::GPCamera->PeekInvZToZBufferFactor(), NEngineInternal::GPCamera->PeekInvZToZBufferOffset());
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, top_factor, side_factor, 1.0f / 256.0f, NonStippleAlphaFactor * 0.5f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, control0 / sphere.Radius, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(4, control1 / sphere.Radius, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(5, sphere.Center / sphere.Radius, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(6, control_tex, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(7, u_min, u_max - u_min, 1.0f / (control1_tex.Y - control_tex.Y), 0.0f);
    CPreTransposedBoneMatrix bone;
    bone.Set(NEngineInternal::GPCamera->PeekNonHomogenousViewTransform());
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstants(8, 3, &bone.Data[0][0]);
    C2DBoxF window = NEngineInternal::GPCamera->PeekCameraState().Window;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0xb, window.GetWidth() * 0.015625f, window.GetHeight() * -0.015625f,
                                                                        ((window.TLX + window.BRX) * 0.015625f) + 0.015625, ((window.TLY + window.BRY) * 0.015625f) + 0.015625);
    float draw_distance_factor = NEngineInternal::GPPrimitiveRenderer->GetZSpriteDrawDistanceFactor();
    float mesh_fade_begin = draw_distance_factor * sprite.FadeDistance.FadeBeginDistance;
    float mesh_fade_scale = 1.0f / (draw_distance_factor * sprite.FadeDistance.MaxDrawDistance - mesh_fade_begin);
    float fade_scale = 1.0f / (fade.MaxDrawDistance - fade.FadeBeginDistance);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0xc, fade_scale, -(fade_scale * fade.FadeBeginDistance),
        mesh_fade_scale * -1.0f, mesh_fade_scale * mesh_fade_begin + 1.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstants(0x10, sprite.ObjectCount, (float*)sprite.ObjectsEntries);
    const CTexture* textures[2] = { BillboardTexture.Texture, &NEngineInternal::GPResourceManager->GetStipleFadeTexture() };
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(EnableZSpriteAlpha);
    NEngineInternal::GPRenderManager->SetAlphaRef(sprite.AlphaRef);
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER);
    NEngineInternal::GPRenderManager->SetSpecularEnable(true);
    NEngineInternal::GPRenderManager->SetFogEnable(true);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(1, TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(1, TEXTURE_FILTER_POINT);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x200);
    unsigned long query;
    PrepareOcclusionTest(sprite, RENDER_PASS_COMBINED, query);
    if (query != 0xffffffff)
        NEngineInternal::GPOcclusionManager->StartOcclusionTest();
    NEngineInternal::GPShaderRenderManager->DrawBuffersIndexed(textures, 2, VertexBuffer, 0, sprite.ObjectCount * 4, IndexBuffer, 0,
                                                               sprite.ObjectCount * 2, PT_NULL);
    if (query != 0xffffffff)
        NEngineInternal::GPOcclusionManager->EndOcclusionTest(query);
}
