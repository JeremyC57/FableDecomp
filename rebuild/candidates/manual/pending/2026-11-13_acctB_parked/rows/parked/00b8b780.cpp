// CEngineBillboardGenerator::RenderBillboard(const C3DMesh2* mesh, CEngineBillboard* billboard, float screenSize, float distance, long alphaRef)
// Grade: DIFFER(3921v3921, 23 hunks) -- only c2 record-id ties left: bookmark test/or operand order in setter blocks 3..5 (0x00B8BC75 AlphaRef, 0x00B8BCE2 AlphaTestFunc, 0x00B8BD49 FogEnable) and loop-2 blocks 0x00B8C325 / 0x00B8C393; PROBE: +108..112 dead outer names anywhere in the body gives RELOCATION_MATCH (retail has ~110 more c1 records)
//   -- render a mesh into a billboard texture: bind the billboard texture's top level (with its own or the engine's scratch D24S8
//   depth surface), clear it, draw every primitive with an orthographic transform fitted to the mesh's bounding sphere, then fill
//   each lower mip level by drawing the texture's top level as a screen quad (pixel / vertex shaders off).
// Retail: Fable.exe 0x00B8B780, 3921 bytes, thiscall `ret 14h`, /GS, biased ebp frame.
// Callees: CBaseClass() 0x0099A2D0 / ~CBaseClass 0x0099A2E0, CTexture::GetDimensions 0x009F9DB0 / GetSurfaceLevel 0x009F9E00 /
//   GetNoLevels 0x009F9DF0, CEngine::GetScratchZBufferD24S8 0x00B27EE0, CRenderTarget::AttachZBuffer 0x00A10E90 / AttachTarget
//   0x00A10E60 / DetachTarget 0x00A10E10, CSurface::Uninitialise 0x009F2E20, CDisplayManager::SetRenderTarget 0x009BF160 /
//   SetRenderTargetAsBackScreen 0x009BF220, CRenderManager2D::ClearRenderTarget 0x009D8CF0, CShaderRenderManager::SetViewTransform
//   0x00988350 / SetProjectionTransform 0x00988490 / SetWorldTransform 0x009881F0 / ApplyVertexShader 0x00988020 / ApplyPixelShader
//   0x00988140 (+ the PrepareForRender Update* helpers), CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksNoMorph 0x00BA48F0 /
//   SetupFirstTextures 0x00B9D670 / RenderMeshPrimitiveBlocks 0x00BA1110, CRenderManagerCore::SetVertexFormat 0x00A0AA30 /
//   DrawAsIndexedTris 0x00A0AEA0, CEngineCamera::RefreshD3DTransforms 0x00B2FC50, CRenderStateManager::RestoreCaptureBlock 0x00A05840.
// Retail-asm: 0x00B8BEDE `fistp dword ptr [ebp+4Ch]` -- the mip level through the shared GFFloatToLongFloor helper (engine inline asm).
// Flags: /GS
// Leaf: ?RenderBillboard@CEngineBillboardGenerator@@QAEXPBVC3DMesh2@@PAVCEngineBillboard@@MMJ@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CTEXTURE_GETNOLEVELS
#define FABLE_RETAIL_CTEXTURE_GETWIDTH
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_BACKSCREEN
#define FABLE_RETAIL_CDISPLAYMANAGER_SETRENDERTARGET
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CENGINECAMERA_NOSCALEVIEWTRANSFORM
#define FABLE_RETAIL_CENGINECAMERA_REFRESHD3DTRANSFORMS
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY
#define FABLE_RETAIL_CMATRIX3X4_BILLBOARD
#define FABLE_RETAIL_CMATRIX3X4_POSTSCALE3
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CENGINEBILLBOARDGENERATOR
#define FABLE_RETAIL_CENGINEBILLBOARDGENERATOR_RENDERBILLBOARD
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS
#define FABLE_RETAIL_C3DMESH2_HELPERPOINTS
#define FABLE_RETAIL_C3DMESHSTATS_BOUNDINGSPHERE
#define FABLE_RETAIL_C3DPRIMITIVE2_HIERARCHY
#define FABLE_RETAIL_C3DPRIMITIVE2_VERTEXBUFFER
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_BILLBOARD
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MIPMAPSIZEMULTIPLIER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MIPMAPSIZEMULTIPLIER
#define FABLE_RETAIL_CENGINE_SCRATCHZBUFFER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY_SEPARATE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CTOR_INITIALISE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#include "engine/Direct3D9Types.h"
#include "engine/CSurface.h"
#include "engine/CTexture.h"
#include "engine/CBaseClass.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CEngineCamera.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DVector.h"
#include "engine/C4DVector.h"
#include "engine/CRGBColour.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CTVertexRHWColTex1.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CEnginePrimitiveManagerMeshBase.h"
#include "engine/CEnginePrimitiveManagerStaticMeshesBase.h"
#include "engine/C3DPrimitive2.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DTextureStages.h"
#include "engine/CGraphicInfo.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CEngine.h"
#include "engine/CEngineBillboardGenerator.h"
#include "engine/GFFloatToLongFloor.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }            // 0x01436E1C
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }    // 0x01436E98
extern CEngineCamera* g_pEngineCamera;                                             // 0x01436EA0
extern CEngine* g_pEngine_01436ea4;                                                // 0x01436EA4
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer;                             // 0x01436E78
extern unsigned char g_highestSetBitTable[256];                                      // 0x0143B990

// CRenderTarget (Ego_r.pdb, 0x24 bytes : CBaseClass; vtable 0x0129C800): no header.
class CRenderTarget : public CBaseClass {
public:
    CSurface RenderToSurface;                                            // +0x04
    CSurface ZBufferSurface;                                             // +0x14
    CRenderTarget() { }
    void AttachZBuffer(const CSurface& surface);                         // 0x00A10E90
    void AttachTarget(const CSurface& surface);                          // 0x00A10E60
    void DetachTarget();                                                 // 0x00A10E10
};
// CEngineBillboard (Ego_r.pdb, 0x18 bytes): no header.
class CEngineBillboard {
public:
    CTexture* Texture;                                                   // +0x00
    CSurface* ZBuffer;                                                   // +0x04
    C3DVector Offset;                                                    // +0x08
    float     Scale;                                                     // +0x14
};

inline unsigned long __fastcall GFGetTopBitInByte(unsigned char value) { return g_highestSetBitTable[value]; }
inline float __fastcall CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier() { return g_pPrimitiveRenderer->MipmapSizeMultiplier; }
FABLE_FORCEINLINE const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index<GraphicList.size() ? &GraphicList[index] : 0;
}
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic,unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}
inline unsigned long CGraphicInfoFrame::GetWidth() const { return GraphicInfo->Header.FrameWidth; }
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

void CEngineBillboardGenerator::RenderBillboard(const C3DMesh2* mesh, CEngineBillboard* billboard, float screenSize, float distance, long alphaRef)
{
    C3DBoundingSphere sphere = mesh->GetBoundingSphere();
    CRenderStateScopedCaptureBlock capture;
    CRenderTarget target;
    if (billboard->ZBuffer)
        target.AttachZBuffer(*billboard->ZBuffer);
    else
        target.AttachZBuffer(*(const CSurface*)g_pEngine_01436ea4->GetScratchZBufferD24S8(billboard->Texture->GetWidth(0)));
    target.AttachTarget(billboard->Texture->GetSurfaceLevel(0));
    NEngineInternal::GPDisplayManager->SetRenderTarget(target);
    NEngineInternal::GPRenderManager->ClearRenderTarget(CRGBColour(0, 0, 0, 0), 0);
    CMatrix3x4 identity;
    identity.SetIdentity();
    NEngineInternal::GPShaderRenderManager->SetViewTransform(identity);
    NEngineInternal::GPShaderRenderManager->SetProjectionTransform(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                                                   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

    float scale = 1.0f / sphere.Radius;
    CMatrix3x4 view = g_pEngineCamera->NoScaleViewTransform;
    CMatrix3x4 world;
    world.SetIdentity();
    world.SetTranslate(-sphere.Center);
    world.PostScale(scale);
    CMatrix3x4 copy(world);
    world.SetMultiplyNoTranslation(copy, view);
    world.PostScale(1.0f, 1.0f, 0.5f);
    world.E43 += 0.5f;
    const float radius = sphere.Radius;
    billboard->Scale = radius * 2.0f;
    billboard->Offset = world.GetTranslation() * radius;
    billboard->Offset.X = -billboard->Offset.X;
    billboard->Offset.Z *= -2.0f;
    NEngineInternal::GPShaderRenderManager->SetWorldTransform(world);

    NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaRef(alphaRef);
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER);
    NEngineInternal::GPRenderManager->SetFogEnable(false);
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(2, C4DVector(1.0f, 1.0f, 1.0f, 1.0f));
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VertexShader, false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PixelShader);

    for (unsigned long i = 0; i < mesh->PrimitiveCount; ++i) {
        C3DPrimitive2& primitive = mesh->Primitives[i];
        unsigned long graphic = mesh->GetMaterial(primitive.MaterialIndex)->DiffuseMapBankIndex;
        if (graphic == 0)
            continue;
        long width = NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(graphic, 0)->GetWidth();
        float mip = (float)width * primitive.AverageTextureStretch * screenSize /
                    (distance * CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier());
        long level = GFFloatToLongFloor(mip);
        if (level < 1)
            level = 1;
        else if (level > 255)
            level = 255;
        long topBit = GFGetTopBitInByte((unsigned char)level);
        C3DTextureStages stages;
        stages.UseBlankTextures = false;
        stages.DiffuseTextureStage = 0;
        stages.BumpTextureStage = 1;
        if (!CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksNoMorph(primitive, *mesh, stages, topBit,
                                                                               NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0), false))
            continue;
        NEngineInternal::GPShaderRenderManager->SelectVertexBuffer(*primitive.VertexBuffer);
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0x11);
        CEnginePrimitiveManagerStaticMeshesBase::SetupFirstTextures(primitive, stages);
        CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocks(primitive, stages, false, false);
    }
    NEngineInternal::GPRenderManager->FlushRender();
    target.DetachTarget();

    CScopedShadersEnable shaders(false, false);
    for (long level = 1; level < billboard->Texture->GetNoLevels(); ++level) {
        long width = billboard->Texture->GetWidth(level);
        target.DetachTarget();
        target.AttachTarget(billboard->Texture->GetSurfaceLevel(level));
        NEngineInternal::GPDisplayManager->SetRenderTarget(target);
        CTVertexRHWColTex1 verts[4];
        float size = (float)width;
        verts[0].X = 0.0f;
        verts[1].X = size;
        verts[2].X = 0.0f;
        verts[3].X = size;
        verts[0].Y = 0.0f;
        verts[1].Y = 0.0f;
        verts[2].Y = size;
        verts[3].Y = size;
        verts[0].U1 = 0.0f;
        verts[1].U1 = 1.0f;
        verts[2].U1 = 0.0f;
        verts[3].U1 = 1.0f;
        verts[0].V1 = 0.0f;
        verts[1].V1 = 0.0f;
        verts[2].V1 = 1.0f;
        verts[3].V1 = 1.0f;
        float halfTexel = 0.5f / size;
        for (long v = 0; v < 4; ++v) {
            verts[v].RHW = 1.0f;
            verts[v].U1 += halfTexel;
            verts[v].Z = 0.0f;
            verts[v].V1 += halfTexel;
        }
        NEngineInternal::GPRenderManager->SetZBufferEnable(false);
        unsigned short indices[6] = { 0, 1, 2, 1, 3, 2 };
        NEngineInternal::GPRenderManager->SetVertexFormat(0x144);
        NEngineInternal::GPRenderManager->SetTextureArgument1(0, TEXTURE_ARG_TEXTURE);
        NEngineInternal::GPRenderManager->SetTextureOperation(0, TEXTURE_OP_SELECT_ARG1);
        NEngineInternal::GPRenderManager->SetTextureAlphaArgument1(0, TEXTURE_ARG_TEXTURE);
        NEngineInternal::GPRenderManager->SetTextureAlphaOperation(0, TEXTURE_OP_SELECT_ARG1);
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
        NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
        NEngineInternal::GPRenderManager->SetTextureMipmapLODBias(0, -1.0f);
        const CTexture* texture = billboard->Texture;
        NEngineInternal::GPRenderManager->DrawAsIndexedTris(&texture, 1, verts, sizeof(CTVertexRHWColTex1), 4, indices, 2);
    }
    NEngineInternal::GPDisplayManager->SetRenderTargetAsBackScreen();
    g_pEngineCamera->RefreshD3DTransforms();
}
