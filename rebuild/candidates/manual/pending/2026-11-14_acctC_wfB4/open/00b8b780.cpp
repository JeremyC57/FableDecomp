// CEngineBillboardGenerator::RenderBillboard(const C3DMesh2* mesh, CEngineBillboard* billboard, float screenSize, float distance, long alphaRef)
// Grade: DIFFER(3921v3921) -- 3 bookmark test/or operand-order sites: 0x00B8BC75 SetAlphaRef, 0x00B8BCE2 SetAlphaTestFunc, 0x00B8C401 loop-2 SetTextureAlphaArgument1 (block pattern ours CCCCSSSSSCCCC, retail CCSSSSSSCCCCC); every other byte equal. A c2 record-id effect, measured (NOTES.md). CRenderTarget from the shared engine/CRenderTarget.h; ONE dead assert, in the mesh loop, on primitive.GetVertexCount() (FableWin: the row is one assert + ~4 instructions larger than this text, and C3DPrimitive2::GetVertexCount is first emitted with this TU); // Inferred: the assert condition.
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
// Inferred: two FABLE_RETAIL_ASSERT statements at the top (mesh, billboard): FableWin ?RenderBillboard 645 /Od instructions v ours 484
//   (+161, /EHsc; ~25 of them are the later compiler's fldz/fld1 constant stores) = about two Lionhead ASSERTs; they emit no bytes.
// Flags: /GS
// Leaf: ?RenderBillboard@CEngineBillboardGenerator@@QAEXPBVC3DMesh2@@PAVCEngineBillboard@@MMJ@Z
// Text evidence (wfB4): EVIDENCED = FableWin /Od size equal (fwsize) and/or FableWin COMDAT emitted with this TU (0x02DBF1B0..0x02DBF920):
//   view.SetTranslateOnly(0,0,0) + world.PostMultiply(view) (SetMultiply 205i, PostMultiply 21i, SetTranslateOnly(MMM) 16i: the twelve sums with the
//   `+ view.E4x` terms folded away and the dead zero stores dropped = retail 0x00B8B9CB..0x00B8BAF3; retail SetMultiplyNoTranslation 0x00A4B820 clears
//   the translation, so it cannot be this call), GetTranslation plain (FableWin 22i = 19i + three later-compiler float pushes), SetPixelShaderConstant
//   subscript text (28i), C3DTextureStages() ctor (14i), C3DMesh2::GetPrimitiveCount / GetMaterial, C3DPrimitive2::GetMaterialIndex /
//   GetAverageTextureStretch / GetVertexBuffer, C3DMeshMaterial::GetDiffuseMapBankIndex, CShaderRenderManager::SelectVertexBuffer through GetD3DDevice().
//   PLAUSIBLE (FableWin has them, user unproven): C3DMesh2::GetPrimitive, CVertexBufferBase::GetD3DVertexBuffer / GetStride inside SelectVertexBuffer.
//   NOT YET PLACED: C3DPrimitive2::GetVertexCount (FableWin emits it with this TU; no retail byte shows its use).
//   INHERITED / FITTED: the two FABLE_RETAIL_ASSERTs (count from agentC22; code-identical), the hand-written 1..255 clamp, sphere members read directly.
#include "fable_compiler.h"
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_ASSERT_DEAD_ARM
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
#define FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY_XYZ
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
#define FABLE_RETAIL_C3DMESH2_PRIMITIVES
#define FABLE_RETAIL_C3DPRIMITIVE2_RENDER
#define FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS
#define FABLE_RETAIL_C3DPRIMITIVE2_GETVERTEXBUFFER
#define FABLE_RETAIL_C3DTEXTURESTAGES_CTOR
#define FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_VERTEXBUFFER
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_BILLBOARD
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MIPMAPSIZEMULTIPLIER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHPRIMITIVE
#define FABLE_RETAIL_CENGINE_SCRATCHZBUFFER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY_SEPARATE
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATE_IDENTITY
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#include "fable_assert.h"
#include "engine/Direct3D9Types.h"
#include "engine/CSurface.h"
#include "engine/CTexture.h"
#include "engine/CBaseClass.h"
#include "engine/CRenderTarget.h"
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
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                                             // 0x01436EA0
namespace NEngineInternal { extern CEngine* GPEngine; }                                                // 0x01436EA4
namespace NEngineInternal { extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; }                             // 0x01436E78
extern unsigned char GTopBitInByteTable[256];                                        // 0x0143B990

// CEngineBillboard (Ego_r.pdb, 0x18 bytes): no header.
class CEngineBillboard {
public:
    CTexture* Texture;                                                   // +0x00
    CSurface* ZBuffer;                                                   // +0x04
    C3DVector Offset;                                                    // +0x08
    float     Scale;                                                     // +0x14
};

inline unsigned long __fastcall GFGetTopBitInByte(unsigned char value) { return GTopBitInByteTable[value]; }
inline float __fastcall CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier() { return NEngineInternal::GPPrimitiveRendererSharedData->MipmapSizeMultiplier; }
FABLE_FORCEINLINE const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index<GraphicList.size() ? &GraphicList[index] : 0;
}
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic,unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}
inline unsigned long CGraphicInfoFrame::GetWidth() const { return GraphicInfo->Header.FrameWidth; }
inline IDirect3DDevice9* CShaderRenderManager::GetD3DDevice() { return D3DDevice; }
inline IDirect3DVertexBuffer9* CVertexBufferBase::GetD3DVertexBuffer() { return PVertexBuffer; }
inline long CVertexBufferBase::GetStride() const { return Stride; }
FABLE_FORCEINLINE void CShaderRenderManager::SelectVertexBuffer(CVertexBufferWin32& vertexBuffer)
{
    GetD3DDevice()->SetStreamSource(0, vertexBuffer.GetD3DVertexBuffer(), 0, vertexBuffer.GetStride());
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
        target.AttachZBuffer(*(const CSurface*)NEngineInternal::GPEngine->GetScratchZBufferD24S8(billboard->Texture->GetWidth(0)));
    target.AttachTarget(billboard->Texture->GetSurfaceLevel(0));
    NEngineInternal::GPDisplayManager->SetRenderTarget(target);
    NEngineInternal::GPRenderManager->ClearRenderTarget(CRGBColour(0, 0, 0, 0), 0);
    CMatrix3x4 identity;
    identity.SetIdentity();
    NEngineInternal::GPShaderRenderManager->SetViewTransform(identity);
    NEngineInternal::GPShaderRenderManager->SetProjectionTransform(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                                                   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

    float scale = 1.0f / sphere.Radius;
    CMatrix3x4 view = NEngineInternal::GPCamera->NoScaleViewTransform;
    view.SetTranslateOnly(0.0f, 0.0f, 0.0f);
    CMatrix3x4 world;
    world.SetTranslate(-sphere.Center);
    world.PostScale(scale);
    world.PostMultiply(view);
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

    for (unsigned long i = 0; i < mesh->GetPrimitiveCount(); ++i) {
        C3DPrimitive2& primitive = *mesh->GetPrimitive(i);
        FABLE_RETAIL_ASSERT(primitive.GetVertexCount() > 0);
        unsigned long graphic = mesh->GetMaterial(primitive.GetMaterialIndex())->GetDiffuseMapBankIndex();
        if (graphic == 0)
            continue;
        long width = NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(graphic, 0)->GetWidth();
        float mip = (float)width * primitive.GetAverageTextureStretch() * screenSize /
                    (distance * CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier());
        long level = GFFloatToLongFloor(mip);
        if (level < 1)
            level = 1;
        else if (level > 255)
            level = 255;
        long topBit = GFGetTopBitInByte((unsigned char)level);
        C3DTextureStages stages;
        stages.DiffuseTextureStage = 0;
        stages.BumpTextureStage = 1;
        if (!CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksNoMorph(primitive, *mesh, stages, topBit,
                                                                               NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0), false))
            continue;
        NEngineInternal::GPShaderRenderManager->SelectVertexBuffer(*primitive.GetVertexBuffer());
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
    NEngineInternal::GPCamera->RefreshD3DTransforms();
}
