// CEnginePrimitiveManagerLines::RenderLine(const CEngineInternalPrimitiveLine& line) -- draw one line as a camera-facing textured quad: clip it to the near plane in view space, offset the ends by half the width across the projected direction, set the blend state and colour constant, draw two triangles
// Grade: DIFFER(3394v3416) -- 46 masked hunks (wfE5 third author; re-authored on shared headers, was 136): frame 0xd0 v 0xc4 (slot packing of start/end/corners), x87 term order in the TransformPoint rows (start Y/Z rows, corner rows), near-plane compare schedule, GetFrame BeginLoading cross-jump (0x00BC9733) and Reset-adopt store order (0x00BC976C).
// Leaf: ?RenderLine@CEnginePrimitiveManagerLines@@QAEXABVCEngineInternalPrimitiveLine@@@Z
// Retail: 0x00BC8A60, 3416 bytes (`ret 4`).
// Local class models: CEngineInternalPrimitiveLine and CEnginePrimitiveManagerLines have no shared header (the landed siblings
// RenderPrimitive 0x00BC9800 / CreateInternalPrimitive 0x00BC7E50 model them locally too). Ego_r.pdb layouts.
#include "fable_compiler.h"
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CSCOPEDSHADERSENABLE_FORCEINLINE
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_GETASINT
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SUMS
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CENGINECAMERA_CAMERASTATE
#define FABLE_RETAIL_CENGINECAMERA_NOSCALEVIEWTRANSFORM
#define FABLE_RETAIL_CENGINECAMERA_NOSCALEVIEWTRANSFORM_CAMERASTATE
#define FABLE_RETAIL_CENGINECAMERA_PEEKNONHOMOGENOUSVIEWTRANSFORM
#define FABLE_RETAIL_CENGINECAMERA_PEEKINVERSENONHOMOGENOUSVIEWTRANSFORM
#define FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CVERTEXBUFFERBASE_ISBUSY_SIZEOF
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_DYNAMICVB
#define FABLE_RETAIL_CTVERTEXCOLTEX1_SETTERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DYNAMICVB
#define FABLE_RETAIL_CSHADERRENDERMANAGER_DRAWNONINDEXED_TEXTURES
#define FABLE_RETAIL_CENGINETREEFILER_INLINES
#define FABLE_RETAIL_CENGINETREEFILER_FINDNODE_INDEX
#define FABLE_RETAIL_CENGINEVERTEXBUFFERPOOL_INLINES
#include "engine/C3DVector.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CArray.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CVertexShader.h"
#include "engine/CTVertexColTex1.h"
#include "engine/EVertexType.h"
#include "engine/EPrimitiveType.h"
#include "engine/CEnginePrimitiveRendererSharedData.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CCountedPointer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicInfo.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/CEngineVertexBufferPool.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/C2DVector.h"
#include "engine/C4DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEngineCamera.h"
#include "engine/CPixelShader.h"

template <class T> FABLE_FORCEINLINE T Min(T a, T b) { return b < a ? b : a; }
template <class T> FABLE_FORCEINLINE T Max(T a, T b) { return b < a ? a : b; }

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; }   // 0x01436E78
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                      // 0x01436EA0

FABLE_FORCEINLINE void CShaderRenderManager::SelectVertexBuffer(CVertexBufferWin32& vertexBuffer)
{
    D3DDevice->SetStreamSource(0, vertexBuffer.PVertexBuffer, 0, vertexBuffer.Stride);
}
FABLE_FORCEINLINE void CShaderRenderManager::DrawBuffersNonIndexed(CVertexBufferWin32& vertexBuffer, unsigned long startVertex,
                                                               unsigned long primitiveCount, EPrimitiveType primitiveType)
{
    SelectVertexBuffer(vertexBuffer);
    RenderManager->RealiseRenderState();
    D3DDevice->DrawPrimitive(primitiveType, startVertex, primitiveCount);
}
FABLE_FORCEINLINE void CShaderRenderManager::DrawBuffersNonIndexed(const CTexture** const textures, unsigned long textureCount,
                                                               CVertexBufferWin32& vertexBuffer, unsigned long startVertex,
                                                               unsigned long primitiveCount, EPrimitiveType primitiveType)
{
    for (unsigned long i = 0; i < textureCount; ++i)
        RenderManager->AttachTextureToStage(i, textures[i]);
    DrawBuffersNonIndexed(vertexBuffer, startVertex, primitiveCount, primitiveType);
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

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }    // 0x01436E98
FABLE_FORCEINLINE const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic, unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}

FABLE_FORCEINLINE unsigned long __fastcall CEnginePrimitiveRenderUtil::GetDynamicVBUsage() { return NEngineInternal::GPPrimitiveRendererSharedData->DynamicVBUsage; }
FABLE_FORCEINLINE EVertexBufferPool __fastcall CEnginePrimitiveRenderUtil::GetDynamicVBPool() { return NEngineInternal::GPPrimitiveRendererSharedData->DynamicVBPool; }
FABLE_FORCEINLINE unsigned long __fastcall CEnginePrimitiveRenderUtil::GetDynamicVBLockFlags() { return NEngineInternal::GPPrimitiveRendererSharedData->DynamicVBLockFlags; }

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


class CEngineInternalPrimitiveLine : public CEngineInternalPrimitiveBase {   // Ego_r.pdb, 0x7c
public:
    C3DVector                 Start;                                     // +0x48
    C3DVector                 End;                                       // +0x54
    CRGBColour                Colour;                                    // +0x60
    float                     Width;                                     // +0x64
    unsigned long             BankIndex;                                 // +0x68
    unsigned long             Frame;                                     // +0x6c
    EEnginePrimitiveBlendMode BlendMode;                                 // +0x70
    EEnginePrimitiveBlendOp   BlendOp;                                   // +0x74
    bool                      UseZBuffer;                                // +0x78
};

class CEnginePrimitiveManagerLines : public CEnginePrimitiveManager {
public:
    CVertexShader VertexShader;                                          // +0x0c
    void RenderLine(const CEngineInternalPrimitiveLine& line);
};

void CEnginePrimitiveManagerLines::RenderLine(const CEngineInternalPrimitiveLine& line)
{
    NEngineInternal::GPShaderRenderManager->ResetWorldTransform();
    CScopedShadersEnable shaders(true, true);
    C3DVector start = line.Start;
    C3DVector end = line.End;
    NEngineInternal::GPCamera->PeekNonHomogenousViewTransform().TransformPoint(start);
    NEngineInternal::GPCamera->PeekNonHomogenousViewTransform().TransformPoint(end);
    float near_plane = NEngineInternal::GPCamera->PeekCameraState().NearZPlane;
    if (start.Z < near_plane) {
        if (end.Z < near_plane)
            return;
        start += (end - start) * ((near_plane - start.Z) / (end.Z - start.Z));
    } else if (end.Z < near_plane) {
        end += (start - end) * ((near_plane - end.Z) / (start.Z - end.Z));
    }
    float inv_start = 1.0f / start.Z;
    C2DVector screen_start(start.X * inv_start, start.Y * inv_start);
    float inv_end = 1.0f / end.Z;
    C2DVector screen_end(end.X * inv_end, end.Y * inv_end);
    C2DVector dir(screen_end.X - screen_start.X, screen_end.Y - screen_start.Y);
    if (!dir.IsNonZero())
        return;
    dir.Normalise();
    float half_width = line.Width * 0.5f;
    dir.X *= half_width;
    dir.Y *= half_width;
    C2DVector perp(dir.Y, -dir.X);
    C3DVector corner0(start.X - perp.X, start.Y - perp.Y, start.Z);
    C3DVector corner1(start.X + perp.X, start.Y + perp.Y, start.Z);
    C3DVector corner2(end.X - perp.X, end.Y - perp.Y, end.Z);
    C3DVector corner3(end.X + perp.X, end.Y + perp.Y, end.Z);
    NEngineInternal::GPCamera->PeekInverseNonHomogenousViewTransform().TransformPoint(corner0);
    NEngineInternal::GPCamera->PeekInverseNonHomogenousViewTransform().TransformPoint(corner1);
    NEngineInternal::GPCamera->PeekInverseNonHomogenousViewTransform().TransformPoint(corner2);
    NEngineInternal::GPCamera->PeekInverseNonHomogenousViewTransform().TransformPoint(corner3);

    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    unsigned long alpha = line.Colour.A;
    unsigned long brightness;
    if (!SetupAlphaBlendingStatesFromBlendMode(line.BlendMode))
        brightness = 0xff;
    else
        brightness = alpha;
    bool textured = line.BankIndex != 0;
    float level = brightness * (1.0f / 255.0f);
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(2, C4DVector(level, level, level, line.Colour.A * (1.0f / 255.0f)));
    if (textured)
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuse);
    else
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseOnly);
    CTVertexColTex1 vertices[4];
    for (long i = 0; i < 4; ++i)
        vertices[i].SetDiffuseColour(line.Colour.GetAsInt());
    vertices[0].SetPosition(corner0);
    vertices[1].SetPosition(corner1);
    vertices[2].SetPosition(corner2);
    vertices[3].SetPosition(corner3);
    vertices[0].SetU1V1(0.0f, 0.0f);
    vertices[1].SetU1V1(1.0f, 0.0f);
    vertices[2].SetU1V1(0.0f, 1.0f);
    vertices[3].SetU1V1(1.0f, 1.0f);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VertexShader, false);
    NEngineInternal::GPShaderRenderManager->ResetWorldTransform();
    NEngineInternal::GPShaderRenderManager->PrepareForRender(1);
    const CTexture* texture;
    unsigned long texture_count = 0;
    if (line.BankIndex != 0) {
        CGraphicFrame frame;
        if (!NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(line.BankIndex, line.Frame)->GetFrame(frame, 0, 0))
            return;
        texture = frame.GetTexture();
        texture_count = 1;
    }
    static unsigned short indices[6] = { 0, 1, 3, 0, 3, 2 };
    NEngineInternal::GPRenderManager->DrawAsIndexedTris(&texture, texture_count, vertices, sizeof(CTVertexColTex1), 4, indices, 2);
}
