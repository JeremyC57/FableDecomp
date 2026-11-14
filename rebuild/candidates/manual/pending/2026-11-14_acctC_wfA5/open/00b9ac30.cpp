// CEnginePrimitiveManagerRepeatedStaticMeshes::RenderSubPrimitive(CEngineSubPrimitiveBase&, ERenderPassType) -- draw one repeated static mesh sub-primitive: clip its sub-sections (SSE or x87), compact the visible objects into the SIMD block, fetch the diffuse texture, set shaders / stipple-alpha states, light, upload and draw the objects in constant-layout-sized batches inside an occlusion test
// Grade: DIFFER(4229v4229) -- 19 hunks, equal length/calls/frame: operand / register picks only: copy-loop bound re-read loads Size before Start (0x00B9AEB5, 0x00B9B001) and the matching ebx/ebp restore order; inlined GetTexture/TouchResource temps pick edx/eax where retail picks eax/ecx (0x00B9B11E..0x00B9B1C7, 0x00B9B2C4; the landed out-of-line 0x00B66C10 has retail's picks with the same text); SetRenderState `Flags |= mask` operand order at the 2nd/3rd sampler setters (0x00B9B4C3, 0x00B9B534); 0x00B9B26B / 0x00B9BC87 are relocation-only (global object address)
// Leaf: ?RenderSubPrimitive@CEnginePrimitiveManagerRepeatedStaticMeshes@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B9AC30, 4229 bytes, thiscall, `ret 8`, /GS cookie, no EH frame.
// Spellings: EVIDENCED by retail bytes: the two duplicated assignment arms (count == ObjectCount / no sub-sections; cross-jumped tail), `++n, ++j`, the `count - done` batch IV (neg-batch slot 0x00B9B986) with `remaining >= batch ? batch : remaining` (jl at 0x00B9B9C2), the window copied to a local C2DBoxF (member-wise copy: BRX int-copied to the frame 0x00B9B353, frame 0xC4), C3DPrimitive2 accessors (FableWin, as the ParticleMeshGroup sibling 0x00BC25F0). FITTED: none beyond what grading chose between equivalent forms above.
// Flags: /GS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES_RENDERSUB
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_PREPARETEST
#define FABLE_RETAIL_CENGINESUBPRIMITIVEREPEATEDSTATICMESH_GETPARENT
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_TESTS
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_PEEKRENDERFLAGS
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_PEEKVSCONSTANTLAYOUTREPEATEDMESH
#define FABLE_RETAIL_CENGINEVSCONSTANTLAYOUTREPEATEDMESH
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_PARENTBASE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETSTIPLEFADETEXTURE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETTEXTURE_INLINE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CSHADERRENDERMANAGER_SELECTVERTEXBUFFER
#define FABLE_RETAIL_CSHADERRENDERMANAGER_INLINE_BLOCKDRAW
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_C3DMESH2_PRIMITIVES
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_VERTEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_PARTICLEMESH
#define FABLE_RETAIL_C3DMESHMATERIAL_PARTICLEMESH
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ISPERSISTENT
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_ISOCCLUDED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_SETUP_FORCEINLINE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEREPEATEDSTATICMESH
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_BLOCKONLOAD
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DISTANCE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHRENDER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_RENDERSETTINGS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER_SET
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER_OCCLUDED
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADD
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADDTYPED
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_FORCEINLINE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_C3DMESHMATERIAL_FLAG_ACCESSORS
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_C3DMESHINFO_ISLOADED
#define FABLE_RETAIL_C3DMESHINFO_DETAILLEVEL
#define FABLE_RETAIL_C3DMESHINFO_BEGINLOADINGTEXTURES
#define FABLE_RETAIL_C3DMESHINFO_LODERROR
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_PEEKWINDOW
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_CENGINESUBPRIMITIVEMESHBASE_PUBLICMEMBERS
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CSHADOWCLIPVOLUME_CLIPBOUNDINGBOX
#define FABLE_RETAIL_CSHADOWRENDERERSCENEDATA_COPY
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_PEEKSCENEDATA
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#include "fable_compiler.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEngineCameraDesc.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/EEngineOcclusionLayer.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CShadowClipVolume.h"
#include "engine/CShadowRendererSceneData.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineInternalPrimitiveOcclusionBase.h"
#include "engine/CEngineInternalPrimitiveRepeatedStaticMesh.h"
#include "engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h"
#include "engine/CEnginePrimitiveRendererSharedData.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEnginePrimitiveOcclusionInfo.h"
#include "engine/CEnginePrimitiveOcclusionManager.h"
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "engine/CEngineSubPrimitiveBoundingBox.h"
#include "engine/CEngineSubPrimitiveRepeatedStaticMesh.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMesh2.h"
#include "engine/CCountedPointer.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
#include "engine/CEngineOcclusionManager.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEngineVSConstantLayoutRepeatedMesh.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfo.h"
#include "engine/CGraphicInfoFrame.h"
#include "engine/C3DPrimitive2.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }      // 0x01436E14
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }        // 0x01436E98
namespace NEngineInternal { extern CEngineOcclusionManager* GPOcclusionManager; }      // 0x01436E5C ?GPOcclusionManager@NEngineInternal@@3PAVCEngineOcclusionManager@@A

extern bool g_SSEAvailable;                                              // 0x013D2880
extern CEngineShadowRenderer*              g_pEngineShadowRenderer;        // 0x01436E60
extern CEnginePrimitiveRenderer*           g_pEnginePrimitiveRenderer;     // 0x01436E80
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }        // 0x01436E9C ?GPLightingManager@NEngineInternal@@3PAVCEngineLightingManager@@A
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                          // 0x01436EA0 ?GPCamera@NEngineInternal@@3PAVCEngineCamera@@A

FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic,unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}

using namespace NEnginePrimitiveManagerRepeatedStaticMeshes;

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

void CEnginePrimitiveManagerRepeatedStaticMeshes::RenderSubPrimitive(CEngineSubPrimitiveBase& sub_primitive, ERenderPassType pass)
{
    CEngineInternalPrimitiveRepeatedStaticMesh* primitive = static_cast<CEngineSubPrimitiveRepeatedStaticMesh&>(sub_primitive).GetParent();
    float fade_distance = g_pEnginePrimitiveRenderer->GetRepeatedMeshDrawDistanceFactor() * primitive->FadeDistance.MaxDrawDistance;
    for (long i = 0; i < 4; ++i)
        SIMDVariableBlock->FadeDistance[i] = fade_distance;
    long aligned_count = (primitive->ObjectCount + 3) & ~3;
    const float* normal_x = primitive->VectorNormals;
    const float* normal_y = normal_x + aligned_count;
    const float* normal_z = normal_y + aligned_count;
    const unsigned char* wind_delays = 0;
    long count;
    const float* render_x;
    const float* render_y;
    const float* render_z;
    C4DVector* matrices;
    const C4DVector* offsets;
    CClipListElement clip_list[32];
    unsigned char visible_wind_delays[32];
    if (primitive->SubSections) {
        CClipListElement* clip_end = clip_list;
        if (g_SSEAvailable)
            ClipSubSectionsSIMD(primitive->SubSections, 0, clip_end);
        else
            ClipSubSections(primitive->SubSections, 0, fade_distance, false, clip_end);
        long clip_count = clip_end - clip_list;
        if (clip_count == 0)
            return;
        count = 0;
        for (long c = 0; c < clip_count; ++c)
            count += clip_list[c].Size;
        if (count == primitive->ObjectCount) {
            render_x = normal_x;
            render_y = normal_y;
            render_z = normal_z;
            matrices = primitive->ObjectMatricies;
            offsets = primitive->ObjectOffsets;
            wind_delays = primitive->WindDelayArray;
        } else {
            CSIMDVariableBlock* block = SIMDVariableBlock;
            render_x = block->NormalX;
            render_y = block->NormalY;
            render_z = block->NormalZ;
            matrices = block->ObjectMatricies;
            offsets = block->ObjectOffsets;
            long n = 0;
            if (primitive->WindDelayArray) {
                wind_delays = visible_wind_delays;
                for (long c = 0; c < clip_count; ++c) {
                    const CClipListElement& clip = clip_list[c];
                    for (long j = clip.Start; j < clip.Start + clip.Size; ++n, ++j) {
                        SIMDVariableBlock->NormalX[n] = normal_x[j];
                        SIMDVariableBlock->NormalY[n] = normal_y[j];
                        SIMDVariableBlock->NormalZ[n] = normal_z[j];
                        SIMDVariableBlock->ObjectMatricies[n] = primitive->ObjectMatricies[j];
                        SIMDVariableBlock->ObjectOffsets[n] = primitive->ObjectOffsets[j];
                        visible_wind_delays[n] = primitive->WindDelayArray[j];
                    }
                }
            } else {
                for (long c = 0; c < clip_count; ++c) {
                    const CClipListElement& clip = clip_list[c];
                    for (long j = clip.Start; j < clip.Start + clip.Size; ++n, ++j) {
                        SIMDVariableBlock->NormalX[n] = normal_x[j];
                        SIMDVariableBlock->NormalY[n] = normal_y[j];
                        SIMDVariableBlock->NormalZ[n] = normal_z[j];
                        SIMDVariableBlock->ObjectMatricies[n] = primitive->ObjectMatricies[j];
                        SIMDVariableBlock->ObjectOffsets[n] = primitive->ObjectOffsets[j];
                    }
                }
            }
        }
    } else {
        count = primitive->ObjectCount;
        render_x = normal_x;
        render_y = normal_y;
        render_z = normal_z;
        matrices = primitive->ObjectMatricies;
        offsets = primitive->ObjectOffsets;
        wind_delays = primitive->WindDelayArray;
    }
    C3DPrimitive2* mesh_primitive = static_cast<CEngineSubPrimitiveMeshBase&>(sub_primitive).MeshPrimitive;
    const C3DMeshMaterial* material = primitive->GetRenderInfo()->Mesh->GetMaterial(mesh_primitive->GetMaterialIndex());
    const CGraphicDataBank* bank = primitive->MeshInfo->PeekTextureBank();
    const CGraphicInfoFrame* frame = bank->PeekGraphicInfoFrame(material->DiffuseMapBankIndex, 0);
    const CTexture* texture;
    if (!frame->GetTexture(&texture, 0, 0))
        return;
    CRenderStateScopedCaptureBlock capture;
    NEngineInternal::GPRenderManager->SetSpecularEnable(true);
    CScopedVertexShaderConstantLayout layout(4);
    if (sub_primitive.PeekRenderFlags() & 8) {
        long stipple_stage;
        if (sub_primitive.PeekRenderFlags() & 0x200) {
            NEngineInternal::GPShaderRenderManager->ApplyVertexShader(*VertexShaderShadowedStippleAlpha, false);
            NEngineInternal::GPShaderRenderManager->ApplyPixelShader(*PixelShaderShadowedStippleAlpha);
            stipple_stage = 2;
        } else {
            NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VertexShaderStippleAlpha, false);
            NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PixelShaderStippleAlpha);
            stipple_stage = 1;
        }
        const C2DBoxF window = NEngineInternal::GPCamera->PeekWindow();
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, (window.BRX - window.TLX) * 0.015625f,
            (window.BRY - window.TLY) * -0.015625f, window.TLX * 0.03125f + 0.015625, window.TLY * 0.03125f + 0.015625);
        NEngineInternal::GPRenderManager->SetTextureFilterModeMin(stipple_stage, TEXTURE_FILTER_POINT);
        NEngineInternal::GPRenderManager->SetTextureFilterModeMag(stipple_stage, TEXTURE_FILTER_POINT);
        NEngineInternal::GPRenderManager->SetTextureUVModeU(stipple_stage, TEXTURE_UV_WRAP);
        NEngineInternal::GPRenderManager->SetTextureUVModeV(stipple_stage, TEXTURE_UV_WRAP);
        NEngineInternal::GPRenderManager->AttachTextureToStage(stipple_stage, &NEngineInternal::GPResourceManager->GetStipleFadeTexture());
    } else if (sub_primitive.PeekRenderFlags() & 0x200) {
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(*VertexShaderShadowed, false);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(*PixelShaderShadowed);
    } else {
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VertexShader, false);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PixelShader);
    }
    bool faded = !(sub_primitive.PeekRenderFlags() & 0x200);
    NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(faded);
    NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
    if (primitive->WindDelayArray)
        UpdateBoneMatrixForWindSheer(*primitive, matrices, wind_delays, count);
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, texture);
    if (material->TwoSided)
        NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaRef(primitive->AlphaRef);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x204);
    NEngineInternal::GPShaderRenderManager->SelectVertexBuffer(*mesh_primitive->GetVertexBuffer());
    unsigned long occlusion_handle;
    PrepareOcclusionTest(*primitive, pass, occlusion_handle);
    if (occlusion_handle != (unsigned long)-1)
        NEngineInternal::GPOcclusionManager->StartOcclusionTest();
    if (sub_primitive.PeekRenderFlags() & 0x200) {
        CEngineShadowRenderer::CColorBufferDepthPackParameters parameters(3, 5, 0);
        g_pEngineShadowRenderer->SetupShadowFuncRenderStates(1, parameters);
    }
    const long batch_size = NEngineInternal::GPResourceManager->PeekVSConstantLayoutRepeatedMesh().ObjectMatricies.Count;
    for (long done = 0; ; done += batch_size) {
        long remaining = count - done;
        long n = remaining >= batch_size ? batch_size : remaining;
        if (n <= 0)
            break;
        ProcessLightingSW(*primitive, render_x + done, render_y + done, render_z + done, offsets + done, n, (sub_primitive.PeekRenderFlags() & 0x200) != 0);
        UploadShaderConstants(*primitive, offsets + done, matrices + done, n);
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
        NEngineInternal::GPShaderRenderManager->DrawBuffersIndexed(0, mesh_primitive->GetVertexCount() * n, *mesh_primitive->GetIndexBuffer(), 0,
            mesh_primitive->GetTriangleCount() * n, PT_NULL);
    }
    if (occlusion_handle != (unsigned long)-1)
        NEngineInternal::GPOcclusionManager->EndOcclusionTest(occlusion_handle);
    NEngineInternal::GPRenderManager->FlushRender();
    NEngineInternal::GPLightingManager->RestoreDefaultPrimitiveParameters();
}
